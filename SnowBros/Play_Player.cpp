#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>

//#include "SnowBros_Helper.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include <conio.h>
#include <vector>

APlay_Player::APlay_Player()
{
}

APlay_Player::~APlay_Player()
{
}



//// 이동관련! 가속도 빼고, 점프 ㄱㄱ


void APlay_Player::BeginPlay()
{
	AActor::BeginPlay();
	MainPlayer = this;  // 아오 이걸 주석처리해놔서 계속 플레이어 없음이 떴네 ..
	

	{
		Renderer = CreateImageRenderer(SnowBrosRenderOrder::Player);
		Renderer->SetImage("SnowBros_Run_R.png");
		Renderer->SetImage("SnowBros_Run_L.png");
		Renderer->SetImage("SnowBros_Jump_R.png");
		Renderer->SetImage("SnowBros_Melt.png");
		Renderer->SetImage("SnowBros_Attack_R.png");
		Renderer->SetImage("SnowBros_Attack_L.png");

		Renderer->SetTransform({ {0,0}, {64*1.1f, 128*1.1f} });
		Renderer->CreateAnimation("Idle_Right", "SnowBros_Idle_R.png", 0, 0, 1.0f, true);
		Renderer->CreateAnimation("Idle_Left", "SnowBros_Idle_L.png", 0, 0, 1.0f, true);
		
		Renderer->CreateAnimation("Run_Right", "SnowBros_Run_R.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Run_Left", "SnowBros_Run_L.png", 0, 3, 0.1f, true);

		Renderer->CreateAnimation("Jump_Right", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);
		Renderer->CreateAnimation("Jump_Left", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);
		
		Renderer->CreateAnimation("DownJump_Left", "SnowBros_Melt.png", 0, 6, 0.1f, true);

		Renderer->CreateAnimation("Attack_Right", "SnowBros_Attack_R.png", 0, 3, 0.02f, true);
		Renderer->CreateAnimation("Attack_Left", "SnowBros_Attack_L.png", 0, 3, 0.02f, true);
		

		StateChange(EPlayState::Idle);
	}


	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Player);
		BodyCollision->SetPosition(Renderer->GetPosition());
		BodyCollision->SetScale({ 32, 72 });
		BodyCollision->SetColType(ECollisionType::Rect);

	}

	/*
	
	
	*/
}



void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	StateUpdate(_DeltaTime);
}



void APlay_Player::DirCheck()
{
	EActorDir Dir = DirState;

	if (UEngineInput::IsDown(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsDown(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

/*여기에 Jump를 추가하니까, '그런 이름의 애니메이션은 없다~'라고. */

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer-> GetCurAnimationTime());
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
		Renderer->ChangeAnimation(Name);
	}
}






std::string APlay_Player::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (DirState)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;

}



void APlay_Player::StateChange(EPlayState _State)
{ // 애니메이션용

	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle: // 가만히
			IdleStart();
			break;
		case EPlayState::Run: // 달리기
			RunStart();
			break;
		case EPlayState::Jump: //위로 점프
			JumpStart();
			break;
		case EPlayState::DownJump: // 아래로 점프
			DownJumpStart();
			break;
		case EPlayState::Attack: // 공격
			AttackStart();
			break;
		
		case EPlayState::FastRun: // 포션먹고 빨리달리기
			FastRunStart();
			break;
	
		case EPlayState::Fly: // Stage이동할 때 날기
			FlyStart();
			break;

		default:
			break;
		}
	}

	State = _State;

}



void APlay_Player::StateUpdate(float _DeltaTime)
{ // 함수용
	switch (State)
	{
	case EPlayState::CameraFreeMove: //안씀
		//CameraFreeMove(_DeltaTime);
		break;
	case EPlayState::FreeMove: //안씀
		//FreeMove(_DeltaTime);
		break;
	case EPlayState::Idle: // 가만히
		Idle(_DeltaTime);
		break;
	case EPlayState::Run: //달리기
		Run(_DeltaTime);
		break;
	case EPlayState::FastRun: // 포션먹고빠르게달리기
		FastRun(_DeltaTime);
		break;
	case EPlayState::Jump: // 위로 점프
		Jump(_DeltaTime);
		break;
	case EPlayState::DownJump: //아래로 점프
		DownJump(_DeltaTime);
		break;
	case EPlayState::Attack: // 기본 공격
		Attack(_DeltaTime);
		break;
	case EPlayState::Strobe: // 충돌시 깜빡깜빡
		Strobe(_DeltaTime);
		break;
	case EPlayState::Fly: // Stage이동할 때 날기
		Fly(_DeltaTime);
		break;
	default:
		break;
	}


}


