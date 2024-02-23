#include "Play_Monster.h"
#include "Play_Bullet.h"

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
		MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		//UImageRenderer* MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		MonsterRenderer->SetTransform({ {0,0}, {64, 64} });
		MonsterRenderer->SetImage("Monster_01.png");
		MonsterRenderer->CreateAnimation("Idle_Right", "Monster_01.png", 0,5, 0.05f, true);
		StateChange(EPlayState::Idle);
	}


	
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Monster);
		BodyCollision->SetScale({ 100, 100 });
		BodyCollision->SetColType(ECollisionType::Rect);
	}

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, Result))
	{
		// �̷������� ��븦 ����Ҽ� �ִ�.
		BodyCollision->SetActive(true, 0.5f);
		// BodyCollision = nullptr;
	}


	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, Result))
	{
		// �̷������� ��븦 ����Ҽ� �ִ�.
		BodyCollision->SetActive(true, 0.1f);
		this->Destroy(1.0f);
		// BodyCollision = nullptr;
	}


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("�÷��̾ �������� �ʽ��ϴ�.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//���� �Ѿƴٴϴ� �Լ�
	//FVector MonsterDir = PlayerPos - MonsterPos;
	//// MonsterDir.Y = 0.0f;
	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	//AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);
	///----


	// �÷��̾ �˾ƾ� �Ѵ�.
}


void APlay_Monster::DirCheck() //�ٽ� ���� �ʿ�
{
	EActorDir Dir = DirState;

/*	if (UEngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

	*/

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



void APlay_Monster::StateChange(EPlayState _State)
{

	if (State != _State)
	{
		switch (_State)
		{
		case EPlayState::Idle:
			IdleStart();
			break;

		default:
			break;
		}
	}

	State = _State;
}


void APlay_Monster::IdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
}



void APlay_Monster::Idle(float _DeltaTime)
{


}



