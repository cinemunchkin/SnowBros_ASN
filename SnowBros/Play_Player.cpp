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



//// �̵�����! ���ӵ� ����, ���� ����


void APlay_Player::BeginPlay()
{
	AActor::BeginPlay();
	MainPlayer = this;  // �ƿ� �̰� �ּ�ó���س��� ��� �÷��̾� ������ ���� ..


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Player);
		BodyCollision->SetScale({ 32, 72 });
		BodyCollision->SetColType(ECollisionType::Rect);
		// �÷��̾� ����

		PushCheckCollision = CreateCollision(SnowBrosCollisionOrder::Player);
		PushCheckCollision->SetScale({ 40, 40 });
		PushCheckCollision->SetColType(ECollisionType::Rect);
		// ����캼 �� ���� �浹 collision
	}



	{
		Renderer = CreateImageRenderer(SnowBrosRenderOrder::Player);
		Renderer->SetImage("SnowBros_Run_R.png");
		Renderer->SetImage("SnowBros_Run_L.png");

		Renderer->SetImage("SnowBros_Jump_R.png");
		Renderer->SetImage("SnowBros_Jump_L.png");

		Renderer->SetImage("SnowBros_Melt.png");

		Renderer->SetImage("SnowBros_Attack_R.png");
		Renderer->SetImage("SnowBros_Attack_L.png");

		Renderer->SetImage("SnowBros_PlayerRolling_R.png");
		Renderer->SetImage("SnowBros_PlayerRolling_L.png");

		/*Renderer->SetImage("SnowBros_PlayerStuck_R.png");
		Renderer->SetImage("SnowBros_PlayerStuck_L.png");
		*/



		Renderer->SetTransform({ {0,0}, {64 * 1.1f, 128 * 1.1f} });
		Renderer->CreateAnimation("Idle_Right", "SnowBros_Idle_R.png", 0, 0, 1.0f, true);
		Renderer->CreateAnimation("Idle_Left", "SnowBros_Idle_L.png", 0, 0, 1.0f, true);

		Renderer->CreateAnimation("Run_Right", "SnowBros_Run_R.png", 0, 3, 0.1f, true);
		Renderer->CreateAnimation("Run_Left", "SnowBros_Run_L.png", 0, 3, 0.1f, true);

		Renderer->CreateAnimation("Jump_Right", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);
		Renderer->CreateAnimation("Jump_Left", "SnowBros_Jump_R.png", 0, 6, 0.05f, true);

		Renderer->CreateAnimation("DownJump_Left", "SnowBros_Jump_L.png", 0, 0, 0.1f, true);
		Renderer->CreateAnimation("DownJump_Right", "SnowBros_Jump_R.png", 0, 0, 0.1f, true);

		Renderer->CreateAnimation("Attack_Right", "SnowBros_Attack_R.png", 0, 3, 0.02f, true);
		Renderer->CreateAnimation("Attack_Left", "SnowBros_Attack_L.png", 0, 3, 0.02f, true);

		Renderer->CreateAnimation("PlayerPush_Left", "SnowBros_PlayerRolling_L.png", 0, 0, 10.0f, true);
		Renderer->CreateAnimation("PlayerPush_Right", "SnowBros_PlayerRolling_R.png", 0, 0, 10.0f, true);

		Renderer->CreateAnimation("PlayerRolling_Left", "SnowBros_PlayerRolling_L.png", 0, 3, 5.0f, true);
		Renderer->CreateAnimation("PlayerRolling_Right", "SnowBros_PlayerRolling_R.png", 0, 3, 5.0f, true);

		Renderer->CreateAnimation("PlayerStuck_Left", "SnowBros_Melt.png", 0, 0, 5.0f, true);
		Renderer->CreateAnimation("PlayerStuck_Right", "SnowBros_Melt.png", 0, 0, 5.0f, true);



		StateChange(EPlayState::Idle);
	}


	/*


	*/
}



