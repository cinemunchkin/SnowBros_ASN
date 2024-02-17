#include "Play_Monster.h"

APlay_Monster::APlay_Monster()
{
}

APlay_Monster::~APlay_Monster()
{
}

void APlay_Monster::BeginPlay()
{
	AActor::BeginPlay();
	//MainPlayer = this;

	{

		MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		//UImageRenderer* MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		MonsterRenderer->SetTransform({ {0,0}, {32, 32} });
		MonsterRenderer->SetImage("Monster_01.png");
		MonsterRenderer->CreateAnimation("Idle_Right", "Monster_01.png", 0,5, 0.1f, true);
		StateChange(EPlayState::Idle);
	}


	/*{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Monster);
		BodyCollision->SetScale({ 64, 32 });
		BodyCollision->SetColType(ECollisionType::Circle);
	}*/
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	//std::vector<UCollision*> Result;
	//if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	//{

	//	// �̷������� ��븦 ����Ҽ� �ִ�.
	//	UCollision* Collision = Result[0];
	//	AActor* Ptr = Collision->GetOwner();
	//	APlay_Player* Player = dynamic_cast<APlay_Player*>(Ptr); //�ٿ�ĳ��Ʈ �� ��


	//	if (nullptr == Player)
	//	{
	//		MsgBoxAssert("�����߰���....");
	//	}

	//	Destroy();
	//}


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("�÷��̾ �������� �ʽ��ϴ�.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	FVector MonsterDir = PlayerPos - MonsterPos;
	// MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);

	// �÷��̾ �˾ƾ� �Ѵ�.
	// 
}


void APlay_Monster::DirCheck() //�ٽ� ���� �ʿ�
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



