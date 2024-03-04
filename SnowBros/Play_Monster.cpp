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
		MonsterRenderer->SetTransform({ {0,-20}, {48, 48} });
		MonsterRenderer->SetImage("Monster_01_R.png");
		MonsterRenderer->SetImage("Monster_01_L.png");
	}

	{
		MonsterRenderer->CreateAnimation("Idle_Right", "Monster_01_R.png", 0, 5, 0.1f, true);
		MonsterRenderer->CreateAnimation("Idle_Left", "Monster_01_L.png", 0, 5, 0.1f, true);

		//눈 속에 갇히고 있을때 1~4 / 5일때는 destroy
		MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_01_R.png", 12, 23, 0.1f, true);
		MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_01_L.png", 12, 23, 0.1f, true);
		/*
		아대박 그러고 보니까 눈 속에 갇히고 있을때+눈덩이에서는, 몬스터 Physics 끄는 기능 만들기
		그때는 플레이어랑 충돌해도 그닥 갠차늠 strobe 없음
		*/
	}


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
		BodyCollision->SetPosition(MonsterRenderer->GetPosition());
		BodyCollision->SetColType(ECollisionType::Rect);
		BodyCollision->SetScale({ 80, 80 });
	}

	{
		SnowBallRenderer = CreateImageRenderer(SnowBrosRenderOrder::Snowball);
		SnowBallRenderer->SetImage("Snowball_01_R.png");
		SnowBallRenderer->SetImage("Rolling_01_R.png");
		SnowBallRenderer->SetTransform({ { 0,-32 }, { 84,66 } });



		//눈 렌더 //snowballstack
		SnowBallRenderer->CreateAnimation("SnowStack_Right", "Snowball_01_R.png", 0, 3, 0.5f, true);
		SnowBallRenderer->CreateAnimation("SnowStack_Left", "Snowball_01_R.png", 0, 3, 0.5f, true);

		//눈 렌더 //snowballrolling
		SnowBallRenderer->CreateAnimation("Rolling_Right", "Rolling_01_R.png", 0, 4, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Rolling_Left", "Rolling_01_R.png", 0, 4, 0.5f, true);

		//SnowBallRenderer->CreateAnimation("RollingFinal_Right",)

		//SnowBallRenderer->CreateAnimation("Snowball_1", "Snowball_01_R.png", 0, 0, 0.5f, true);
		//SnowBallRenderer->CreateAnimation("Snowball_2", "Snowball_01_R.png", 1, 1, 0.5f, true);
		//SnowBallRenderer->CreateAnimation("Snowball_3", "Snowball_01_R.png", 2, 2, 0.5f, true);
		//SnowBallRenderer->CreateAnimation("Snowball_4", "Snowball_01_R.png", 3, 3, 0.5f, true);
		//SnowBallRenderer->CreateAnimation("Snowball_5", "Snowball_01_R.png", 4, 4, 0.5f, true);
		//SnowBallRenderer->CreateAnimation("Snowball_6", "Snowball_01_R.png", 5, 5, 0.5f, true);//헐 이것도 추가해야함

		// SnowBallRenderer->CreateAnimation("Rolling", "Rolling_01_R.png", 0, 3, 0.1f, true);

		SnowBallRenderer->ActiveOff();// SnowBallRender는 처음에 Off해두고
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
	MonsterGroundUp(_DeltaTime);
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



void APlay_Monster::RollingStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	DirCheck();
}


void APlay_Monster::MoveCheck(float _DeltaTime)
{
	//DirCheck();
	MonsterDirVector(_DeltaTime);
	MonsterGravity(_DeltaTime);


	


	//몬스터 쫓아다니는 함수

	//FVector MonsterDir = PlayerPos - MonsterPos;
	//MonsterDir.Y = 0.0f;
	//FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();
	//AddActorLocation(MonsterDirNormal * _DeltaTime * 50.0f);
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

	//SetAnimation("Idle_Left");
	MoveCheck(_DeltaTime);
	SetAnimation("Idle_Right");
	

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	FVector MonsterDir = PlayerPos - MonsterPos;
	MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	AddActorLocation(FVector::Right * MonsterDirNormal*MoveAcc* _DeltaTime);

	/*switch (MonsterDirState)
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

	}*/
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

void APlay_Monster::SnowballStart()
{// 몬스터 -> snowballstart 하면, snowrender -> on
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	SnowBallRenderer->SetActive(true); // Begin할때는 off해두었다가 
	// 이렇게 하면 되는군나... 처음에 그냥 snowball을 액터로 만들어서-> 같은 포지션에 spawn함
	//	그냥 렌더 한번 더 얹는걸로 바뀜ㅁ


//SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack); -> 이건 없어도됨
	DirCheck();
}

void APlay_Monster::Snowball(float _DeltaTime)
{
	DirCheck();


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
	//여기에 걸어줘야하나..!
	//오오오ㅗ오됐다도샏ㅅ담쇠도됐다 //
	//애니메이션 인덱스 오버되는거 해결함
}





void APlay_Monster::Rolling(float _DeltaTime)
{
	SnowBallRenderer->ChangeAnimation(GetAnimationName("Rolling"));
	//ColMoveUpdate(_DeltaTime);
	// Rolling함수 실행하면.. 
	return;

}



void APlay_Monster::ColMoveUpdate(float _DeltaTime) // 몬스터가 snowball상태일 때, 플레이어가 밀 수 있음
{
	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector CurPlayerPos = Player->GetActorLocation();
	FVector CurMonsterPos = GetActorLocation();
	FVector PlayerSpeed = Player->PlayerRollingSpeed;

	switch (Player->DirState)
	{
	case EActorDir::Left:
	{
		FVector MonsterDir = CurMonsterPos - CurPlayerPos; /*+ CurMonsterPos*/
		MonsterDir.iX() == CurPlayerPos.iX();
		FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();
		AddActorLocation(MonsterDirNormal * _DeltaTime * PlayerSpeed);
	}
	break;
	case EActorDir::Right:
		
	{
		FVector MonsterDir = CurPlayerPos - CurMonsterPos; /*+ CurMonsterPos*/
		MonsterDir.iX() == CurPlayerPos.iX();
		FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();
		AddActorLocation(MonsterDirNormal * _DeltaTime * PlayerSpeed);
	}
	break;
	default:
		break;

	}





}



void APlay_Monster::MonsterColPhysics(float _DeltaTime)
{// 몬스터 충돌시 반응
	//DirCheck();

	std::vector<UCollision*> BulletResult;// 완전 반대로 적어놨네엥..
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, BulletResult))
	{
		APlay_Bullet* Bullet = (APlay_Bullet*)BulletResult[0]->GetOwner();
		Bullet->Destroy();
		++SnowStack;
		StateChange(EMonsterState::Snowball);
		//	return;

			/*int StackNum = 5;
			if (SnowStack >= StackNum)
			{
				SnowBallRenderer->ChangeAnimation(GetAnimationName("Snowball"));
				return;
			}*/
		return;
	}


	std::vector<UCollision*> PlayerResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{

		if (SnowStack = 5)
		{
			StateChange(EMonsterState::Rolling);

			return;
		}

	}
	// 




}

void APlay_Monster::AddMoveVector(const FVector& _DirDelta) // 가속도 -> 등속으로 바꿈
{
	MoveVector = _DirDelta * MoveAcc;
}


void APlay_Monster::MonsterGravity(float _DeltaTime)
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
		}
		else
		{
			break;
		}
	}
}



void APlay_Monster::MonsterDirVector(float _DeltaTime)
{
	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	FVector MonsterDir = PlayerPos-MonsterPos; 
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();  

	return;

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