void APlay_Player::PlayerColState(EPlayState _State)
{

	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Strobe: // 공격받으면 깜빡깜빡
			StrobeStart();
			break;

		default:
			break;
		}
	}


	State = _State;

}

void APlay_Player::IdleStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
	
}

void APlay_Player::RunStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Run"));
	DirCheck();
}

void APlay_Player::JumpStart()
{
	JumpVector = JumpPower;
	Renderer->ChangeAnimation(GetAnimationName("Jump"));
	DirCheck();
}

void APlay_Player::DownJumpStart()
{
	Renderer->ChangeAnimation(GetAnimationName("DownJump"));
	DirCheck();
}


void APlay_Player::FastRunStart()
{
	Renderer->ChangeAnimation(GetAnimationName("FastRun"));
	DirCheck();

}


void APlay_Player::StrobeStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Strobe"));
	DirCheck();
	this->Strobe(0.1f);
}


void APlay_Player::FlyStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Fly"));

}

void APlay_Player::AttackStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Attack"));
	DirCheck();
	Fire_Bullet();
	
}



void APlay_Player::Idle(float _DeltaTime)
{
	//MoveVector = FVector::Zero;
	DirCheck();
	PlayerColPhysics(_DeltaTime);
	MoveUpdate(_DeltaTime);

	//Idle상태에서
	///방향키를 누르면 왼/오 이동
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayState::Run);
		return;
	}
	///왼쪽Shift로 점프
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;
	}
	///Z키로 공격
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayState::Attack);
		return;
	}
}



void APlay_Player::Run(float _DeltaTime)
{
	DirCheck();
	PlayerColPhysics(_DeltaTime);
	

	
	//양쪽 방향키 둘다 안눌렸으면 ; Idle
	if (true == UEngineInput::IsFree(VK_LEFT) 
		&& true == UEngineInput::IsFree(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Idle);
		//MoveUpdate(_DeltaTime);
		return;
	}

	//뛰는 동안에 점프키 누르면 점프
	//방향키 누르는 방향으로 전진 - 가속도 필요없음
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;		
	}

	if (true==UEngineInput::IsPress(VK_LEFT))
	{

		AddMoveVector(FVector::Left * _DeltaTime);
		//return;
	}


	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right*_DeltaTime);	
		
	}
	
	
	MoveUpdate(_DeltaTime);

	FVector CheckPos = GetActorLocation();
	
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 15;
		break;
	case EActorDir::Right:
		CheckPos.X += 15;
		break;
	default:
		break;
	}
	CheckPos.Y -= 15;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	if (Color != Color8Bit(0, 255, 255, 0)) // 플레이어 x의 +-15, y의 -15가 cyan이 아니면, 계속 감. cyan이면 멈춤
	{
		AddActorLocation(MoveVector); /// 이거 잘봐!!
		return;
	}

	//MoveUpdate(_DeltaTime);
}


void APlay_Player::FastRun(float _DeltaTime)
{
}


void APlay_Player::Strobe(float _StrobeTime)
{
	float Strobetime = _StrobeTime;
	StrobeUpdate(_StrobeTime);

	//IsStrobeUpdate = false;
	//if -> 0초보다 크면, 점점 줄어들고 그동안 깜빡
	//if (0.0f <= _StrobeTime)
	//{
	//	for (_StrobeTime; _StrobeTime == 0.0f; _StrobeTime--)
	//	{
	//		StrobeUpdate(_StrobeTime);
	//		//깜빡거리는거지 
	//	}
	//}
	//else
	//{
	//	APlay_Player::StateChange(EPlayState::Idle);
	//}

	/*if (_StrobeTime < 5.0f)
	{
		Renderer->ChangeAnimation(GetAnimationName("Idle"));
		StateChange(EPlayState::Idle);

	}*/



}


