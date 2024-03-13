#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>

#include <string_view>
#include <vector>

#include "SnowBros_Helper.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include "Play_Monster.h"


APlay_Bullet::APlay_Bullet()
{
}

APlay_Bullet::~APlay_Bullet()
{
}


void APlay_Bullet::BeginPlay()
{
	AActor::BeginPlay();
	{
		//일단 만들어지는 지부터 보자!
		BulletRenderer = CreateImageRenderer(SnowBrosRenderOrder::Bullet);
		BulletRenderer->SetImage("SnowBros_Bullet_R.png");
		BulletRenderer->SetImage("SnowBros_Bullet_L.png");
		BulletRenderer->SetImage("SnowBros_BulletCol_R.png");
		BulletRenderer->SetImage("SnowBros_BulletCol_L.png");

		BulletRenderer->SetTransform({ {16,-24}, {32 * 1.2f, 68 * 1.2f} });
		BulletRenderer->CreateAnimation("Bullet_Right", "SnowBros_Bullet_R.png", 0, 1, 0.05f, true);
		BulletRenderer->CreateAnimation("Bullet_Left", "SnowBros_Bullet_L.png", 0, 1, 0.05f, true);
		

		
		BulletRenderer->CreateAnimation("BulletBomb_Right", "SnowBros_BulletCol_R.png", 0, 6, 10.0f, true);
		BulletRenderer->CreateAnimation("BulletBomb_Left", "SnowBros_BulletCol_L.png", 0, 6, 10.0f, true);
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Bullet);
		BodyCollision->SetPosition(BulletRenderer->GetPosition()); // 오!! 됐다!! Bullet Position = Bullet Collision 
		BodyCollision->SetScale({ 25, 25 });
		BodyCollision->SetColType(ECollisionType::Rect);
	}

StateChange(EBulletState::Bullet);
}

void APlay_Bullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	//이것도 곡선으로 나가는거.. 해야함.. 포물선

	StateUpdate(_DeltaTime);
}


void APlay_Bullet::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EBulletState::Bullet:
		Bullet(_DeltaTime);
		break;

		case EBulletState::BulletBomb:
		BulletBomb(_DeltaTime);
		break;

	default:
		break;

	}

}


void APlay_Bullet::StateChange(EBulletState _State)
{//애니메이션 - 보통상태 / collision일때 
	if (State != _State)
	{
		switch (_State)
		{
		case EBulletState::Bullet:
			BulletStart();
			break;

		case EBulletState::BulletBomb:
			BulletBombStart();
			break;

		default:
			break;
		}
	}

	State = _State;
}




void APlay_Bullet::BulletStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));

}


void APlay_Bullet::BulletBombStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("BulletBomb"));
}



void APlay_Bullet::Bullet(float _DeltaTime)
{
	BulletMoveUpdate(_DeltaTime);
	BulletColCheck(_DeltaTime);
}

void APlay_Bullet::BulletBomb(float _DeltaTime)
{
	//BulletPhysics(_DeltaTime);
	if (BulletRenderer->IsCurAnimationEnd())
	{
		//Monster -> BulletColCheck
		Destroy();

	}
}



void APlay_Bullet::BulletMoveUpdate(float _DeltaTime)
{
	BulletGravityCheck(_DeltaTime);
	BulletLastMoveVector(_DeltaTime);
	AddActorLocation(Dir * _DeltaTime * 150.0f);
	//Gravity

}


void APlay_Bullet::BulletGravityCheck(float _DeltaTime)
{
	BulletGravityVector += BulletGravityAcc * _DeltaTime;
	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(),
		GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		BulletGravityVector = FVector::Zero;
		BulletMoveVector = FVector::Zero;
	}
}

void APlay_Bullet::BulletLastMoveVector(float _DeltaTime)
{
	TotalLastMoveVector = FVector::Zero;
	TotalLastMoveVector = TotalLastMoveVector + BulletMoveVector;
	TotalLastMoveVector = TotalLastMoveVector + BulletGravityVector;
}





std::string APlay_Bullet::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	//APlay_Player* Player = APlay_Player::GetMainPlayer();

	switch (BulletDirState)
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


void APlay_Bullet::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	BulletRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_Bullet::GetAnimationFullName(std::string _Name)
{
	return _Name;

}


bool APlay_Bullet::BulletColCheck(float _DeltaTime)
{//Monster랑 Bullet이랑 collision
	//문제 bullet 포물선 함수 추가하기 

	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{
		
		StateChange(EBulletState::BulletBomb);

		return IsBulletBomb = true;
	}

	return IsBulletBomb = false;
}



