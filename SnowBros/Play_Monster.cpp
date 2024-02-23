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
		// 이런식으로 상대를 사용할수 있다.
		BodyCollision->SetActive(true, 0.5f);
		// BodyCollision = nullptr;
	}


	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, Result))
	{
		// 이런식으로 상대를 사용할수 있다.
		BodyCollision->SetActive(true, 0.1f);
		this->Destroy(1.0f);
		// BodyCollision = nullptr;
	}


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//몬스터 쫓아다니는 함수
	//FVector MonsterDir = PlayerPos - MonsterPos;
	//// MonsterDir.Y = 0.0f;
	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	//AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);
	///----


	// 플레이어를 알아야 한다.
}


void APlay_Monster::DirCheck() //다시 설정 필요
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
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
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



