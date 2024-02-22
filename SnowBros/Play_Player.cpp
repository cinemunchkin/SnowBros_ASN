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



//// �̵�����! ���ӵ� ����, ���� ����


void APlay_Player::BeginPlay()
{
	AActor::BeginPlay();
	MainPlayer = this;  // �ƿ� �̰� �ּ�ó���س��� ��� �÷��̾� ������ ���� ..
	



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

/*���⿡ Jump�� �߰��ϴϱ�, '�׷� �̸��� �ִϸ��̼��� ����~'���. */

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer-> GetCurAnimationTime());
		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
		Renderer->ChangeAnimation(Name);
	}
}


void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	// �浹 ���� = 0.0 ���� �ʱ�ȭ ������ϴ±���
	

	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Player);
		BodyCollision->SetScale({ 100, 100 });
		BodyCollision->SetColType(ECollisionType::Rect);

	}


	//APlay_Player::Strobe(_DeltaTime);


	//std::vector<UCollision*> Result;
	//if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	//{
	//	// �̷������� ��븦 ����Ҽ� �ִ�.
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
		MsgBoxAssert("�÷��̾ �������� �ʽ��ϴ�.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	


	StateUpdate(_DeltaTime);
}




void APlay_Player::Strobe(float _StrobeTime)
{
	StrobeUpdate(_StrobeTime);

	//IsStrobeUpdate = false;
	float Strobetime = _StrobeTime;
	//if -> 0�ʺ��� ũ��, ���� �پ��� �׵��� ����
	//if (0.0f <= _StrobeTime)
	//{
	//	for (_StrobeTime; _StrobeTime == 0.0f; _StrobeTime--)
	//	{
	//		StrobeUpdate(_StrobeTime);
	//		//�����Ÿ��°��� 
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
{ // �ִϸ��̼ǿ�
	
	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle: // ������
			IdleStart();
			break;
		case EPlayState::Run: // �޸���
			RunStart();
			break;
		case EPlayState::Jump: //���� ����
			JumpStart();
			break;
		case EPlayState::DownJump: // �Ʒ��� ����
			DownJumpStart();
			break;
		case EPlayState::Attack: // ����
			AttackStart();
			break;
		case EPlayState::Strobe: // ���ݹ����� ��������
			StrobeStart();
			break;
		case EPlayState::FastRun: // ���Ǹ԰� �����޸���
			FastRunStart();
			break;
		
		case EPlayState::Fly: // Stage�̵��� �� ����
			FlyStart();
			break;
				
		default:
			break;
		}
	}

	State = _State;


}

void APlay_Player::StateUpdate(float _DeltaTime)
{ // �Լ���
	switch (State)
	{
	case EPlayState::CameraFreeMove: //�Ⱦ�
		//CameraFreeMove(_DeltaTime);
		break;
	case EPlayState::FreeMove: //�Ⱦ�
		//FreeMove(_DeltaTime);
		break;
	case EPlayState::Idle: // ������
		Idle(_DeltaTime);
		break;
	case EPlayState::Run: //�޸���
		Run(_DeltaTime);
		break;
	case EPlayState::FastRun: // ���Ǹ԰�����Դ޸���
		FastRun(_DeltaTime);
		break;
	case EPlayState::Jump: // ���� ����
		Jump(_DeltaTime);
		break;
	case EPlayState::DownJump: //�Ʒ��� ����
		DownJump(_DeltaTime);
		break;
	case EPlayState::Attack: // �⺻ ����
		Attack(_DeltaTime);
		break;
	case EPlayState::Strobe: // �浹�� ��������
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
	//Idle���¿���
	///����Ű�� ������ ��/�� �̵�
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayState::Run);
		return;
	}
	///����Shift�� ����
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;
	}
	///ZŰ�� ����
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
	

	
	//���� ����Ű �Ѵ� �ȴ������� ; Idle
	if (true == UEngineInput::IsFree(VK_LEFT) 
		&& true == UEngineInput::IsFree(VK_RIGHT)
		&& true == UEngineInput::IsFree('Z'))
	{
		StateChange(EPlayState::Idle);
		//MoveUpdate(_DeltaTime);
		return;
	}

	//�ٴ� ���ȿ� ����Ű ������ ����
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;		
	}

	//����Ű ������ �������� ���� - ���ӵ� �ʿ����
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
	if (Color != Color8Bit(0, 255, 255, 0)) // �÷��̾� x�� +-15, y�� -15�� cyan�� �ƴϸ�, ��� ��. cyan�̸� ����
	{
		AddActorLocation(MoveVector); /// �̰� �ߺ�!!
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
	// �� �̰� �̷��� �ٲٸ� ������.. �ٲٴ� ���� �𸣰ٴ�

	return;



	// APlay_Bullet::BulletFired���� ����� 


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
		// ���� �ʿ�
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
	// DownŰ + JumpŰ  = �Ʒ������� �������� => �� �������� �ٴ� �浹 �÷� �ٲٸ� �ǳ�!?

	DirCheck();

	//Jump ���¿���, �����̽��ٳ� ����Ű ��� �ȴ��������� -> Idle�� ���ư���
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


void APlay_Player::AddMoveVector(const FVector& _DirDelta) // ���ӵ� -> ������� �ٲ�
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
		MoveVector = FVector::Zero; // �÷��� Cyan�̸�(���� �ϴ� ������), MoveVector �� 0, 0
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
	GravityVector += GravityAcc * _DeltaTime; // �߷°��ӵ��� ���� ������ ��ġ. \

	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// ���� ������ 0���� �ʱ�ȭ 
	}
}



// ���� ī�޶� Y������ ����������? �ƴϸ� ī�޶�� ���ΰ�, �������� �̵��� ���� ��Ե� �����̴°ɷ�
void APlay_Player::CamMoveLastMoveVector(float _DeltaTime)
{
	// ī�޶�� x�����θ� �������� �ϴϱ�.
	//GetWorld()->AddCameraPos(MoveVector * _DeltaTime);
	AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlay_Player::CalLastMoveVector(float _DeltaTime)
{
	// ���η� ���� �ʱ�ȭ ��Ų��.
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + MoveVector;
	LastMoveVector = LastMoveVector + GravityVector;
	LastMoveVector = LastMoveVector + JumpVector;
	 // �� �� ������?
	// ���� ��ġ = x������ ������ ��ġ + ����+ �߷�

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


