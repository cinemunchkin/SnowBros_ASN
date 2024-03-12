#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/SceneComponent.h>
#include <iostream>
#include "Play_Item.h"
#include "Play_Monster.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include "Play_SnowBall.h"

#include <vector>
#include <string>


/*
Random
UEngineRandom
*/



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
		MonsterRenderer->SetTransform({ {0,-26}, {48 * 1.3f, 48 * 1.3f} });

		//몬스터 기본 애니메이션
		MonsterRenderer->SetImage("Monster_01_R.png");
		MonsterRenderer->SetImage("Monster_01_L.png");

		//몬스터 기타 애니메이션
		MonsterRenderer->SetImage("Monster_02_R.png");
		MonsterRenderer->SetImage("Monster_02_L.png");

		MonsterRenderer->SetImage("Monster_Item_01.png");

		MonsterRenderer->SetImage("SnowBomb_01.png");

	}



	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
		BodyCollision->SetPosition(MonsterRenderer->GetPosition());
		BodyCollision->SetColType(ECollisionType::Rect);
		BodyCollision->SetScale({ 80, 80 });
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Snowball);
		BodyCollision->SetPosition(MonsterRenderer->GetPosition());
		BodyCollision->SetColType(ECollisionType::Rect);
		//BodyCollision->SetScale({ 80, 80 });
	}

	{
		MonsterRenderer->CreateAnimation("MonIdle", "Monster_01_R.png", 6, 6, 0.1f, true);

		// 몬스터 기본 ; 이동
		MonsterRenderer->CreateAnimation("MonMove_Right", "Monster_01_R.png", 0, 5, 0.1f, true);
		MonsterRenderer->CreateAnimation("MonMove_Left", "Monster_01_L.png", 0, 5, 0.1f, true);

		//눈 속에 갇히고 있을때 아둥바둥
		MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_02_R.png", 8, 9, 0.07f, true);
		MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_02_L.png", 5, 6, 0.07f, true);
		//MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_01_R.png", 7, 9, 0.05f, true);
		//MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_01_L.png", 8, 10, 0.05f, true);


		//몬스터 점프
		MonsterRenderer->SetTransform({ {0,-26}, {48 * 1.3f, 48 * 1.3f} });
		MonsterRenderer->CreateAnimation("Jump_Left", "Monster_01_L.png", 10, 11, 0.5f, true);
		MonsterRenderer->CreateAnimation("Jump_Right", "Monster_01_R.png", 6, 7, 0.5f, true);



		// 몬스터가 스노우볼에 맞아서 날아갈때 
		MonsterRenderer->CreateAnimation("MonFlying_Right", "Monster_02_L.png", 15, 16, 1.0f, true);
		MonsterRenderer->CreateAnimation("MonFlying_Left", "Monster_02_L.png", 15, 16, 1.0f, true);

		//눈 렌더 //맨 아랫층 벽에 닿으면 터짐
		MonsterRenderer->CreateAnimation("SnowBomb_Right", "SnowBomb_01.png", 0, 3, 0.05f, true);
		MonsterRenderer->CreateAnimation("SnowBomb_Left", "SnowBomb_01.png", 0, 3, 0.05f, true);
	}

	

	{
		SnowBallRenderer = CreateImageRenderer(SnowBrosRenderOrder::Snowball);
		SnowBallRenderer->SetImage("Snowball_01_R.png");
		SnowBallRenderer->SetImage("Rolling_01_R.png");
		
		SnowBallRenderer->SetTransform({ { +6,-38 }, { 78 * 1.15f,66 * 1.15f } });



		//눈 렌더 //snowballstack
		SnowBallRenderer->CreateAnimation("SnowStack_Right", "Snowball_01_R.png", 0, 4, 0.5f, true);
		SnowBallRenderer->CreateAnimation("SnowStack_Left", "Snowball_01_R.png", 0, 4, 0.5f, true);

		//눈 렌더 //snowballrolling
		SnowBallRenderer->CreateAnimation("Rolling_Right", "Rolling_01_R.png", 0, 3, 0.1f, true);
		SnowBallRenderer->CreateAnimation("Rolling_Left", "Rolling_01_R.png", 0, 3, 0.1f, true);


		SnowBallRenderer->ActiveOff();// SnowBallRender는 처음에 Off해두고
	}


	StateChange(EMonsterState::MonMove);

}


void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	StateUpdate(_DeltaTime);
}





