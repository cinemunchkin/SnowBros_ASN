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

		BulletRenderer->SetTransform({ {16,-26}, {32 * 1.4f, 32 * 1.4f} });
		BulletRenderer->CreateAnimation("Bullet_Right", "SnowBros_Bullet_R.png", 0, 1, 0.05f, true);
		BulletRenderer->CreateAnimation("Bullet_Left", "SnowBros_Bullet_L.png", 0, 1, 0.05f, true);
		

		
		BulletRenderer->CreateAnimation("BulletBomb_Right", "SnowBros_BulletCol_R.png", 3, 5, 0.05f, true);
		BulletRenderer->CreateAnimation("BulletBomb_Left", "SnowBros_BulletCol_R.png", 3, 5, 0.05f, true);
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Bullet);
		BodyCollision->SetPosition(BulletRenderer->GetPosition()); // ��!! �ƴ�!! Bullet Position = Bullet Collision 
		BodyCollision->SetScale({ 25*0.5f, 32*0.5f });
		BodyCollision->SetColType(ECollisionType::Rect);
	}

StateChange(EBulletState::Bullet);
}

void APlay_Bullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	//�̰͵� ����� �����°�.. �ؾ���.. ������

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
{//�ִϸ��̼� - ������� / collision�϶� 
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
	//BulletMoveUpdate(_DeltaTime);
	BulletColCheck(_DeltaTime);
	BulletColorCheck(_DeltaTime);



	BulletGravityCheck(_DeltaTime);
	FVector BulletSpeed = Dir * 300.f;

	BulletTotal = BulletSpeed + BulletGravityVector;
	ReverseDir = -Dir * _DeltaTime;
	BulletDirSpeed = Dir + (ReverseDir);

	AddActorLocation((BulletTotal+BulletDirSpeed)*_DeltaTime);


}

void APlay_Bullet::BulletBomb(float _DeltaTime)
{
	BulletRenderer->SetTransform({ {16,-26}, {32 * 2.0f, 32 * 1.6f} });
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
	//AddActorLocation(Dir * _DeltaTime * 150.0f);
	//Gravity

}


void APlay_Bullet::BulletGravityCheck(float _DeltaTime)
{
	BulletGravityVector += BulletGravityAcc * _DeltaTime*.9;
	/*Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(),
		GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		BulletGravityVector = FVector::Zero;
		BulletMoveVector = FVector::Zero;
	}*/
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
{//Monster�� Bullet�̶� collision
	//���� bullet ������ �Լ� �߰��ϱ� 

	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, MonsterResult))
	{
		StateChange(EBulletState::BulletBomb);
		return  true;
	}

	return  false;
}

bool APlay_Bullet::BulletColorCheck(float _DeltaTime)
{

	FVector CheckPos = GetActorLocation();
	CheckPos.Y -= 10.0f;
	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	if (ColorCyan == Color8Bit(0, 255, 255, 0))
	{
		
		StateChange(EBulletState::BulletBomb);
		return true;
	}

	return false;
	// bool -> �ε������� return true �ϵ���!!
}
