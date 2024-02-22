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

		StateChange(EBulletState::NormalAttack);
	
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

		//BulletDir = FVector::Zero;

		FVector BulletDirNormal = BulletDir.Normalize2DReturn();

		AddActorLocation(BulletDirNormal * _DeltaTime* PlayerPos);
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
		case EBulletState::NormalAttack:
			BulletFired();
			break;
			

		case EBulletState::ColAttack:
			BulletCol();
			break;
					
		default:
			break;
		}
	}

	State = _State;


}


void APlay_Bullet::BulletFired()
{
	
	
	APlay_Player* Player = APlay_Player::GetMainPlayer();
	
	BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));
	
	if (true == UEngineInput::IsPress(VK_SPACE))
	{

		////Ű�� ������ -> �׶� ������ �Ǿ�� �ϴµ� �� 
		//APlay_Bullet* Bullet = SpawnActor<APlay_Bullet>();
		//Bullet->SetName("Bullet");
		//Bullet->SetActorLocation(Player->GetActorLocation());
	}



}

void APlay_Bullet::BulletCol()
{

}

void APlay_Bullet::Bullet_Destroy()
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






//
//
//
//void APlay_Bullet::DirCheck() //�ٽ� ���� �ʿ�
//{
//	EActorDir Dir = DirState;
//
//	//APlay_Bullet* NewBullet = APlay_Bullet
//
//	
//	/*APlay_Player* Player = APlay_Player::GetMainPlayer();
//
//	if (DirCheck() == GetActorLocation())
//	{
//		Dir = EActorDir::Left;
//	}
//	if (UEngineInput::IsPress(VK_RIGHT))
//	{
//		Dir = EActorDir::Right;
//	}*/
//
//
//
//	if (Dir != DirState)
//	{
//		DirState = Dir;
//		std::string Name = GetAnimationName(CurAnimationName);
//
//		MonsterRenderer->ChangeAnimation(Name, true, MonsterRenderer->GetCurAnimationFrame(), MonsterRenderer->GetCurAnimationTime());
//		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
//		MonsterRenderer->ChangeAnimation(Name);
//	}
//}
