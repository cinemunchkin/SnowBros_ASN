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
		// 이런식으로 상대를 사용할수 있다.
		//UCollision* Collision = Result[0];
		//ATestPlayer* Player = dynamic_cast<ATestPlayer*>(Collision->GetOwner());
		Destroy();
	}


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	FVector MonsterDir = PlayerPos - MonsterPos;
	// MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	AddActorLocation(MonsterDirNormal * _DeltaTime * 300.0f);

	// 플레이어를 알아야 한다.
	// 
}