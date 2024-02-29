#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>

#include <string_view>
#include <vector>

#include "SnowBros_Helper.h"
#include "Play_Bullet.h"
#include "Play_Player.h"


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

		BulletRenderer->SetTransform({ {16,-24}, {32*1.2f, 68*1.2f} });
		
		BulletRenderer->CreateAnimation("Bullet_Right","SnowBros_Bullet_R.png", 0, 1, 0.05f, true);
		BulletRenderer->CreateAnimation("Bullet_Left", "SnowBros_Bullet_L.png", 0, 1, 0.05f, true);

		BulletRenderer->CreateAnimation("BulletCol_Right", "SnowBros_BulletCol_R.png", 0, 6, 0.1f, true);
		BulletRenderer->CreateAnimation("BulletCol_Left", "SnowBros_BulletCol_L.png", 0, 6, 0.1f, true);
			
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
	//Bullet collisioncheck -> 여기서해보기
	//이것도 곡선으로 나가는거.. 해야함.. 포물선

	//BulletPhysics(_DeltaTime);
	AddActorLocation(Dir * _DeltaTime * 150.0f);
}



std::string APlay_Bullet::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	APlay_Player* Player = APlay_Player::GetMainPlayer();

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



void APlay_Bullet::StateChange(EBulletState _State)
{//애니메이션
	if (State != _State)
	{
		switch (_State)
		{
		case EBulletState::Bullet:
			BulletStart();
			break;

		case EBulletState::BulletCol:
			BulletColStart();
			break;

		default:
			break;
		}
	}

	State = _State;
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


void APlay_Bullet::BulletPhysics(float _DeltaTime)
{

	std::vector<UCollision*> BulletResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, BulletResult))
	{
		StateChange(EBulletState::BulletCol);
		return;
	}

}


void APlay_Bullet::Bullet(float _DeltaTime)
{
	
}
void APlay_Bullet::BulletCol(float _DeltaTime)
{
	
	
}


void APlay_Bullet::BulletStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));

}


void APlay_Bullet::BulletColStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("BulletCol"));
}

//{
//	if (/*충돌하면0 > GetPos().Y*/)
//	{
//		Destroy();
//		return;
//	}
//	else
//	{
//		/*
//		n초후 삭제
//		*/
//	}
//AddMoveVector;
//}


