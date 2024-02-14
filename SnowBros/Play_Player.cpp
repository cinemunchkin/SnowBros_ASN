#include "Play_Player.h"
#include <EnginePlatform\EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include "SnowBros_Helper.h"

APlay_Player::APlay_Player()
{
}

APlay_Player::~APlay_Player()
{
}

void APlay_Player::BeginPlay()
{
	AActor::BeginPlay();

	//MainPlayer = this; 
	//			/* ATestPlayer* ATestPalyer:: MainPlayer, ATestPlayer* = This */
	//			/*static ATestPlayer* MainPlayer*/
	Renderer = CreateImageRenderer(SnowBrosRenderOrder::Player);
	Renderer->SetImage("SnowBros_Test_char.png");
	Renderer->SetTransform({ {0,0}, {64, 128} });
	Renderer->CreateAnimation("Idle_Right", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
	Renderer->CreateAnimation("Move_Right", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
	Renderer->CreateAnimation("Jump_Right", "SnowBros_Test_Jump.png", 0, 6, 0.1f, true);

	Renderer->CreateAnimation("Idle_Left", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
	Renderer->CreateAnimation("Move_Left", "SnowBros_Test_char.png", 0, 3, 0.1f, true);
	Renderer->CreateAnimation("Jump_Left", "SnowBros_Test_Jump.png", 0, 6, 0.1f, true);

	// Renderer->ChangeAnimation("Idle_Right");


	StateChange(EPlayState::Idle);
}

void APlay_Player::GravityCheck(float _DeltaTime)
{
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color != Color8Bit(0, 255, 255, 0))
	{
		AddActorLocation(FVector::Down * _DeltaTime * Gravity);
	}
}


void APlay_Player::GravityOff(float _DeltaTime)
{
	
		AddActorLocation(FVector::Down * _DeltaTime/* * Gravity*/);
	
}


void APlay_Player::DirCheck()
{
	EActorDir Dir = DirState;
	if (EngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (EngineInput::IsPress(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

/*여기에 Jump를 추가하니까, '그런 이름의 애니메이션은 없다~'라고. */

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		//Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationframe(), Renderer-> GetCurAnimationTime());
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
		//Renderer->ChangeAnimation(Name);
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
	Renderer->ChangeAnimation(GetAnimationName("Jump"));
	DirCheck();
}

void APlay_Player::StateChange(EPlayState _State)
{
	// 이전상태와 지금 상태가 같지 않아
	// 이전에는 move 지금은 Idle
	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle:
			IdleStart();
			break;
		case EPlayState::Move:
			MoveStart();
			break;
		case EPlayState::Jump:
			JumpStart();
			break;
		default:
			break;
		}
	}

	State = _State;


}

void APlay_Player::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EPlayState::CameraFreeMove:
		CameraFreeMove(_DeltaTime);
		break;
	case EPlayState::FreeMove:
		FreeMove(_DeltaTime);
		break;
	case EPlayState::Idle:
		Idle(_DeltaTime);
		break;
	case EPlayState::Move:
		Move(_DeltaTime);
		break;
	case EPlayState::Jump:
		Jump(_DeltaTime);
		break;
	default:
		break;
	}


}

void APlay_Player::CameraFreeMove(float _DeltaTime)
{
	if (EngineInput::IsPress(VK_LEFT))
	{
		GetWorld()->AddCameraPos(FVector::Left * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Left * _DeltaTime * 500.0f);
	}

	if (EngineInput::IsPress(VK_RIGHT))
	{
		GetWorld()->AddCameraPos(FVector::Right * _DeltaTime * 500.0f);
	}

	if (EngineInput::IsPress(VK_UP))
	{
		GetWorld()->AddCameraPos(FVector::Up * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Up * _DeltaTime * 500.0f);
	}

	if (EngineInput::IsPress(VK_DOWN))
	{
		GetWorld()->AddCameraPos(FVector::Down * _DeltaTime * 500.0f);
		// AddActorLocation(FVector::Down * _DeltaTime * 500.0f);
	}

	if (EngineInput::IsDown('2'))
	{
		StateChange(EPlayState::Idle);
	}
}

void APlay_Player::FreeMove(float _DeltaTime)
{
	FVector MovePos;

	if (EngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * FreeMoveSpeed;
	}

	if (EngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * FreeMoveSpeed;
	}

	if (EngineInput::IsPress(VK_UP))
	{
		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
	}

	if (EngineInput::IsPress(VK_DOWN))
	{
		MovePos += FVector::Down * _DeltaTime * FreeMoveSpeed;
	}

	AddActorLocation(MovePos);
	GetWorld()->AddCameraPos(MovePos);

	if (EngineInput::IsDown('1'))
	{
		StateChange(EPlayState::Idle);
	}
}


void APlay_Player::Idle(float _DeltaTime)
{
	/* 왼쪽 오른쪽도 안되고 있고.
	 여기서는 정말
	 가만히 있을때만 어떻게 할지 신경쓰면 됩니다.*/
	if (true == EngineInput::IsDown('1'))
	{
		StateChange(EPlayState::FreeMove);
		return;
	}

	if (true == EngineInput::IsDown('2'))
	{
		StateChange(EPlayState::CameraFreeMove);
		return;
	}


	if (
		true == EngineInput::IsPress(VK_LEFT) ||
		true == EngineInput::IsPress(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Move);
		return;
	}

	if (
		true == EngineInput::IsPress(VK_SPACE)
		)
	{
		StateChange(EPlayState::Jump);
		return;
	}

	GravityCheck(_DeltaTime);
}





void APlay_Player::Jump(float _DeltaTime)
{
	DirCheck();
	GravityCheck(_DeltaTime);

	//Jump 상태에서, 스페이스바나 방향키 모두 안눌려있을때 -> Idle로 돌아가기
	if (true == EngineInput::IsFree(VK_SPACE)
		&& EngineInput::IsFree(VK_RIGHT)
		&& EngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	FVector MovePos;
	if (EngineInput::IsPress(VK_SPACE))
	{
		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
				
	}
	AddActorLocation(MovePos);
	//됐다 점프!!!


//Jump키 떼는 순간, 중력 체크
	

	// 이건 안먹ㅇ히네.. 위로 충돌하는거 막기
	FVector CheckPos = GetActorLocation();

	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 30;
		break;
	case EActorDir::Right:
		CheckPos.X += 30;
		break;
		
	case EActorDir::Jump:
		CheckPos.Y -= 60;
		break;
	default:
		break;
	}
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);

	if (Color != Color8Bit(0, 255, 255, 0))
	{
		AddActorLocation(MovePos);
	
	}

}



