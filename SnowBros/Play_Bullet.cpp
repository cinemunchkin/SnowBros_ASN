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

		StateChange(EBulletState::AttackNormal);
	
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
		// 아 오키 방향을 정해줘야하는구만 ㅇㅋㅇㅋ
		
		AddActor
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


