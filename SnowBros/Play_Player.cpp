#include "Play_Player.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include "SnowBros_Helper.h"

//// �̵�����! ���ӵ� ����, ���� ����


void APlay_Player::AddMoveVector(const FVector& _DirDelta) // ���ӵ�
{
	MoveVector += _DirDelta * MoveAcc;
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
		MoveVector = FVector::Zero; // �÷��� Cyan�̸�(���� �ϴ� ������), MoveVector �� 0, 0
	}


	////////////////���ӵ� ��� �Լ�

	if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
	{
		// ����Ű �ƹ��͵� �ȴ����� ������, ���ӵ� ���� ����. 
		if (0.001 <= MoveVector.Size2D()) // ���ӵ� �����̶� ������, ���� ����
		{
			MoveVector += (-MoveVector.Normalize2DReturn()) * _DeltaTime * MoveAcc;
		}
		else 
		{ // ���� ����
			MoveVector = float4::Zero;
		}
	}

	if (MoveMaxSpeed <= MoveVector.Size2D()) // ���ӵ� ����Ʈ ����. ���ӵ��� �ִ�ġ�� ������
	{
		MoveVector = MoveVector.Normalize2DReturn() * MoveMaxSpeed; // ���ӵ� Normalize
	}
}

void APlay_Player::CalJumpVector(float _DeltaTime)
{

}

// �̰͵� ������ �ʿ�����Ű�ư��
void APlay_Player::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // �߷°��ӵ��� ���� ������ ��ġ. 
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// ���� ������ 0���� �ʱ�ȭ 
	}
}



// ���� ī�޶� Y������ ����������
void APlay_Player::CamMoveLastMoveVector(float _DeltaTime)
{
	// ī�޶�� x�����θ� �������� �ϴϱ�.
	//GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	//AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlay_Player::CalLastMoveVector(float _DeltaTime)
{
	// ���η� ���� �ʱ�ȭ ��Ų��.
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + MoveVector;
	LastMoveVector = LastMoveVector + JumpVector;
	LastMoveVector = LastMoveVector + GravityVector;
	LastMoveVector + JumpVector; // �� �� ������?
	// ���� ��ġ = x������ ������ ��ġ + ����+ �߷�

}


//�̰͵� ������ �ʿ� ����
void APlay_Player::GroundUp() // ����// �̵��� �ϰ� ������ ���� ���� ó���� ������ ����?
{
	while (true)
	{		// �̰� Magenta�� �־߰ٴ�. ������â �ܺ��̶� �÷��� ������. �ܺ��� ��������� Up Up �ݺ��ϵ�
		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
		if (Color == Color8Bit(255, 0, 255, 0))
		{
			AddActorLocation(FVector::Up);
		}
		else
		{
			break;
		}
	}
}

void APlay_Player::MoveUpdate(float _DeltaTime)
{
	CalMoveVector(_DeltaTime);
	CalGravityVector(_DeltaTime);
	CalLastMoveVector(_DeltaTime);
	//CamMoveLastMoveVector(_DeltaTime);
	//GroundUp();
	
}

////////////////////////////////

APlay_Player* APlay_Player::MainPlayer = nullptr;

APlay_Player* APlay_Player::GetMainPlayer()
{
	return MainPlayer;
}
















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
	//			/* APlay_Player* ATestPalyer:: MainPlayer, APlay_Player* = This */
	//			/*static APlay_Player* MainPlayer*/
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

//void APlay_Player::GravityCheck(float _DeltaTime) // GravityCheck����� MoveUpdate��
//{
//	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
//	if (Color != Color8Bit(0, 255, 255, 0))
//	{
//		AddActorLocation(FVector::Down * _DeltaTime * Gravity);
//	}
//}


void APlay_Player::GravityOff(float _DeltaTime)
{
	
		AddActorLocation(FVector::Down * _DeltaTime/* * Gravity*/);
	
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

/*���⿡ Jump�� �߰��ϴϱ�, '�׷� �̸��� �ִϸ��̼��� ����~'���. */

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		//Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationframe(), Renderer-> GetCurAnimationTime());
		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
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
	// �������¿� ���� ���°� ���� �ʾ�
	// �������� move ������ Idle
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
	case EPlayState::Run:
		Run(_DeltaTime);
		break;
	case EPlayState::Jump:
		Jump(_DeltaTime);
		break;
	case EPlayState::DownJump:
		Jump(_DeltaTime);
		break;
	default:
		break;
	}


}


void APlay_Player::Idle(float _DeltaTime)
{
	/* ���� �����ʵ� �ȵǰ� �ְ�.
	 ���⼭�� ����
	 ������ �������� ��� ���� �Ű澲�� �˴ϴ�.*/
	/*if (true == UEngineInput::IsDown('1'))
	{
		StateChange(EPlayState::FreeMove);
		return;
	}

	if (true == UEngineInput::IsDown('2'))
	{
		StateChange(EPlayState::CameraFreeMove);
		return;
	}
*/
//////////////////////////////////////////
	if (
		true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Run);
		return;
	}



	if (
		true == UEngineInput::IsDown('z')
		)
	{
		StateChange(EPlayState::Jump);
		return;
	}

	MoveUpdate(_DeltaTime);
}





void APlay_Player::Jump(float _DeltaTime)
{
	DirCheck();
	MoveUpdate(_DeltaTime);

	//Jump ���¿���, �����̽��ٳ� ����Ű ��� �ȴ��������� -> Idle�� ���ư���
	if (true == UEngineInput::IsFree(VK_SPACE)
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	FVector MovePos;
	if (UEngineInput::IsPress(VK_SPACE))
	{
		MovePos += FVector::Up * _DeltaTime * FreeMoveSpeed;
				
	}
	AddActorLocation(MovePos);
	//�ƴ� ����!!!


//JumpŰ ���� ����, �߷� üũ
	

	// �̰� �ȸԤ�����.. ���� �浹�ϴ°� ����
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
{// DownŰ + JumpŰ  = �Ʒ������� �������� => �� �������� �ٴ� �浹 �÷� �ٲٸ� �ǳ�!?

	DirCheck();

	//Jump ���¿���, �����̽��ٳ� ����Ű ��� �ȴ��������� -> Idle�� ���ư���
	if (true == UEngineInput::IsFree(VK_SPACE)
		&& UEngineInput::IsFree(VK_RIGHT)
		&& UEngineInput::IsFree(VK_LEFT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	FVector MovePos;

	if (UEngineInput::IsPress('a'))
	{
		
		return GravityOff(_DeltaTime);

	}



}




void APlay_Player::Run(float _DeltaTime)
{
	DirCheck();
		
(_DeltaTime);

	if (true == UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		StateChange(EPlayState::Idle);
		return;
	}


	if (true == UEngineInput::IsDown('z'))
	{// Move���¿��� JumpŰ ������ ���� status ����
		StateChange(EPlayState::Jump);
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
		AddActorLocation(MoveVector); /// �̰� �ߺ�!!
		//GetWorld()->AddCameraPos(MovePos);
	}

	MoveUpdate(_DeltaTime);
}




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
