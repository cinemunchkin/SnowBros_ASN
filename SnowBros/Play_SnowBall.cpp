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

		SnowBallRenderer->SetTransform({{ 0,-11 }, { 82,64 }});

		SnowBallRenderer->CreateAnimation("Snowball_01", "Snowball_01_R.png", 0, 0, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_02", "Snowball_01_R.png", 1, 1, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_03", "Snowball_01_R.png", 2, 2, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball_04", "Snowball_01_R.png", 3, 3, 0.5f, true);
		SnowBallRenderer->CreateAnimation("Snowball", "Snowball_01_R.png", 4, 4, 0.5f, true);
		//한번 맞았을때 ~2~3~4~완전 눈사람

	}

	StateChange(ESnowballState::Snow1);

}


void APlay_SnowBall::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	/*
	BulletPhysics(_DeltaTime);
	AddActorLocation(Dir * _DeltaTime * 150.0f);
	*/
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
			SnowballStart();
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
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow2:
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow3:
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::Snow4:
		SnowballStack(_DeltaTime);
		break;
	
	case ESnowballState::SnowBall:
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

void APlay_SnowBall::SnowballStart()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball"));

}


void APlay_SnowBall::Snowball01_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_01"));

}

void APlay_SnowBall::Snowball02_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_02"));

}

void APlay_SnowBall::Snowball03_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_03"));

}

void APlay_SnowBall::Snowball04_Start()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball_04"));

}