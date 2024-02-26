#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>

#include <string_view>

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
		//�ϴ� ��������� ������ ����!
		BulletRenderer = CreateImageRenderer(SnowBrosRenderOrder::Bullet);
		BulletRenderer->SetImage("SnowBros_Bullet_R.png");
		BulletRenderer->SetTransform({ {16,-24}, {32*1.2f, 68*1.2f} });
		
		BulletRenderer->CreateAnimation("Bullet_Right","SnowBros_Bullet_R.png", 0, 1, 0.05f, true);

		//StateChange(EBulletState::AttackNormal);
	
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Bullet);
		BodyCollision->SetPosition(BulletRenderer->GetPosition()); // ��!! �ƴ�!! Bullet Position = Bullet Collision 
		BodyCollision->SetScale({ 25, 25 });
		BodyCollision->SetColType(ECollisionType::CirCle);
	}


}

void APlay_Bullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, Result))
	{
		// �̷������� ��븦 ����Ҽ� �ִ�.
		BodyCollision->SetActive(true, 1.0f);
		// BodyCollision = nullptr;
	}

	
	//Bullet �ϴ� �÷��̾� ��ó���� ���ľ���
	// ���� Ű ������ spawn�ǵ���

	{
		FVector PlayerPos = Player->GetActorLocation();
		FVector BulletPos = GetActorLocation();

		FVector BulletDir = FVector::Right;

		FVector BulletDirNormal = BulletDir.Normalize2DReturn();


		
		AddActorLocation(BulletDirNormal * _DeltaTime * PlayerPos);
		// �� ��Ű ������ ��������ϴ±��� ��������
		
	BulletRenderer -> Destroy(0.1f);
	}
	

}





void APlay_Bullet::DirCheck()
{
	EBulletDir Dir = BulletDirState;
	//if (�÷��̾��� ������ Left��)
	//{
	//	Dir = EActorDir::Left;
	//}
	//if (�÷��̾��� ������ Right��)
	//{
	//	Dir = EActorDir::Right;
	//}

	

	if (Dir != BulletDirState)
	{
		BulletDirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		//Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer->GetCurAnimationTime());
		////Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
		//Renderer->ChangeAnimation(Name);
	}
}


void APlay_Bullet::StateChange(EBulletState _State)

{

	if (State != _State)
	{
		switch (_State)
		{
		case EBulletState::AttackNormal:
			BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));
			break;
			
		case EBulletState::AttackCol:
			//BulletCol();
			break;
					
		default:
			break;
		}
	}

	State = _State;


}



void APlay_Bullet::BulletNormal()
{

}
void APlay_Bullet::BulletCol()
{

}

void APlay_Bullet::BulletDestroy()
{

}
//{
//	if (/*�浹�ϸ�0 > GetPos().Y*/)
//	{
//		Destroy();
//		return;
//	}
//	else
//	{
//		/*
//		n���� ����
//		*/
//	}
//AddMoveVector;
//}




std::string APlay_Bullet::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (BulletDirState)
	{
	case EBulletDir::Left:
		DirName = "_Left";
		break;

	case EBulletDir::Right:
		DirName = "_Right";
		break;
		
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;

}


