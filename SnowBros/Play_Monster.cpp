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
		UImageRenderer* Renderer = CreateImageRenderer(SnowBrosRenderOrder::Player);
		Renderer->SetTransform({ {0,0}, {100, 100} });
		Renderer->SetImage("TestFolderAnimation");
	}

	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Monster);
		BodyCollision->SetScale({ 100, 100 });
	}
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	{
		// �̷������� ��븦 ����Ҽ� �ִ�.
		//UCollision* Collision = Result[0];
		//ATestPlayer* Player = dynamic_cast<ATestPlayer*>(Collision->GetOwner());
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

	AddActorLocation(MonsterDirNormal * _DeltaTime * 300.0f);

	// �÷��̾ �˾ƾ� �Ѵ�.
	// 
}