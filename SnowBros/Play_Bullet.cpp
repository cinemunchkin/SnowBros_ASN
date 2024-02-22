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
		//일단 만들어지는 지부터 보자!
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
	
	//Bullet 일단 플레이어 근처에서 얼쩡얼쩡
	// 이제 키 누르면 spawn되도록

	{
		FVector PlayerPos = Player->GetActorLocation();
		FVector BulletPos = GetActorLocation();


		//FVector BulletDir = PlayerPos - BulletPos;
		// 총알이 몸 바깥으로 가도록 어떻게하지
		FVector BulletDir = FVector::Right;

		//BulletDir = FVector::Zero;

		FVector BulletDirNormal = BulletDir.Normalize2DReturn();

		AddActorLocation(BulletDirNormal * _DeltaTime* PlayerPos);
	}

}





void APlay_Bullet::DirCheck()
{
	EActorDir Dir = DirState;
	//if (플레이어의 방향이 Left면)
	//{
	//	Dir = EActorDir::Left;
	//}
	//if (플레이어의 방향이 Right면)
	//{
	//	Dir = EActorDir::Right;
	//}

	

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		//Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer->GetCurAnimationTime());
		////특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
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

		////키를 누르면 -> 그때 스폰이 되어야 하는디 흠 
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
//void APlay_Bullet::DirCheck() //다시 설정 필요
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
//		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
//		MonsterRenderer->ChangeAnimation(Name);
//	}
//}
