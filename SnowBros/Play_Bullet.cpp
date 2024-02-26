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
		BulletRenderer->SetImage("SnowBros_Bullet_L.png");
		BulletRenderer->SetTransform({ {16,-24}, {32*1.2f, 68*1.2f} });
		
		BulletRenderer->CreateAnimation("Bullet_Right","SnowBros_Bullet_R.png", 0, 1, 0.05f, true);
		BulletRenderer->CreateAnimation("Bullet_Left", "SnowBros_Bullet_L.png", 0, 1, 0.05f, true);

		StateChange(EBulletState::BulletNormal);
	
	}

	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Bullet);
		BodyCollision->SetPosition(BulletRenderer->GetPosition()); // 오!! 됐다!! Bullet Position = Bullet Collision 
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
		// 이런식으로 상대를 사용할수 있다.
		BodyCollision->SetActive(true, 1.0f);
		// BodyCollision = nullptr;
	}

	
	{
		FVector PlayerPos = Player->GetActorLocation();
		FVector BulletPos = GetActorLocation(); // 플레이어, 공격 포지션 받아서 

		/*
		플레이어 방향이 -> Right이면 / Left면 
		*/
		
		APlay_Bullet* Bullet = nullptr;
		
		if (true == UEngineInput::IsPress(VK_RIGHT))
		{
			FVector BulletDir = FVector::Right;

			FVector BulletDirNormal = BulletDir.Normalize2DReturn();
			AddActorLocation(BulletDirNormal * _DeltaTime * PlayerPos);


		}
		else
		{
			FVector BulletDir = FVector::Left;

			FVector BulletDirNormal = BulletDir.Normalize2DReturn();
			AddActorLocation(BulletDirNormal * _DeltaTime * PlayerPos);

		}

	

		





		//Bullet->DirState.X-> SetBulletDir(Player->DirState.X)

		//fvector                float
		//Bullet->SetBulletDir(Player->DirState);
		//fvector 에서 x랑 y랑 따로 꺼내면 됨!!!
		//

		
		
		
	}
	
	BulletRenderer->Destroy(0.1f);

}





void APlay_Bullet::DirCheck()
{
	EBulletDir Dir = BulletDirState;
	//if (플레이어의 방향이 Left면)
	//{
	//	Dir = EActorDir::Left;
	//}
	//if (플레이어의 방향이 Right면)
	//{
	//	Dir = EActorDir::Right;
	//}

	

	if (Dir != BulletDirState)
	{
		BulletDirState = Dir;
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
		case EBulletState::BulletNormal:
			BulletRenderer->ChangeAnimation(GetAnimationName("Bullet"));
			break;
			
		case EBulletState::BulletCol:
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


