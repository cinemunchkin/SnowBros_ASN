#include <EngineBase/Transform.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>

#include <string_view>
#include <vector>

#include "SnowBros_Helper.h"
#include "Play_SnowBall.h"
#include "Play_Player.h"




APlay_SnowBall::APlay_SnowBall()
{
}

APlay_SnowBall::~APlay_SnowBall()
{
}


void APlay_SnowBall::BeginPlay()
{
	ULevel::BeginPlay();
	{
		SnowBallRenderer = CreateImageRenderer(SnowBrosRenderOrder::Snowball);
		SnowBallRenderer->SetImage("Snowball_01_R.png");
		SnowBallRenderer->SetImage("Rolling_01_R.png");


		SnowBallRenderer->SetTransform({ { 0,-11 }, { 82,64 } });

		SnowBallRenderer->CreateAnimation("Snowball_1", "Snowball_01_R.png", 0, 0, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_2", "Snowball_01_R.png", 1, 1, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_3", "Snowball_01_R.png", 2, 2, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_4", "Snowball_01_R.png", 3, 3, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_5", "Snowball_01_R.png", 4, 4, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_6", "Snowball_01_R.png", 5, 5, 0.5f, true);//헐 이것도 추가해야함

		SnowBallRenderer->CreateAnimation("Rolling", "Rolling_01_R.png", 0, 3, 0.1f, true);
		//한번 맞았을때 ~2~3~4~완전 눈사람

	}


	{ // Collision은 사실 SnowBall_5에서 두어야하는데에..
		SnowCollision = CreateCollision(SnowBrosCollisionOrder::Snowball);
		SnowCollision->SetPosition(SnowBallRenderer->GetPosition());
		SnowCollision->SetColType(ECollisionType::Rect);
		SnowCollision->SetScale({ 80, 80 });
	}

	StateChange(ESnowballState::Snow1); // 초기값

}


void APlay_SnowBall::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	SnowballPhysics(_DeltaTime);
	SnowballMove(_DeltaTime);
	/*
	BulletPhysics(_DeltaTime);
	AddActorLocation(Dir * _DeltaTime * 150.0f);
	*/
}



void APlay_SnowBall::DirCheck()
{
	EActorDir Dir = SnowballDirState;
	APlay_Player* Player = APlay_Player::GetMainPlayer();

	FVector PlayerPos = Player->GetActorLocation();
	FVector SnowBallPos = GetActorLocation();


	if (PlayerPos.X > SnowBallPos.X)
	{
		Dir = EActorDir::Right;
		return;
	}
	if (PlayerPos.X < SnowBallPos.X)
	{
		Dir = EActorDir::Left;
		return;
	}

	if (Dir != SnowballDirState)
	{
		SnowballDirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		SnowBallRenderer->ChangeAnimation(Name, true, SnowBallRenderer->GetCurAnimationFrame(), SnowBallRenderer->GetCurAnimationTime());
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
		SnowBallRenderer->ChangeAnimation(Name);
	}



}


void APlay_SnowBall::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	SnowBallRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_SnowBall::GetAnimationFullName(std::string _Name)
{
	return _Name;

}


std::string APlay_SnowBall::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	APlay_Player* Player = APlay_Player::GetMainPlayer();

	switch (SnowballDirState)
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

void APlay_SnowBall::SnowballPhysics(float _DeltaTime)
{// Snowball5(눈덩이)가 되면, 플레이어와 충돌 가능하도록 
	IsSnowballComplete = false;
	
	std::vector<UCollision*> BulletResult;
	if (true == SnowCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, BulletResult))
	{//snowball이 bullet에 닿으면 ball5
		IsSnowballComplete = true;
		this->SetAnimation("Snowball_1");
		return;

	}
	

}



void APlay_SnowBall::SnowballMove(float _DeltaTime) 
// 플레이어가 눈덩이를 밀어내려면..?
// 일단 움직이게 해보자
{
	//std::vector<UCollision*> PlayerResult;
	//if (true == SnowCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	//{
	//	APlay_Player* Player = APlay_Player::GetMainPlayer();
	//	FVector PlayerPos = Player->GetActorLocation();
	//	FVector SnowballPos = GetActorLocation();

	//	if (PlayerPos.Y = SnowballPos.Y) // 둘이 같은 선상에 있고, 
	//	{
	//		if (PlayerPos.X + 15 > SnowballPos.X) // 플레이어가 더 오른쪽에 있으면, 
	//		
	//			AddMoveVector(FVector::Left * _DeltaTime);
	//		}
	//	}
	//	else
	//	{
	//		AddMoveVector(FVector::Right * _DeltaTime);
	//	}

	//}

}




void APlay_SnowBall::StateChange(ESnowballState _State)
{//애니메이션
	if (State != _State)
	{
		switch (_State)
		{
		case ESnowballState::None:
			break;

		case ESnowballState::Snow1:
			Snowball01_Start(); // 얘네를 다 하나로 묶어서 , 조건으로 돌려야겠다 현재 상태가 01 이면 02가 되도록
			break;
			
		case ESnowballState::Snow2:
			Snowball02_Start();
			break;
			
		case ESnowballState::Snow3:
			Snowball03_Start();
			break;
			
		case ESnowballState::Snow4:
			Snowball04_Start();
			break;
		
		case ESnowballState::SnowBall:
			Snowball05_Start();
			break;

		default:
			break;
		}
	}

	State = _State;
}


void APlay_SnowBall::SnowballStateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ESnowballState::None:
			break;

	case ESnowballState::Snow1:
		DirCheck();
		SnowballMove(_DeltaTime);
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow2:
		DirCheck();
		SnowballMove(_DeltaTime);
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow3:
		DirCheck();
		SnowballMove(_DeltaTime);
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow4:
		DirCheck();
		SnowballMove(_DeltaTime);
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::SnowBall:
		DirCheck();
		SnowballMove(_DeltaTime);
		SnowballStack(_DeltaTime);
		break;

	default:
		break;

	}
		
}


void APlay_SnowBall::SnowballStack(float _DeltaTime)
{
	//if (this -> State = ESnowballState::None);
	//// 현재 상태가 none 이면 -> snowball01로 
}


void APlay_SnowBall::Snowball01_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_1"));

}

void APlay_SnowBall::Snowball02_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_2"));

}

void APlay_SnowBall::Snowball03_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_3"));

}

void APlay_SnowBall::Snowball04_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_4"));

}


void APlay_SnowBall::Snowball05_Start()
{
	IsSnowballComplete = true;
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_5"));

}





void APlay_SnowBall::AddMoveVector(const FVector& _DirDelta)
{
	MoveVector = _DirDelta * MoveAcc;
}