void APlay_Player::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	// PlayerColPhysics(_DeltaTime);
	// �̰� ���⿡ �־ �ڲ� �̴±��� �̰� �ּ��Ŵϱ� �ٷ� �ߵ�
	// -> �ذ�Ȱ� ; ����캼 1���� �а�, ����/�÷��̾� ���������� �ȿ�����

	StateUpdate(_DeltaTime);
}



void APlay_Player::DirCheck()
{
	EActorDir Dir = DirState;

	if (UEngineInput::IsDown(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if(UEngineInput::IsDown(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

	/*���⿡ Jump�� �߰��ϴϱ�, '�׷� �̸��� �ִϸ��̼��� ����~'���. */

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer->GetCurAnimationTime());
		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
		Renderer->ChangeAnimation(Name);
	}

	switch (DirState)
	{
	case EActorDir::Left:
		PushCheckCollision->SetPosition(FVector::Left * 10.0f);
		break;
	case EActorDir::Right:
		PushCheckCollision->SetPosition(FVector::Right * 10.0f);
		break;
	default:
		break;
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

		case EPlayState::FastRun: // ���Ǹ԰� �����޸���
			FastRunStart();
			break;

		case EPlayState::Fly: // Stage�̵��� �� ����
			FlyStart();
			break;

		case EPlayState::PlayerPush: // Stage�̵��� �� ����
			PlayerPushStart();
			break;

		case EPlayState::PlayerRolling:
			PlayerRollingStart();
			break;
		case EPlayState::PlayerStuck:
			PlayerStuckStart();
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
	case EPlayState::PlayerPush: // ������ �б⸸ �ϱ�
		PlayerPush(_DeltaTime);
		break;
	case EPlayState::PlayerRolling: // ������ ������
		PlayerRolling(_DeltaTime);
		break;
	case EPlayState::PlayerStuck: // ������ �б⸸ �ϱ�
		PlayerStuck(_DeltaTime);
		break;
	case EPlayState::Fly: // Stage�̵��� �� ����
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
		case EPlayState::Strobe: // ���ݹ����� ��������
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


void APlay_Player::PlayerRollingStart()
{
	Renderer->ChangeAnimation(GetAnimationName("PlayerRolling"));

	DirCheck();
}
void APlay_Player::PlayerPushStart()
{
	Renderer->ChangeAnimation(GetAnimationName("PlayerPush"));
	DirCheck();
}

void APlay_Player::PlayerStuckStart()
{
	Renderer->ChangeAnimation(GetAnimationName("PlayerPush"));

	DirCheck();
}


void APlay_Player::PlayerStuck(float _DeltaTime)
{

	DirCheck();
	PlayerColPhysics(_DeltaTime);
	MoveUpdate(_DeltaTime);

	{
		std::vector<UCollision*> MonsterResult;
		APlay_Monster* Monster = (APlay_Monster*)MonsterResult[0]->GetOwner();
		
		Renderer->SetPosition(Monster->SnowBallRenderer->GetPosition());
	}

}
void APlay_Player::PlayerRolling(float _DeltaTime)
{

	DirCheck();
	PlayerColPhysics(_DeltaTime);
	MoveUpdate(_DeltaTime);



	if (true == UEngineInput::IsFree(VK_LEFT)
		&& true == UEngineInput::IsFree(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Idle);
		//MoveUpdate(_DeltaTime);
		return;
	}
	//----------------------------------

	MoveUpdate(_DeltaTime);
	FVector CheckPos = GetActorLocation();

	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 35;
		break;
	case EActorDir::Right:
		CheckPos.X += 35;
		break;
	default:
		break;
	}
	CheckPos.Y -= 10;
	std::vector<UCollision*> SnowballResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Snowball, SnowballResult))
	{
		//������ ���� �� �ƹ��͵� ���ϳ� �÷��̾ ����캼�ϰ� �浹������
	}

	return;

}

//
void APlay_Player::PlayerPush(float _DeltaTime)
{//���� ���� Rolling�̶�  push�� ������
	
	DirCheck();

	if (true == UEngineInput::IsFree(VK_LEFT)
		&& true == UEngineInput::IsFree(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsDown('X') && true == RollingCheck())
	{
		StateChange(EPlayState::Idle);
		return;
	}

	MoveUpdate(_DeltaTime);
	return;
}



void APlay_Player::Idle(float _DeltaTime)
{
	//MoveVector = FVector::Zero;
	DirCheck();
	Renderer->SetAlpha(1.0f);
	//PlayerColPhysics(_DeltaTime);
	StrobeColCheck(_DeltaTime);
	MoveUpdate(_DeltaTime);

	//Idle���¿���
	///����Ű�� ������ ��/�� �̵�
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayState::Run);
		return;
	}
	///����Shift�� ����
	if (true == (UEngineInput::IsDown('Z')&&UEngineInput::IsPress(VK_DOWN)))
	{
		StateChange(EPlayState::DownJump);
		return;
	}

	
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayState::Jump);
		return;
	}

	/// ����
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayState::Attack);
		return;
	}
}

