#include "Play_Player.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include "SnowBros_Helper.h"

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
		Renderer->SetImage("SnowBros_Test_char.png");
		Renderer->SetTransform({ {0,0}, {64, 128} });
		Renderer->CreateAnimation("Idle_Right", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Move_Right", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Jump_Right", "SnowBros_Test_Jump.png", 0, 6, 0.1f, true);

		Renderer->CreateAnimation("Idle_Left", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Move_Left", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Jump_Left", "SnowBros_Test_Jump.png", 0, 6, 0.1f, true);
		Renderer->CreateAnimation("DownJump_Left", "SnowBros_Test_Jump.png", 0, 0, 0.1f, true);

		StateChange(EPlayState::Idle);
	}
	
	/*{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Player);
		BodyCollision->SetScale({ 64, 64 });
		BodyCollision->SetColType(ECollisionType::Circle);
	}*/

}



void APlay_Player::DirCheck()
{
	EActorDir Dir = DirState;

	if (UEngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsPress(VK_RIGHT))
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
{

	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle:
			IdleStart();
			break;
		case EPlayState::Run:
			MoveStart();
			break;
		case EPlayState::Jump:
			JumpStart();
			break;
		case EPlayState::DownJump:
			DownJumpStart();
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

void APlay_Player::MoveStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Move"));
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


void APlay_Player::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EPlayState::CameraFreeMove: //안씀
		CameraFreeMove(_DeltaTime);
		break;
	case EPlayState::FreeMove: //안씀
		FreeMove(_DeltaTime);
		break;
	case EPlayState::Idle:
		Idle(_DeltaTime);
		break;
	case EPlayState::Run:
		Run(_DeltaTime);
		break;
	case EPlayState::Jump:
		Jump(_DeltaTime);
		break;
	case EPlayState::DownJump:
		DownJump(_DeltaTime);
		break;
	default:
		break;
	}


}


void APlay_Player::Idle(float _DeltaTime)
{

	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayState::Run);
		return;
	}



	if (true == UEngineInput::IsDown(VK_LSHIFT))
	{
		StateChange(EPlayState::Jump);
		return;
	}


	MoveUpdate(_DeltaTime);

}



void APlay_Player::Run(float _DeltaTime)
{
	DirCheck();
	MoveUpdate(_DeltaTime);
	
	//양쪽 방향키 둘다 안눌렸으면 ; Idle
	if (true == UEngineInput::IsFree(VK_LEFT) 
		&& UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);
		return;
	}

	// Run상태에서 Jump키 누르는 순간 변경
	if (true == UEngineInput::IsDown(VK_LSHIFT))
	{
		StateChange(EPlayState::Jump);
		return;
	}


	//방향키 누르는 방향으로 전진 - 가속도 필요없음
	if (UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime);
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime);
	}



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
	CheckPos.Y -= 30;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	if (Color != Color8Bit(0, 255, 255, 0))
	{
		AddActorLocation(MoveVector); /// 이거 잘봐!!
		//GetWorld()->AddCameraPos(MovePos);
	}

	MoveUpdate(_DeltaTime);
}




void APlay_Player::Jump(float _DeltaTime)
{
	FVector JumpPos;

	if (true == UEngineInput::IsFree(VK_LSHIFT)
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		JumpVector = FVector::Zero;
		StateChange(EPlayState::Idle);
		return;
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime);
	}
		if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime);
	}





	if (UEngineInput::IsDown(VK_LSHIFT))
	{
	
		JumpPos =  JumpPower;
		CalGravityVector(_DeltaTime);
		// 수정 필요
		AddActorLocation(JumpPos);
		if (UEngineInput::IsUp(VK_LSHIFT))
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


}




void APlay_Player::DownJump(float _DeltaTime)
{// Down키 + Jump키  = 아래단으로 내려가기 => 그 순간에만 바닥 충돌 컬러 바꾸면 되나!?

	DirCheck();

	//Jump 상태에서, 스페이스바나 방향키 모두 안눌려있을때 -> Idle로 돌아가기
	if (true == UEngineInput::IsFree(VK_SPACE)
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	FVector MovePos;

	


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
	CheckPos.Y -= 30;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);

	if (Color == Color8Bit(0, 255, 255, 0))
	{
		MoveVector = FVector::Zero; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
	}


}

void APlay_Player::CalJumpVector(float _DeltaTime)
{

}

void APlay_Player::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // 중력가속도에 의해 움직인 위치. 
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
	LastMoveVector = LastMoveVector + JumpVector;
	LastMoveVector = LastMoveVector + GravityVector;
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


void APlay_Player::CameraFreeMove(float _DeltaTime)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		GetWorld()->AddCameraPos(FVector::Left * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Left * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		GetWorld()->AddCameraPos(FVector::Right * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_UP))
	{
		GetWorld()->AddCameraPos(FVector::Up * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Up * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsPress(VK_DOWN))
	{
		GetWorld()->AddCameraPos(FVector::Down * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Down * _DeltaTime * 500.0f);
	}

	if (UEngineInput::IsDown('2'))
	{
		StateChange(EPlayState::Idle);
	}
}

void APlay_Player::FreeMove(float _DeltaTime)
{
	FVector MovePos;

	if (UEngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_UP))
	{
		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
	}

	if (UEngineInput::IsPress(VK_DOWN))
	{
		MovePos += FVector::Down * _DeltaTime * FreeMoveSpeed;
	}

	AddActorLocation(MovePos);
	GetWorld()->AddCameraPos(MovePos);

	if (UEngineInput::IsDown('1'))
	{
		StateChange(EPlayState::Idle);
	}
}






void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);



	StateUpdate(_DeltaTime);
}
