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
		BulletRenderer->SetTransform({ {0,1}, {64, 64} });
		
		BulletRenderer->CreateAnimation("Bullet_Right","SnowBros_Bullet_R.png", 0, 1, 0.1f, true);

		StateChange(EBulletState::AttackNormal);
	
	}


}

void APlay_Bullet::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	APlay_Player* Player = APlay_Player::GetMainPlayer();
	
	//Bullet �ϴ� �÷��̾� ��ó���� ���ľ���
	// ���� Ű ������ spawn�ǵ���

	{
		FVector PlayerPos = Player->GetActorLocation();
		FVector BulletPos = GetActorLocation();


		//FVector BulletDir = PlayerPos - BulletPos;
		// �Ѿ��� �� �ٱ����� ������ �������
		FVector BulletDir = FVector::Right;


		if (true == UEngineInput::IsPress(VK_LEFT))
		{
			BulletDir = FVector::Left;
		}
		if (true == UEngineInput::IsPress(VK_RIGHT))
		{
			BulletDir = FVector::Right;
		}
		else
		{
			BulletDir = FVector::Right;
		}


		//BulletDir = FVector::Zero;

		FVector BulletDirNormal = BulletDir.Normalize2DReturn();

		//AddActorLocation(BulletDirNormal * _DeltaTime* PlayerPos);
		// �� ��Ű ������ ��������ϴ±��� ��������
		
		AddActor
	}

}





void APlay_Bullet::DirCheck()
{
	EActorDir Dir = DirState;
	//if (�÷��̾��� ������ Left��)
	//{
	//	Dir = EActorDir::Left;
	//}
	//if (�÷��̾��� ������ Right��)
	//{
	//	Dir = EActorDir::Right;
	//}

	

	if (Dir != DirState)
	{
		DirState = Dir;
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
			BulletNormal();
			break;
			

		case EBulletState::AttackCol:
			BulletCol();
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

	switch (DirState)
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


