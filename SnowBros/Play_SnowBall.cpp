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

		SnowBallRenderer->SetTransform({{ 0,0 }, { 64,64 }});

		SnowBallRenderer->CreateAnimation("Snowball", "Snowball_01_R.png", 4, 4, 0.05f, true);
		//일단 테스트로 이 이미지 돌려보자!

	}
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

		case ESnowballState::SnowBall:
			SnowballStart();
			break;

		default:
			break;
		}
	}

	State = _State;
}

void APlay_SnowBall::SnowballStart()
{
	SnowBallRenderer->ChangeAnimation(GetAnimationFullName("Snowball"));

}