bool APlay_Player::DamageCheck()
{
	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{//�÷��̾ ���Ͷ� �浹������, 

		AActor* Owner = MonsterResult[0]->GetOwner();
		//���Ϳ� �÷��̾ �浹�� ��� = MonsterResult
		// ù��°= [0]
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

		if (nullptr == Monster) // ����� üũ; ���Ͱ� ���࿡ nullptr�� ���!
		{
			MsgBoxAssert("���Ͱ� �ƴմϴ�");
		}

		if (Monster->GetState() == EMonsterState::Snowball)
		{
			return false;
		}

		return true;
		// �����Դ´ٴ� ���⼭ // strobe ����� �ű��
	}

	return false;

}

bool APlay_Player::PushCheck()
{
	FVector NextCheckPos = FVector::Zero;
	std::vector<UCollision*> MonsterResult;
	if (true == PushCheckCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{//�÷��̾ ���Ͷ� �浹������, 

		AActor* Owner = MonsterResult[0]->GetOwner();
		//���Ϳ� �÷��̾ �浹�� ��� = MonsterResult
		// ù��°= [0]
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

		if (nullptr == Monster) // ����� üũ; ���Ͱ� ���࿡ nullptr�� ���!
		{
			MsgBoxAssert("���Ͱ� �ƴմϴ�");
		}

		if (false == Monster->IsRolling())
		{
			return false;
		}


		StateChange(EPlayState::PlayerPush);

		return true;
		// �����Դ´ٴ� ���⼭
	}

	return false;

}

bool APlay_Player::RollingCheck()
{
	FVector NextCheckPos = FVector::Zero;
	std::vector<UCollision*> MonsterResult;
	if (true == PushCheckCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{//�÷��̾ ���Ͷ� �浹������, 

		AActor* Owner = MonsterResult[0]->GetOwner();
		//���Ϳ� �÷��̾ �浹�� ��� = MonsterResult
		// ù��°= [0]
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

		if (nullptr == Monster) // ����� üũ; ���Ͱ� ���࿡ nullptr�� ���!
		{
			MsgBoxAssert("���Ͱ� �ƴմϴ�");
		}

		if (false == Monster->IsRolling())
		{
			return false;
		}


		Monster->StateChange(EMonsterState::Rolling);
		switch (DirState) // ���⼭ ���͸� �о��ִ±���
		{
		case EActorDir::Left:
			Monster->MonsterDir = FVector::Left;
			Monster->SetMoveVector(FVector::Left * 500.0f);
			break;
		case EActorDir::Right:
			Monster->MonsterDir = FVector::Right;
			Monster->SetMoveVector(FVector::Right * 500.0f);
			break;
		default:
			break;
		}

		return true;
	}

	return false;
}


void APlay_Player::Run(float _DeltaTime)
{
	DirCheck();

	if (true == PushCheck())
	{
		StateChange(EPlayState::PlayerPush);
		return;
	}

	//���� ����Ű �Ѵ� �ȴ������� ; Idle
	if (true == UEngineInput::IsFree(VK_LEFT)
		&& true == UEngineInput::IsFree(VK_RIGHT)
		)
	{
		StateChange(EPlayState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime);
		if (true == UEngineInput::IsDown('Z'))
		{
			JumpVector += FVector::Right * _DeltaTime;
		}

	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime);
		if (true == UEngineInput::IsDown('Z'))
		{
			JumpVector += FVector::Right * _DeltaTime;
		}
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
	CheckPos.Y -= 32;
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


void APlay_Player::Strobe(float _DeltaTime)
{
	AlphaTime += _DeltaTime;

	if (0.1f <= AlphaTime)
	{
		Dir = !Dir;
		AlphaTime = 0.0f;
	}
	if (true == Dir)
	{
		Renderer->SetAlpha(0.5f - AlphaTime);
		//Renderer->SetAlpha(AlphaTime);
	}
	else
	{
		//Renderer->SetAlpha(0.5f - AlphaTime);
	}
	if (AlphaTime > 1.0f)
	{
		StateChange(EPlayState::Idle);
		//return;
	}
}


void APlay_Player::StrobeColCheck(float _DeltaTime)
{ //���� �̰͵� bool�� �ٲ���??

	DirCheck();
	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{//�÷��̾ ���Ͷ� �浹������, 
		AActor* Owner = MonsterResult[0]->GetOwner();
		//���Ϳ� �÷��̾ �浹�� ��� = MonsterResult
		// ù��°= [0]
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

		if (nullptr == Monster) // ����� üũ; ���Ͱ� ���࿡ nullptr�� ���!
		{
			MsgBoxAssert("���Ͱ� �ƴմϴ�");
		}

		if (EMonsterState::Snowball != Monster->GetState())
			// ���Ͱ� snowball state�� �ƴҶ���, �浹�ϸ� strobe
		{
			true == Dir;
			Strobe(_DeltaTime);
			return;
		}

		else if (EMonsterState::Snowball == Monster->GetState())
		{// ���Ͱ� Snowball ������ ��, 
			return;
		}
		return;
	}

}



void APlay_Player::Attack(float _DeltaTime)
{
	DirCheck();
	if (Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayState::Idle);
		return;
	}
	MoveUpdate(_DeltaTime);

}



void APlay_Player::Fire_Bullet()
{// bullet������ tick���� �ع�����, �÷��̾ ���� ��ȯ �� ������ �̤� 
// ������ƼŬ ���⵵ ���� ��ȯ�Ǿ���� �Ф�


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
		//Bullet�� �ִϸ��̼��� �����ִ� �Լ� �ʿ�
		//==>>>G���㷯���Ϸ��������¤��ٵƴ�!!!! �������� ������� �ִϸ��̼�!!!!!!! �ФФФФФФФ�
		// �浹�ϸ� -> ��ƼŬ ��� �ٲ�°�.. ���⼭ �ؾ��� �� ������.. �� �ȵɱ�..

		break;

	case EActorDir::Right:
		Bullet->Dir = FVector::Right;
		Bullet->SetAnimation("Bullet_Right");

		break;

	default:
		break;
	}





	return;
}




void APlay_Player::Jump(float _DeltaTime)
{
	DirCheck();
	//PlayerColPhysics(_DeltaTime);



	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		
		AddMoveVector(FVector::Left * _DeltaTime);
		MoveUpdate(_DeltaTime);

	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime);
		MoveUpdate(_DeltaTime);
	}


	MoveUpdate(_DeltaTime);


	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		//if (true == UEngineInput::IsDown('Z'))

		GroundUp(_DeltaTime); // ���� �����°� �ذ�!!
		JumpVector = FVector::Zero;
		StateChange(EPlayState::Idle);
		return;

	}
	//	MoveUpdate(_DeltaTime);

}



