#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>

#include "SnowBros_Helper.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include "Play_Physics_Core.h"
#include <conio.h>

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

		Renderer->SetTransform({ {0,0}, {64, 128} });
		Renderer->CreateAnimation("Idle_Right", "SnowBros_Idle_R.png", 0, 0, 1.0f, true);
		Renderer->CreateAnimation("Idle_Left", "SnowBros_Idle_L.png", 0, 0, 1.0f, true);
		
		Renderer->CreateAnimation("Run_Right", "SnowBros_Run_R.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Run_Left", "SnowBros_Run_L.png", 0, 3, 0.1f, true);

		Renderer->CreateAnimation("Jump_Right", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);
		Renderer->CreateAnimation("Jump_Left", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);
		
		Renderer->CreateAnimation("DownJump_Left", "SnowBros_Melt.png", 0, 6, 0.1f, true);

		Renderer->CreateAnimation("Attack_Right", "SnowBros_Melt.png", 0, 1, 0.01f, true);
		Renderer->CreateAnimation("Attack_Left", "SnowBros_Melt.png", 0, 1, 0.01f, true);
		

		StateChange(EPlayState::Idle);
	}
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


void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	// 충돌 순간 = 0.0 으로 초기화 해줘야하는구나
	

	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Player);
		BodyCollision->SetScale({ 100, 100 });
		BodyCollision->SetColType(ECollisionType::Rect);

	}


	//APlay_Player::Strobe(_DeltaTime);


	//std::vector<UCollision*> Result;
	//if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	//{
	//	// 이런식으로 상대를 사용할수 있다.
	//	BodyCollision->SetActive(true, 0.5f);
	//	// BodyCollision = nullptr;
	//}
	//UCollision* CollisionPlay = Result[0];
	

	//	if (_DeltaTime < 0.5f)
	//	{
	//		APlay_Player::Strobe(_DeltaTime);
	//	}
	//	else
	//	{
	//		StateChange(EPlayState::Idle);
	//	}

	//}



	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	


	StateUpdate(_DeltaTime);
}




void APlay_Player::Strobe(float _StrobeTime)
{
	StrobeUpdate(_StrobeTime);

	//IsStrobeUpdate = false;
	float Strobetime = _StrobeTime;
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

	if(AlphaTime > 5.0f)
	{
		Renderer->ChangeAnimation(GetAnimationName("Idle"));


		/*StateChange(EPlayState::Idle);*/
	
				
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
		case EPlayState::Strobe: // 공격받으면 깜빡깜빡
			StrobeStart();
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
	default:
		break;
	}


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

}


void APlay_Player::FlyStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Fly"));

}

void APlay_Player::AttackStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Attack"));
	//Fire_Bullet();
	DirCheck();

	/*
	if (Player->DirState == EActorDir::Right)
		{
			this->SetBulletDir(FVector::Right);
		}
		else
		{
			return;
		}
	*/

}







void APlay_Player::Idle(float _DeltaTime)
{
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

	MoveUpdate(_DeltaTime);

}



void APlay_Player::Run(float _DeltaTime)
{
	DirCheck();
	

	
	//양쪽 방향키 둘다 안눌렸으면 ; Idle
	if (true == UEngineInput::IsFree(VK_LEFT) 
		&& true == UEngineInput::IsFree(VK_RIGHT)
		&& true == UEngineInput::IsFree('Z'))
	{
		StateChange(EPlayState::Idle);
		//MoveUpdate(_DeltaTime);
		return;
	}

	//뛰는 동안에 점프키 누르면 점프
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;		
	}

	//방향키 누르는 방향으로 전진 - 가속도 필요없음
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


void APlay_Player::Attack(float _DeltaTime)
{
	DirCheck();
	Fire_Bullet();
	if (Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayState::Idle);
		return; 
	}

	MoveUpdate(_DeltaTime);

}

void APlay_Player::Fire_Bullet()
{
	//APlay_Player* Player = nullptr;
	APlay_Bullet* Bullet = GetWorld()->SpawnActor<APlay_Bullet>();
	Bullet->SetName("Bullet");
	Bullet->SetActorLocation(this->GetActorLocation());



	//fvector                float
	//Bullet->SetBulletDir(Player->DirState);
	// 아 이거 이렇게 바꾸면 좋은데.. 바꾸는 법을 모르겟다

	return;



	// APlay_Bullet::BulletFired실행 만들기 


}




void APlay_Player::Jump(float _DeltaTime)
{	
	DirCheck();
	FVector JumpPos;

	if (true == UEngineInput::IsUp('Z'))
	{

		JumpVector = FVector::Zero;
		
		MoveUpdate(_DeltaTime);
		//return;

	}
	if (true == UEngineInput::IsFree('Z')
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		JumpVector = FVector::Zero;
		StateChange(EPlayState::Idle);
		return;
	}


	if (/*true == UEngineInput::IsPress(VK_LEFT) ||*/ true == UEngineInput::IsPress(VK_RIGHT))
	{
		JumpPos += FVector::Left * _DeltaTime;
		
		/*StateChange(EPlayState::Idle);
		
		return;*/
	}

	if (true == UEngineInput::IsPress(VK_LEFT) /*|| true == UEngineInput::IsPress(VK_RIGHT)*/)
	{
		JumpPos += FVector::Right * _DeltaTime;

		/*StateChange(EPlayState::Idle);

		return;*/
	}

	if (true == UEngineInput::IsPress(VK_LEFT) && true == UEngineInput::IsDown('Z'))
	{

		JumpPos = JumpPower;
		CalGravityVector(_DeltaTime);
		// 수정 필요
		AddActorLocation(JumpPos);
		if (UEngineInput::IsUp('Z'))
		{
			JumpPos = FVector::Zero;
			StateChange(EPlayState::Idle);
		}

	}
		MoveUpdate(_DeltaTime);


		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
		if (Color == Color8Bit(0, 255, 255, 0))
		{
			JumpPos = FVector::Zero;
			//	JumpVector = FVector::Zero;
			StateChange(EPlayState::Idle);
			return;
		}

	


	
	//JumpPos = FVector::Zero;
	//MoveUpdate(_DeltaTime);

}



void APlay_Player::DownJump(float _DeltaTime)
{
	// Down키 + Jump키  = 아래단으로 내려가기 => 그 순간에만 바닥 충돌 컬러 바꾸면 되나!?

	DirCheck();

	//Jump 상태에서, 스페이스바나 방향키 모두 안눌려있을때 -> Idle로 돌아가기
	if (true == UEngineInput::IsFree('X')
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


}




void APlay_Player::Fly(float _DeltaTime)
{
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

void APlay_Player::CalJumpVector(float _DeltaTime)
{

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


