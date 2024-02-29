#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/SceneComponent.h>

#include "Play_Monster.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include "Play_SnowBall.h"

#include <vector>

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

	}


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
		BodyCollision->SetPosition(MonsterRenderer->GetPosition());
		BodyCollision->SetColType(ECollisionType::Rect);
		BodyCollision->SetScale({ 80, 80 });
	}

		StateChange(EMonsterState::Idle);

}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	DirCheck();
	MoveCheck(_DeltaTime);

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}
	
	MonsterColPhysics(_DeltaTime);
	
	StateUpdate(_DeltaTime);
}





void APlay_Monster::DirCheck()
{
	EMonsterDir Dir = MonsterDirState;
	APlay_Player* Player = APlay_Player::GetMainPlayer();

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();


	if (PlayerPos.X > MonsterPos.X)
	{
		Dir = EMonsterDir::Right; 
		return;
	}
	if (PlayerPos.X < MonsterPos.X)
	{
		Dir = EMonsterDir::Left;
		return;
	}

	if (Dir != MonsterDirState)
	{
		MonsterDirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		MonsterRenderer->ChangeAnimation(Name, true, MonsterRenderer->GetCurAnimationFrame(), MonsterRenderer->GetCurAnimationTime());
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
		MonsterRenderer->ChangeAnimation(Name);
	}

	

}



std::string APlay_Monster::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		DirName = "_Left";
		break;
	case EMonsterDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;

}



void APlay_Monster::StateChange(EMonsterState _State)
{//애니메이션

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
		case EMonsterState::Snowball:
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
	case EMonsterState::Snowball:
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
	//DirCheck();
	MonsterGravity(_DeltaTime);
	

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();



	//몬스터 쫓아다니는 함수
	
	//FVector MonsterDir = PlayerPos - MonsterPos;
	//MonsterDir.Y = 0.0f;
	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	//if (this->MonsterDirState == EMonsterDir::Right) // 여기 작동을 안하는듯함..ㅠㅠbullet보고 수정필요 
	//{
	//	MonsterRenderer->ChangeAnimation("Idle_Right");
	//	AddActorLocation(MonsterDirNormal * _DeltaTime * 50.0f);
	//	return;
	//}
	//if (this->MonsterDirState == EMonsterDir::Left)
	//{
	//	MonsterRenderer->ChangeAnimation("Idle_Left");
	//	AddActorLocation(MonsterDirNormal * _DeltaTime * 50.0f);
	//	return;
	//}


}


void APlay_Monster::Idle(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	
	//FVector MonsterDir = PlayerPos - MonsterPos;
	MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	switch (MonsterDirState)
	{
	case EMonsterDir::None:
		break;
	case EMonsterDir::Left:
		this->MonsterDir = FVector::Left;
		SetAnimation("Idle_Left");
		AddActorLocation(MonsterDirNormal * _DeltaTime * 50.0f);
		break;
	case EMonsterDir::Right:
		this->MonsterDir = FVector::Right;
		SetAnimation("Idle_Right");
		AddActorLocation(MonsterDirNormal * _DeltaTime * 50.0f);
		break;

	}

	/*MonsterColPhysics(_DeltaTime);

	FVector MonsterPos = this->GetActorLocation();
	

	if (MonsterPos.X > 200.0f)
	{
		StateChange(EMonsterState::Snowball);
		return;
	}*/

}

void APlay_Monster::Jump(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);

}

void APlay_Monster::DownJump(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);

}

void APlay_Monster::Snowball(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);
	
	StackSnowball(_DeltaTime);
}

void APlay_Monster::StackSnowball(float _DeltaTime)
{
	APlay_SnowBall* Snowball = GetWorld()->SpawnActor<APlay_SnowBall>();
	Snowball->SetActorLocation(this->GetActorLocation());
	// 눈덩이 spawn 만들기 - APlay_Snowball 에서 만들고

	switch (State)
	{
	case EMonsterState::None:
		break;

	case EMonsterState::Snowball:
		Snowball->SetAnimation("Snowball_01"); // 여기서 해야하나!!!!!!!!!
		SnowballStackCheck(_DeltaTime);
		break;
	
	default:
	break;
	}
	return;


}

void APlay_Monster::SnowballStackCheck(float _DeltaTime)
{
	MonsterColPhysics(_DeltaTime);
	for(SnowStack;SnowStack <6; Snowstack++)
	

}


void APlay_Monster::Rolling(float _DeltaTime)
{

}



void APlay_Monster::MonsterColPhysics(float _DeltaTime)
{// 몬스터 충돌시 반응
	//DirCheck();

	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, MonsterResult))
	{

		StateChange(EMonsterState::Snowball);
		return;
	}
	
	
}

void APlay_Monster::MonsterGravity(float _DeltaTime)
{

	MonsterMoveVector(_DeltaTime);
	MonsterLastMoveVector(_DeltaTime);
	MonsterGravityVector(_DeltaTime);

}

void APlay_Monster::MonsterMoveVector(float _DeltaTime)
{

	FVector CheckPos = GetActorLocation();
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		CheckPos.X -= 30;
		break;
	case EMonsterDir::Right:
		CheckPos.X += 30;
		break;
	default:
		break;
	}
	CheckPos.Y -= 10.0f;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);

	if (Color == Color8Bit(0, 255, 255, 0))
	{
		MoveVector = FVector::Zero; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
	}


}


void APlay_Monster::MonsterGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // 중력가속도에 의해 움직인 위치. \

	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), 
		GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// 땅에 닿으면 0으로 초기화 
	}
}

void APlay_Monster::MonsterLastMoveVector(float _DeltaTime)
{
	TotalLastMoveVector = FVector::Zero;
	TotalLastMoveVector = TotalLastMoveVector + MoveVector;
	TotalLastMoveVector = TotalLastMoveVector + GravityVector;
	TotalLastMoveVector = TotalLastMoveVector + JumpVector;
}



void APlay_Monster::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	MonsterRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_Monster::GetAnimationFullName(std::string _Name)
{
	return _Name;

}
