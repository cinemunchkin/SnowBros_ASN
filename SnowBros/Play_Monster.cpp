#include "Play_Monster.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include <EngineCore/SceneComponent.h>
APlay_Monster::APlay_Monster()
{
}

APlay_Monster::~APlay_Monster()
{
}

void APlay_Monster::BeginPlay()
{
	AActor::BeginPlay();
	

	{
		//UImageRenderer* MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);

		MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		MonsterRenderer->SetTransform({ {0,0}, {48, 48} });
		MonsterRenderer->SetImage("Monster_01_R.png");
		MonsterRenderer->SetImage("Monster_01_L.png");
	}
	
	{
		MonsterRenderer->CreateAnimation("Idle_Right", "Monster_01_R.png", 0, 5, 0.1f, true);
		MonsterRenderer->CreateAnimation("Idle_Left", "Monster_01_L.png", 0, 5, 0.1f, true);

		MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_01_R.png", 12, 23, 0.1f, true);
		MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_01_L.png", 12, 23, 0.1f, true);

		StateChange(EMonsterState::Idle);
	}



	
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	DirCheck();

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("�÷��̾ �������� �ʽ��ϴ�.");
	}
	
	{
		std::vector<UCollision*> Result;

		{
			BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
			BodyCollision->SetPosition(MonsterRenderer->GetPosition());
			BodyCollision->SetColType(ECollisionType::Rect);
			BodyCollision->SetScale({ 80, 80 });
		}

		if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
		{
			UCollision* Collision = Result[0];
			BodyCollision->SetActive(true, 0.5f);
		}


	}

	MoveCheck(_DeltaTime);
}





void APlay_Monster::DirCheck() //�ٽ� ���� �ʿ�
{
	EActorDir Dir = DirState;


	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		MonsterRenderer->ChangeAnimation(Name, true, MonsterRenderer->GetCurAnimationFrame(), MonsterRenderer->GetCurAnimationTime());
		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
		MonsterRenderer->ChangeAnimation(Name);
	}

	

}



std::string APlay_Monster::GetAnimationName(std::string _Name)
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



void APlay_Monster::StateChange(EMonsterState _State)
{//�ִϸ��̼�

	if (State != _State)
	{
		switch (_State)
		{
		case EMonsterState::Idle:
			IdleStart();
			break;
		case EMonsterState::Jump:
			JumpStart();
			break;
		case EMonsterState::DownJump:
			DownJumpStart();
			break;
		case EMonsterState::SnowBall:
			SnowballStart();
			break;
		case EMonsterState::Rolling:
			RollingStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}

void APlay_Monster::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EMonsterState::Idle:
		Idle(_DeltaTime);
		break;

	case EMonsterState::Jump:
		Jump(_DeltaTime);
		break;
	case EMonsterState::DownJump:
		DownJump(_DeltaTime);
		break;
	case EMonsterState::SnowBall:
		Snowball(_DeltaTime);
		break;
	case EMonsterState::Rolling:
		Rolling(_DeltaTime);
		break;
	}

}




void APlay_Monster::IdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
	
}


void APlay_Monster::JumpStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Jump"));
	DirCheck();
}


void APlay_Monster::DownJumpStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("DownJump"));
	DirCheck();
}


void APlay_Monster::SnowballStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	DirCheck();
}

void APlay_Monster::RollingStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Rolling"));
	DirCheck();
}


void APlay_Monster::MoveCheck(float _DeltaTime)
{
	DirCheck();


	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//���� �Ѿƴٴϴ� �Լ�
	FVector MonsterDir = PlayerPos - MonsterPos;

	MonsterDir.Y = 0.0f;
	Physics(_DeltaTime);
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	

	if (this->DirState == EActorDir::Right)
	{
		MonsterRenderer->ChangeAnimation("Idle_Right");
		AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);
		return;
	}
	if (this->DirState == EActorDir::Left)
	{
		MonsterRenderer->ChangeAnimation("Idle_Left");
		AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);
		return;
	}


}


void APlay_Monster::Idle(float _DeltaTime)
{
	DirCheck();
	


}

void APlay_Monster::Jump(float _DeltaTime)
{

}

void APlay_Monster::DownJump(float _DeltaTime)
{

}

void APlay_Monster::Snowball(float _DeltaTime)
{

}

void APlay_Monster::Rolling(float _DeltaTime)
{

}



void APlay_Monster::Physics(float _DeltaTime)
{// �߷��̶� �浹
	DirCheck();
	
	
	std::vector<UCollision*> ColResult;

	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, ColResult))
		//this(����)��  player �� �浹������
		// ���Ͷ� �÷��̾�� �ε�������
	{
		UCollision* Collision = ColResult[0];
		AActor* ColPtr = Collision->GetOwner();

		APlay_Player* Player = dynamic_cast<APlay_Player*>(ColPtr);
		FVector PlayerPos = Player->GetActorLocation(); // �÷��̾��� ���� ������
		FVector MonPos = GetActorLocation(); // ������ ���� ��ġ
		//�ٲٱ�
		if (PlayerPos.X = MonPos.X ) // 
			// �÷��̾�.x�� -10���� ũ�ų� 
		{
			Player->PlayerColState(EPlayState::Strobe);

			return;
		}
		
	}
	if(true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, ColResult))
	{

		UCollision* Collision = ColResult[0];
		AActor* ColPtr = Collision->GetOwner();

		APlay_Bullet* Bullet = dynamic_cast<APlay_Bullet*>(ColPtr);
		FVector BulletPos = Bullet->GetActorLocation(); // �÷��̾��� ���� ������
		FVector MonPos = GetActorLocation(); // ������ ���� ��ġ
		
		if (BulletPos.X = MonPos.X)
		{
			StateChange(EMonsterState::SnowBall);
			Bullet->BulletColState(EBulletState::BulletCol);


		}

	}

	MonsterMoveVector(_DeltaTime);
	MonsterLastMoveVector(_DeltaTime);
	MonsterGravityVector(_DeltaTime);
	

}

void APlay_Monster::MonsterMoveVector(float _DeltaTime)
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
	CheckPos.Y -= 10.0f;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);

	if (Color == Color8Bit(0, 255, 255, 0))
	{
		MoveVector = FVector::Zero; // �÷��� Cyan�̸�(���� �ϴ� ������), MoveVector �� 0, 0
	}


}


void APlay_Monster::MonsterGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // �߷°��ӵ��� ���� ������ ��ġ. \

	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), 
		GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// ���� ������ 0���� �ʱ�ȭ 
	}
}

void APlay_Monster::MonsterLastMoveVector(float _DeltaTime)
{
	TotalLastMoveVector = FVector::Zero;
	TotalLastMoveVector = TotalLastMoveVector + MoveVector;
	TotalLastMoveVector = TotalLastMoveVector + GravityVector;
	TotalLastMoveVector = TotalLastMoveVector + JumpVector;
}