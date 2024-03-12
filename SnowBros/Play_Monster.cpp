#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/SceneComponent.h>
#include <iostream>
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
		//문제 ;; 눈덩이 안에 있을때 destroy 해야됨!!!
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
	}

	{
		SnowBallRenderer = CreateImageRenderer(SnowBrosRenderOrder::Snowball);
		SnowBallRenderer->SetImage("Snowball_01_R.png");
		SnowBallRenderer->SetImage("Rolling_01_R.png");
		SnowBallRenderer->SetImage("SnowBomb_01.png");
		SnowBallRenderer->SetTransform({ { +6,-38 }, { 78 * 1.15f,66 * 1.15f } });



		//눈 렌더 //snowballstack
		SnowBallRenderer->CreateAnimation("SnowStack_Right", "Snowball_01_R.png", 0, 4, 0.5f, true);
		SnowBallRenderer->CreateAnimation("SnowStack_Left", "Snowball_01_R.png", 0, 4, 0.5f, true);

		//눈 렌더 //snowballrolling
		SnowBallRenderer->CreateAnimation("Rolling_Right", "Rolling_01_R.png", 0, 3, 0.1f, true);
		SnowBallRenderer->CreateAnimation("Rolling_Left", "Rolling_01_R.png", 0, 3, 0.1f, true);

		//눈 렌더 //맨 아랫층 벽에 닿으면 터짐
		SnowBallRenderer->CreateAnimation("SnowBomb", "SnowBomb_01.png", 0, 3, 1.5f, true);



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


	{
		SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack); // SnowStack n번째
		int StackNum = 5;
		if (SnowStack < StackNum)
		{
			return;
		}
		else
		{
			SnowBallRenderer->SetImage("Snowball_01_R.png", 4);
			MonsterColPhysics(_DeltaTime);
			//ColMoveUpdate(_DeltaTime);
		}
		return;
	}
	/*MonsterColPhysics(_DeltaTime);

	FVector MonsterPos = this->GetActorLocation();

	if (MonsterPos.X > 200.0f)
	{
		StateChange(EMonsterState::Snowball);
		return;
	}*/

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


void APlay_Monster::Snowball(float _DeltaTime)
{
	if (true == BulletColCheck(_DeltaTime))
	{
		return;
	}

	DirCheck();
	SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack); // SnowStack n번째
	int StackNum = 3;
	if (SnowStack < StackNum)
	{
		return;
	}
	else
	{
		SnowBallRenderer->SetImage("Snowball_01_R.png", 3);
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
	MoveCheck(_DeltaTime);
}



void APlay_Monster::SnowBallMoveVector(float _DeltaTime)
{
	MoveCheck(_DeltaTime);

	FVector CheckPos = GetActorLocation();
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		CheckPos.X += 20;
		break;
	case EMonsterDir::Right:
		CheckPos.X -= 20;
		break;
	default:
		break;
	}
	CheckPos.Y -= 10.0f;
	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	Color8Bit ColorYellow = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::YellowA);

	if (ColorYellow == Color8Bit(255, 255, 0, 0))
	{
		MoveVector = FVector::Zero; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
		//SnowBallRenderer->ChangeAnimation("SnowBomb");
		//Destroy();
		BallAngleReverse(_DeltaTime);

		Destroy(_DeltaTime);
		//이제 여기서, Snowball 터지는 애니메이션으로 ㄱㄱ 한다음에
		// 로직 ;컬러 magenta 닿으면 반대방향으로 바꾸고
		//    지금처럼 yellow닿으면 그 안에서 destroy 하기
	}
	else if (ColorCyan == Color8Bit(0, 255, 255, 0))
	{


		MoveVector = FVector::Zero; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
		//Destroy();
		return;
	}

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

	//switch (Player->DirState)
	//{
	//case EActorDir::Left: // 아니 이러면 플레이어가 방향 바꿀때마다.. 
	//						//몬스터가 고개돌려버림
	//{
	//	FVector MonsterDir = CurMonsterPos - CurPlayerPos; /*+ CurMonsterPos*/
	//	this->SetAnimation("MonMove_Left");
	//	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();
	//	//AddActorLocation(MonsterDirNormal * _DeltaTime * PlayerSpeed*0.1f);
	//}
	//break;
	//case EActorDir::Right:
	//{
	//	FVector MonsterDir = CurPlayerPos - CurMonsterPos; /*+ CurMonsterPos*/
	//	this->SetAnimation("MonMove_Right");
	//	//MonsterDir.iX() == CurPlayerPos.iX();
	//}
	//break;
	//default:
	//	break;

	//}
	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();
	//AddActorLocation(MonsterDirNormal * _DeltaTime * PlayerSpeed * 0.1f);


}



