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

	{
		UImageRenderer* Renderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		Renderer->SetTransform({ {0,0}, {32, 32} });
		Renderer->SetImage("Monster_01.png");
		Renderer->CreateAnimation("Idle", "Monster_01.png", 0,5, 0.5f, true);
		
	}


	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Monster);
		BodyCollision->SetScale({ 64, 32 });
		BodyCollision->SetColType(ECollisionType::Circle);
	}
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	{

		// �̷������� ��븦 ����Ҽ� �ִ�.
		UCollision* Collision = Result[0];
		AActor* Ptr = Collision->GetOwner();
		APlay_Player* Player = dynamic_cast<APlay_Player*>(Ptr); //�ٿ�ĳ��Ʈ �� ��


		if (nullptr == Player)
		{
			MsgBoxAssert("�����߰���....");
		}

		Destroy();
	}


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