void APlay_Player::DownJump(float _DeltaTime)
{// Down키 + Jump키  = 아래단으로 내려가기 => 그 순간에만 바닥 충돌 컬러 바꾸면 되나!?

	DirCheck();

	//Jump 상태에서, 스페이스바나 방향키 모두 안눌려있을때 -> Idle로 돌아가기
	if (true == EngineInput::IsFree(VK_SPACE)
		&& EngineInput::IsFree(VK_RIGHT)
		&& EngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	FVector MovePos;

	if (EngineInput::IsPress('a'))
	{
		
		return GravityOff(_DeltaTime);

	}



}




void APlay_Player::Move(float _DeltaTime)
{
	DirCheck();
	GravityCheck(_DeltaTime);

	if (true == EngineInput::IsFree(VK_LEFT) && EngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	if (true == EngineInput::IsDown(VK_SPACE))
	{// Move상태에서 Jump키 누르는 순간 status 변경
		StateChange(EPlayState::Jump);
		return;
	}

	FVector MovePos = FVector::Zero;
	if (EngineInput::IsPress(VK_LEFT))
	{
		MovePos += FVector::Left * _DeltaTime * FreeMoveSpeed;
	}

	if (EngineInput::IsPress(VK_RIGHT))
	{
		MovePos += FVector::Right * _DeltaTime * FreeMoveSpeed;
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
		AddActorLocation(MovePos);
		//GetWorld()->AddCameraPos(MovePos);
	}
}


void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);



	StateUpdate(_DeltaTime);
}