void APlay_Monster::BallAngleReverse(float _DeltaTime)
{ /*
  Snowball상태일 때 벽에 닿으면 AddActorLocation방향을 반대로 바꾸자
   ReverseMonsterDir = MonsterDir.X * -1
  */
	MoveCheck(_DeltaTime);
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		MonsterDir = FVector::Right;
		break;
	case EMonsterDir::Right:
		MonsterDir = FVector::Left;
		break;
	default:
		break;
	}
	return;
}


bool APlay_Monster::BulletColCheck(float _DeltaTime)
{
	std::vector<UCollision*> BulletResult;// 완전 반대로 적어놨네엥..
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
{// 몬스터 충돌시 반응
	//DirCheck();

	std::vector<UCollision*> BulletResult;// 완전 반대로 적어놨네엥..
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, BulletResult))
	{
		APlay_Bullet* Bullet = (APlay_Bullet*)BulletResult[0]->GetOwner();
		Bullet->SetAnimation("BulletCol");
		++SnowStack;
		StateChange(EMonsterState::Snowball);
		Bullet->Destroy();


		//Bullet->BulletPhysics(_DeltaTime);
		return;
	}


	std::vector<UCollision*> PlayerResult;
	// 플레이어가 몬스터와 충돌했을때, 
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{

		APlay_Player* Player = APlay_Player::GetMainPlayer();
		if (EMonsterState::Snowball == this->GetState()
			&& EPlayState::PlayerPush == Player->GetState())
		{	//만약 몬스터 상태가 스노우볼이고, 플레이어가 Push 상태이면
			//IsRolling();
			switch (Player->DirState)
			{
			case EActorDir::Left:
				MonsterDir = FVector::Left;
				//AddMoveVector(MonsterDir * _DeltaTime * 50.0f);

				break;

			case EActorDir::Right:
				MonsterDir = FVector::Right;
				//AddMoveVector(MonsterDir * _DeltaTime * 50.0f);

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
					//스노우볼 완성된 상태에서 몬스터(몬스터상태)와 충돌했을때
					// 몬스터와 몬스터가 충돌했을때 
						//	APlay_Monster* Monster = (APlay_Monster*)TestResult[1]->GetOwner();
					SetAnimation("MonFlying_Right");
					return;
				}
			}



			if (true == UEngineInput::IsDown('X')
				&& (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT)))
			{
				// 스노우볼 상태에서, 

				Player->DirCheck();
				//IsRolling() == true;
				// 여기 들어오면, IsRolling은 true로 보고, 
				switch (Player->DirState)
				{
				case EActorDir::Left:
					MonsterDir = FVector::Left; // 아 됐다 양쪽으로 굴리기
					this->Rolling(_DeltaTime); // Rolling으로 감!
					break;

				case EActorDir::Right:
					MonsterDir = FVector::Right;
					this->Rolling(_DeltaTime);
					break;

				default:
					break;

				}

				return;
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
	CheckPos.Y -= 32.0f;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);

	if (Color == Color8Bit(0, 255, 255, 0))
	{
		MoveVector.X *= -1.0f; // 컬러가 Cyan이면(땅에 일단 닿으면), MoveVector 는 0, 0
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