void APlay_Monster::DirCheck()
// 
{
	EMonsterDir Dir = MonsterDirState;
	PrevDir = Dir;
	APlay_Player* Player = APlay_Player::GetMainPlayer();

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();


	if (PlayerPos.X > MonsterPos.X)
	{
		Dir = EMonsterDir::Right;
		MonsterDirState = Dir;
		return;
	}
	else
	{
		Dir = EMonsterDir::Left;
		MonsterDirState = Dir;
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
		case EMonsterState::MonIdle:
			MonIdleStart();
			break;
		case EMonsterState::MonMove:
			MonMoveStart();
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
		case EMonsterState::MonFlying:
			MonFlyingStart();
			break;
		case EMonsterState::SnowBomb:
			SnowBombStart();
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
	case EMonsterState::MonIdle:
		MonIdle(_DeltaTime);
		break;
	case EMonsterState::MonMove:
		MonMove(_DeltaTime);
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
	case EMonsterState::MonFlying:
		MonFlying(_DeltaTime);
		break;
	case EMonsterState::SnowBomb:
		SnowBomb(_DeltaTime);
		break;


	default:
		break;
	}

}




void APlay_Monster::MonIdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonIdle"));
	DirCheck();

}

void APlay_Monster::MonMoveStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonMove"));
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


void APlay_Monster::RollingStart()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationName("Rolling"));
	DirCheck();
}


void APlay_Monster::MonFlyingStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonFlying"));
	DirCheck();

}

void APlay_Monster::SnowBombStart()
{

	int n = MonsterDir.X;
	int reverse = 0;
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		reverse = (n + 20) * -1;
		break;
	case EMonsterDir::Right:
		reverse = (n + 20) * +1;
		break;
	default:
		break;
	}

	MonsterRenderer->SetTransform({ {reverse,-28}, {48 * 1.7f, 48 * 1.7f} });
	MonsterRenderer->ChangeAnimation(GetAnimationName("SnowBomb"));
	DirCheck();
}





void APlay_Monster::SnowballStart()
{

	MonsterRenderer->SetTransform({ {0,-45}, {48 * 1.4f, 48 * 1.4f} });
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	SnowBallRenderer->SetActive(true); // Begin할때는 off해두었다가 
	DirCheck();
}

void APlay_Monster::MonIdle(float _DeltaTime)
{
	if (true == BulletColCheck(_DeltaTime))
	{
		return;
	}

}

void APlay_Monster::MonMove(float _DeltaTime)
{

	if (true == BulletColCheck(_DeltaTime))
	{
		StateChange(EMonsterState::Snowball);
		return;
	}

	DirCheck();
	ColMoveUpdate(_DeltaTime);
	//SetAnimation("Idle_Left");
	MoveCheck(_DeltaTime);
	//SetAnimation("Idle_Right");


	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//FVector MonsterDir = MonsterDIrState
	// MonsterDir -> case로 바꾸기
	MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	//	AddActorLocation(FVector::Right * MonsterDirNormal * MoveAcc * _DeltaTime);
		// 여기도 문제 있음 -> Idle에서 Right로 놔둔거 고치기

	if (MonsterDirState == EMonsterDir::Left)
	{
		SetAnimation("MonMove_Left");
		AddActorLocation(FVector::Left * _DeltaTime * MoveAcc);
		return;
	}
	if (MonsterDirState == EMonsterDir::Right)
	{
		SetAnimation("MonMove_Right");
		AddActorLocation(FVector::Right * _DeltaTime * MoveAcc);
		return;
	}

	else if (MonsterDirState == EMonsterDir::None)
	{
		SetAnimation("MonIdle");
		return;
	}


}



void APlay_Monster::MonFlying(float _DeltaTime)
{


	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);
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

void APlay_Monster::SnowBomb(float _DeltaTime)
{
	/*switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		MonsterDir.X -= 10;
		break;
	case EMonsterDir::Right:
		MonsterDir.X = 10;
		break;
	default:
		break;
	}*/

		SnowBallRenderer->ActiveOff();	
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		Destroy();
		return;	
	}
}






void APlay_Monster::Snowball(float _DeltaTime)
{
	if (true == BulletColCheck(_DeltaTime))
	{ //Snowball상태에서 BulletColCheck -> true면 그냥 그대로 return;
		return;
	}

	DirCheck();
	SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack); // SnowStack n번째
	int StackNum = 4;
	if (SnowStack < StackNum)
	{
		return;
	}
	else
	{
		SnowBallRenderer->SetImage("Snowball_01_R.png", 4);
		MonsterRenderer->SetTransform({ this->GetActorLocation(), {48 * 0.1f, 48 * 0.1f} });
		MonsterColPhysics(_DeltaTime);
	}
	return;
}