void APlay_Player::StrobeUpdate(float _DeltaTime)
{

	AlphaTime += _DeltaTime;

	if (0.1f <= AlphaTime)
	{
		Dir = !Dir;
		AlphaTime = 0.0f;
	}
	if (true == Dir)
	{
		Renderer->SetAlpha(AlphaTime);
	}
	else
	{
		Renderer->SetAlpha(0.5f - AlphaTime);
	}
	if (AlphaTime > 5.0f)
	{
		Renderer->ChangeAnimation(GetAnimationName("Idle"));
		StateChange(EPlayState::Idle);
		

	}

}



void APlay_Player::Attack(float _DeltaTime)
{
	DirCheck();
	

	if(Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayState::Idle);
		return; 
	}
	MoveUpdate(_DeltaTime);

}



void APlay_Player::Fire_Bullet()
{// bullet방향을 tick에서 해버리면, 플레이어가 방향 전환 할 때마다 ㅜㅜ 
// 공격파티클 방향도 같이 전환되어벌임 ㅠㅠ


	DirCheck();
	APlay_Bullet* Bullet = GetWorld()->SpawnActor<APlay_Bullet>();
	Bullet->SetName("Bullet");
	Bullet->SetActorLocation(this->GetActorLocation());


	switch (DirState)
	{
	case EActorDir::None:
		break;
	
	case EActorDir::Left:
	Bullet->Dir = FVector::Left;
	Bullet->SetAnimation("Bullet_Left");
	//Bullet의 애니메이션을 정해주는 함수 필요
	//==>>>G헐허러러하러러러도ㅒㅆ다됐다!!!! 양쪽으로 방향따라서 애니메이션!!!!!!! ㅠㅠㅠㅠㅠㅠㅠㅠ
	// 충돌하면 -> 파티클 모양 바뀌는걸.. 여기서 해야할 것 같은데.. 왜 안될까..
	
	break;
	
	case EActorDir::Right:
		Bullet->Dir = FVector::Right;	
		Bullet->SetAnimation("Bullet_Right");
		
		break;
	
	default:
		break;
	}
	
	if (true == Bullet->IsBulletCol)
	{
		Bullet->SetAnimation("BulletCol");
	}


	
	return;
}




void APlay_Player::Jump(float _DeltaTime)
{	
	DirCheck();
	PlayerColPhysics(_DeltaTime);
	
	

	if ( true == UEngineInput::IsDown(VK_RIGHT))
	{
		JumpVector += FVector::Right * _DeltaTime;
		return;
	}

	if (true == UEngineInput::IsDown(VK_LEFT) )
	{
		JumpVector += FVector::Left * _DeltaTime;
		return;
	}


	MoveUpdate(_DeltaTime);


	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		JumpVector = FVector::Zero;
		//	JumpVector = FVector::Zero;
		StateChange(EPlayState::Idle);
		return;
	}
		

}



void APlay_Player::DownJump(float _DeltaTime)
{
	// Down키 + Jump키  = 아래단으로 내려가기 => 그 순간에만 바닥 충돌 컬러 바꾸면 되나!?

	DirCheck();
	PlayerColPhysics(_DeltaTime);

	//Jump 상태에서, 스페이스바나 방향키 모두 안눌려있을때 -> Idle로 돌아가기
	

}




void APlay_Player::Fly(float _DeltaTime)
{
}



//플레이어 충돌시 
void APlay_Player::PlayerColPhysics(float _DeltaTime)
{	
	std::vector<UCollision*> MonsterResult;
	// 설마 설마.. 헤더에 벡터 추가 안해서 ㄱ여태 안됏던 것 같다.??. 잠시 멍청했지만 이제 극복햇다
	//해내버렸다.. 
	
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{
		AActor* Owner = MonsterResult[0]->GetOwner();
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

		if (nullptr == Monster)
		{
			MsgBoxAssert("몬스터가 아닙니다");
		}

		
		if (EMonsterState::Snowball != Monster->GetState())
		{
			Strobe(_DeltaTime);
		}
		else if(EMonsterState::Snowball == Monster->GetState())
		{
			Monster->StateChange(EMonsterState::Rolling);
		}


		return;
	}
	
	Renderer->SetAlpha(1.0f);
	// 몬스터에서 벗어나면 알파값 다시 255(=1.0f)로 돌아오도록
	// 3초 쯤 뒤에 돌아오는로 수정

	

}