void APlay_Player::DownJump(float _DeltaTime)
{

	DirCheck();

	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * _DeltaTime);
	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * _DeltaTime);
	}
	
	//GravityVector += GravityAcc * _DeltaTime; // �߷°��ӵ��� ���� ������ ��ġ. \

	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	Color8Bit ColorYellow = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::YellowA);
	if (ColorCyan == Color8Bit(0, 255, 255, 0) || ColorYellow == Color8Bit(255, 255,0 , 0))
	{
		if (this->GetActorLocation().Y < 448) // ������ ���� �Ʒ��� �������� �ʰ� ���ƹ���
		{
			AddActorLocation(FVector::Down * _DeltaTime * 150.0f);
			MoveUpdate(_DeltaTime);
			return;
		}
	}

	StateChange(EPlayState::Idle);
	return;

}




void APlay_Player::Fly(float _DeltaTime)
{
}



//�÷��̾� �浹�� 
void APlay_Player::PlayerColPhysics(float _DeltaTime)
{
	DirCheck();
	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{//�÷��̾ ���Ͷ� �浹������, 

		AActor* Owner = MonsterResult[0]->GetOwner();
		//���Ϳ� �÷��̾ �浹�� ��� = MonsterResult
		// ù��°= [0]
		APlay_Monster* Monster = dynamic_cast<APlay_Monster*>(Owner);

					if (nullptr == Monster) // ����� üũ; ���Ͱ� ���࿡ nullptr�� ���!
					{
						MsgBoxAssert("���Ͱ� �ƴմϴ�");
					}

		//if (EMonsterState::Snowball != Monster->GetState()) ==============>StrobeColcheck�� ���� �и��ع���!!
		//	// ���Ͱ� snowball state�� �ƴҶ���, �浹�ϸ� strobe
		//{
		//	Strobe(_DeltaTime);
		//	return;
		//}

		if (EMonsterState::Rolling == Monster->GetState())
		{
			this->StateChange(EPlayState::PlayerStuck); // ==============> �÷��̾� �����̿� ������ ���̤�
		}

		else if (EMonsterState::Snowball == Monster->GetState())
		{// ���Ͱ� Snowball ������ ��, 
			bool MonsterRolling = Monster->IsRolling();
			// ���� bool= IsRolling �޴� �Լ�

			// �̰� ���ͷ� �Ű�����.

			if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
			{// ���Ͱ� snowball������ �� ����Ű�� �и�
				APlay_Player* Player = APlay_Player::GetMainPlayer(); 
				this->StateChange(EPlayState::PlayerPush);
				
				switch (DirState)
				{
				case EActorDir::Left:
					PlayerDir = FVector::Left;
					break;
				
				case EActorDir::Right:
					PlayerDir = FVector::Right;
					break;

				default:
					break;

				}
				//Monster->ColMoveUpdate(_DeltaTime);
				return;
				MonsterRolling = false;
			}
			else if (true == UEngineInput::IsDown('X') &&
				true == (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT)))
				//// ���Ͱ� snowball������ �� ����Ű�� �и�
			{
				Monster->StateChange(EMonsterState::Rolling);
			}
			else 
			{
				MonsterRolling = false;
				this->StateChange(EPlayState::Idle);
				Monster->ColMoveUpdate(_DeltaTime);
				//Monster->StateChange(EMonsterState::Snowball);
			}
			return;
		}




		return;
	}



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


void APlay_Player::GroundUp(float _DeltaTime)
{
	while (true)
	{
		FVector Location = GetActorLocation();
		Location.Y -= 0.5f;
		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(Location.iX(), Location.iY(), Color8Bit::CyanA);
		if (Color == Color8Bit(0, 255, 255, 0))
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
	CamMoveLastMoveVector(_DeltaTime);
	//GroundUp(_DeltaTime);

}

////////////////////////////////

APlay_Player* APlay_Player::MainPlayer = nullptr;

APlay_Player* APlay_Player::GetMainPlayer()
{
	return MainPlayer;
}


