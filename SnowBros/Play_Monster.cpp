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
		Renderer->SetTransform({ {0,0}, {32, 32} });
		Renderer->SetImage("boss_1.png");
	}

	{
		BodyCollision = CreateCollision(SnowBrosRenderOrder::Monster);
		BodyCollision->SetScale({ 32, 32 });
		BodyCollision->SetColType(ECollisionType::Circle);
	}
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosRenderOrder::Player, Result))
	{

		// 이런식으로 상대를 사용할수 있다.
		UCollision* Collision = Result[0];
		AActor* Ptr = Collision->GetOwner();
		APlay_Player* Player = dynamic_cast<APlay_Player*>(Ptr); //다운캐스트 한 꼴


		if (nullptr == Player)
		{
			MsgBoxAssert("터져야겠지....");
		}

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