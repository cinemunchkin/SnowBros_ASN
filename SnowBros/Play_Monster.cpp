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

	//	// 이런식으로 상대를 사용할수 있다.
	//	UCollision* Collision = Result[0];
	//	AActor* Ptr = Collision->GetOwner();
	//	APlay_Player* Player = dynamic_cast<APlay_Player*>(Ptr); //다운캐스트 한 꼴


	//	if (nullptr == Player)
	//	{
	//		MsgBoxAssert("터져야겠지....");
	//	}

	//	Destroy();
	//}


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

	AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);

	// 플레이어를 알아야 한다.
	// 
}


void APlay_Monster::DirCheck() //다시 설정 필요
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

	/*여기에 Jump를 추가하니까, '그런 이름의 애니메이션은 없다~'라고. */

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



