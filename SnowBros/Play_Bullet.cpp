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
		//�ϴ� ��������� ������ ����!
		BulletRenderer = CreateImageRenderer(SnowBrosRenderOrder::Bullet);
		BulletRenderer->SetImage("SnowBros_Bullet_R.png");
		BulletRenderer->SetImage("SnowBros_Bullet_L.png");
		BulletRenderer->SetImage("SnowBros_BulletCol_R.png");
		BulletRenderer->SetImage("SnowBros_BulletCol_L.png");

		BulletRenderer->SetTransform({ {16,-24}, {32*1.2f, 68*1.2f} });
		
		BulletRenderer->CreateAnimation("Bullet_Right","SnowBros_Bullet_R.png", 0, 1, 0.05f, true);
		BulletRenderer->CreateAnimation("Bullet_Left", "SnowBros_Bullet_L.png", 0, 1, 0.05f, true);

		BulletRenderer->CreateAnimation("BulletCol", "SnowBros_BulletCol_R.png", 0, 6, 0.03f, true);
			
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Bullet);
		BodyCollision->SetPosition(BulletRenderer->GetPosition()); // ��!! �ƴ�!! Bullet Position = Bullet Collision 
		BodyCollision->SetScale({ 25, 25 });
		BodyCollision->SetColType(ECollisionType::Rect);
	}

StateChange(EBulletState::Bullet);
}

void APlay_Bullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	//�̰͵� ����� �����°�.. �ؾ���.. ������

	AddActorLocation(Dir * _DeltaTime * 150.0f);// Dir�� Left�� 

	//BulletPhysics(_DeltaTime); //�� ����ٰ� ������ �ڲ�.. Col�̹����� �����Ǵ°���
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
{//�ִϸ��̼�
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
	IsBulletCol = true;

	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{
		BulletRenderer->SetTransform({ {16,-24}, {80 * 0.9f, 64 * 0.9f} });
		this->SetAnimation("BulletCol");
		//StateChange(EBulletState::BulletCol);
		if (BulletRenderer->IsCurAnimationEnd())
		{
		Destroy();
		} // ���� �ƹ� �ۿ���ϴµ�
		return;
	}

	IsBulletCol = false;
}


void APlay_Bullet::Bullet(float _DeltaTime)
{
	
}
void APlay_Bullet::BulletCol(float _DeltaTime)
{
	Destroy();
	
}


void APlay_Bullet::BulletStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));

}


void APlay_Bullet::BulletColStart()
{
	BulletRenderer->ChangeAnimation(GetAnimationName("BulletCol"));
}