void APlay_Player::AddMoveVector(const FVector& _DirDelta) // 가속도 -> 등속으로 바꿈
{
	MoveVector = _DirDelta * MoveAcc;
}


void APlay_Player::CalMoveVector(float _DeltaTime)
{


	FVector CheckPos = GetActorLocation();
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 30;
		break;
	case EActorDir::Right:
		CheckPos.X += 30;
		break;
	default:
		break;
	}
	CheckPos.Y -= 32.0f;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		MoveVector = FVector::Zero; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
	}

	if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		MoveVector = FVector::Zero;
	}

}



void APlay_Player::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // 중력가속도에 의해 움직인 위치. \

	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// 땅에 닿으면 0으로 초기화 
	}
}



// 나는 카메라 Y축으로 움직여야함? 아니면 카메라는 놔두고, 스테이지 이동할 때만 어떻게든 움직이는걸로
void APlay_Player::CamMoveLastMoveVector(float _DeltaTime)
{
	// 카메라는 x축으로만 움직여야 하니까.
	//GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlay_Player::CalLastMoveVector(float _DeltaTime)
{
	// 제로로 만들어서 초기화 시킨다.
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + MoveVector;
	LastMoveVector = LastMoveVector + GravityVector;
	LastMoveVector = LastMoveVector + JumpVector;
	 // 왜 또 더해줌?
	// 최종 위치 = x축으로 움직인 위치 + 점프+ 중력

}



void APlay_Player::MoveUpdate(float _DeltaTime)
{
	CalMoveVector(_DeltaTime);
	CalGravityVector(_DeltaTime);
	CalLastMoveVector(_DeltaTime);
	CamMoveLastMoveVector(_DeltaTime);
	

	//GroundUp();

}

////////////////////////////////

APlay_Player* APlay_Player::MainPlayer = nullptr;

APlay_Player* APlay_Player::GetMainPlayer()
{
	return MainPlayer;
}


//////////////////

//////////////////

//
//void APlay_Player::CameraFreeMove(float _DeltaTime)
//{
//	if (UEngineInput::IsPress(VK_LEFT))
//	{
//		GetWorld()->AddCameraPos(FVector::Left * _DeltaTime * 500.0f);
//		// AddActorLocation(FVector::Left * _DeltaTime * 500.0f);
//	}
//
//	if (UEngineInput::IsPress(VK_RIGHT))
//	{
//		GetWorld()->AddCameraPos(FVector::Right * _DeltaTime * 500.0f);
//	}
//
//	if (UEngineInput::IsPress(VK_UP))
//	{
//		GetWorld()->AddCameraPos(FVector::Up * _DeltaTime * 500.0f);
//		// AddActorLocation(FVector::Up * _DeltaTime * 500.0f);
//	}
//
//	if (UEngineInput::IsPress(VK_DOWN))
//	{
//		GetWorld()->AddCameraPos(FVector::Down * _DeltaTime * 500.0f);
//		// AddActorLocation(FVector::Down * _DeltaTime * 500.0f);
//	}
//
//	if (UEngineInput::IsDown('2'))
//	{
//		StateChange(EPlayState::Idle);
//	}
//}
//
//void APlay_Player::FreeMove(float _DeltaTime)
//{
//	FVector MovePos;
//
//	if (UEngineInput::IsPress(VK_LEFT))
//	{
//		MovePos += FVector::Left * _DeltaTime * FreeMoveSpeed;
//	}
//
//	if (UEngineInput::IsPress(VK_RIGHT))
//	{
//		MovePos += FVector::Right * _DeltaTime * FreeMoveSpeed;
//	}
//
//	if (UEngineInput::IsPress(VK_UP))
//	{
//		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
//	}
//
//	if (UEngineInput::IsPress(VK_DOWN))
//	{
//		MovePos += FVector::Down * _DeltaTime * FreeMoveSpeed;
//	}
//
//	AddActorLocation(MovePos);
//	GetWorld()->AddCameraPos(MovePos);
//
//	if (UEngineInput::IsDown('1'))
//	{
//		StateChange(EPlayState::Idle);
//	}
//}
//