// 여긴 스노우볼의 Rolling 이고, Snowballrender->Rolling 이랑 전진이동 
void APlay_Monster::Rolling(float _DeltaTime)
// Rolling으로 들어와서, Snowball정지이미지
// 플레이어가  push상태면 굴릴 수 있도록
{
	DirCheck();
	MoveCheck(_DeltaTime); // 문제 여기서 Rolling에 들어가긴함 ? 그냥 Movecheck에서 굴려주는 느낌인데
}


void APlay_Monster::MoveCheck(float _DeltaTime)
{
	MonsterMoveUpdate(_DeltaTime);

}

void APlay_Monster::ColMoveUpdate(float _DeltaTime) // 몬스터가 snowball상태일 때, 플레이어가 밀 수 있음
{
	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector CurPlayerPos = Player->GetActorLocation();
	FVector CurMonsterPos = GetActorLocation();
	FVector PlayerSpeed = Player->PlayerRollingSpeed;

}


bool APlay_Monster::BulletColCheck(float _DeltaTime)
{
	std::vector<UCollision*> BulletResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, BulletResult))
	{
		APlay_Bullet* Bullet = (APlay_Bullet*)BulletResult[0]->GetOwner();
		Bullet->SetAnimation("BulletCol");
		++SnowStack;
		Bullet->Destroy();
		return true;
	}

	return false;
}

void APlay_Monster::MonsterColPhysics(float _DeltaTime)
{


	std::vector<UCollision*> PlayerResult;
	// 플레이어가 몬스터와 충돌했을때, 
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{

		APlay_Player* Player = APlay_Player::GetMainPlayer();
		if (EMonsterState::Snowball == this->GetState()
			&& EPlayState::PlayerPush == Player->GetState())
		{	//만약 몬스터 상태가 스노우볼이고, 플레이어가 Push 상태이면
			switch (Player->DirState)
			{
			case EActorDir::Left:
				MonsterDir = FVector::Left;

				break;

			case EActorDir::Right:
				MonsterDir = FVector::Right;

				break;

			default:
				break;

			}

			std::vector<UCollision*> MontoSnowballResult;
			std::vector<UCollision*> SnowBallResult;
			if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MontoSnowballResult))
			{
				if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Snowball, SnowBallResult))
				{
					SetAnimation("MonFlying_Right");
					return;
				}
			}

		}


	}
}


void APlay_Monster::AddMoveVector(const FVector& _DirDelta) // 가속도 -> 등속으로 바꿈
{
	MoveVector = _DirDelta * MoveAcc;
}


void APlay_Monster::MonsterMoveUpdate(float _DeltaTime)
{
	MonsterMoveVector(_DeltaTime);
	MonsterGravityVector(_DeltaTime);
	MonsterLastMoveVector(_DeltaTime);
	AddActorLocation(TotalLastMoveVector * _DeltaTime);
}


void APlay_Monster::MonsterGroundUp(float _DeltaTime)
{
	while (true)
	{
		FVector Location = GetActorLocation();
		Location.Y -= 1.0f;
		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(Location.iX(), Location.iY(), Color8Bit::CyanA);
		if (Color == Color8Bit(0, 255, 255, 0))
		{
			AddActorLocation(FVector::Up);
			//	this->StateChange(EMonsterState::Jump);
		}
		else
		{
			break;
		}
	}
}


void APlay_Monster::MonsterDeath(float _DeltaTime)
{ //문제
	//SnowBallRenderer->ActiveOff();

	Spawn_Item();
	Destroy();
}

void APlay_Monster::Spawn_Item()
{

	APlay_Item* Item = GetWorld()->SpawnActor<APlay_Item>();
	Item->SetName("Item");
	Item->SetActorLocation(this->GetActorLocation());

	Item->ItemMoveVector.X = MoveVector.X * -1.0f;

	return;
}



void APlay_Monster::MonsterMoveVector(float _DeltaTime)
{

	FVector CheckPos = GetActorLocation();
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		CheckPos.X -= 1;
		break;
	case EMonsterDir::Right:
		CheckPos.X += 1;
		break;
	default:
		break;
	}
	CheckPos.Y -= 32.0f;
	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	Color8Bit ColorMagenta = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (ColorCyan == Color8Bit(0, 255, 255, 0))
	{
		if (ColorMagenta == Color8Bit(255, 0, 255, 0))
		{
			// MonsterDeath(_DeltaTime); 
			// 아 여기가 아닌디 눈덩이가 구르고 나서 터지는게 아님..!
			StateChange(EMonsterState::SnowBomb);
		}
		MoveVector.X *= -1.0f; 
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
