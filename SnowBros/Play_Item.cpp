#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>

#include <vector>


#include "Play_Item.h"


APlay_Item::APlay_Item()
{
}

APlay_Item::~APlay_Item()
{
}

void APlay_Item::BeginPlay()
{
	AActor::BeginPlay();
	{
		ItemRenderer = CreateImageRenderer(SnowBrosRenderOrder::Item);
	ItemRenderer->SetImage("Monster_Item_01.png");
	ItemRenderer->SetTransform({ {0,-26}, {48 * 0.6f, 48 * 0.6f} });


	ItemRenderer->CreateAnimation("ItemSushi", "Monster_Item_01.png", 0, 11, 1.0f, true);
	}


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Item);
		BodyCollision->SetPosition(ItemRenderer->GetPosition());
		BodyCollision->SetScale({ 40,40 });
		BodyCollision->SetColType(ECollisionType::Rect);
	}

	StateChange(EItemState::ItemSushi);

}

void APlay_Item::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}



void APlay_Item::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EItemState::ItemSushi:
		ItemSushi(_DeltaTime);
		break;

	case EItemState::ItemScore:
		ItemScore(_DeltaTime);
		break;

	default:
		break;

	}

}


void APlay_Item::StateChange(EItemState _State)
{
	// 애니메이션 -스시일 때, 점수일 때 

	if (State != _State)
	{
		switch (_State)
		{
		case EItemState::ItemSushi:

			ItemSushiStart();
			break;

		case EItemState::ItemScore:
			ItemScoreStart();
			break;

		default:
			break;

		}
	}
	State = _State;
}

void APlay_Item::ItemSushi(float _DeltaTime)
{
	ItemPosVector(_DeltaTime);
	ItemColCheck(_DeltaTime);

}

void APlay_Item::ItemScore(float _DeltaTime)
{

}


void APlay_Item::ItemSushiStart()
{
	//ItemRenderer->ChangeAnimation(GetAnimationFullName("ItemSushi"));
		this->SetAnimation("ItemSushi");
}
void APlay_Item::ItemScoreStart(){}








void APlay_Item::ItemPosVector(float _DeltaTime)
{
	AddActorLocation(ItemMoveVector * _DeltaTime * 150.0f);
	/* 위치 잡는 함수..!
	어디에 놔!!!*/
}

void APlay_Item::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	ItemRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_Item::GetAnimationFullName(std::string _Name)
{
	return _Name;

}


void APlay_Item::ItemColCheck(float _Deltatime)
{//Item과 플레이어가 충돌했는지 
	

	std::vector<UCollision*> PlayerResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{
		Destroy();
	}

	
}