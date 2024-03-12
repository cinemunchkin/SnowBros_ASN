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
	// �ִϸ��̼� -������ ��, ������ �� 

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
	ItemMoveUpdate(_DeltaTime);
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



void APlay_Item::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	ItemRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_Item::GetAnimationFullName(std::string _Name)
{
	return _Name;

}



void APlay_Item::ItemMoveUpdate(float _DeltaTime)
{
	ItemGravityCheck(_DeltaTime);
	ItemLastMoveVector(_DeltaTime);
	AddActorLocation(TotalLastMoveVector * _DeltaTime);
}

void APlay_Item::ItemLastMoveVector(float _DeltaTime)
{

	
	TotalLastMoveVector = FVector::Zero;
	TotalLastMoveVector = TotalLastMoveVector + ItemMoveVector;
	TotalLastMoveVector = TotalLastMoveVector + ItemGravityVector;

}


void APlay_Item::ItemGravityCheck(float _DeltaTime)
{ //���� ; �� ����ϸ鼭 ������������..  �ٴڿ� �����ɾƾ��ϴµ�.. 
	ItemGravityVector += ItemGravityAcc * _DeltaTime; 
		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(),
			GetActorLocation().iY(), Color8Bit::CyanA);
		if (Color == Color8Bit(0, 255, 255, 0))
		{
			TotalLastMoveVector = FVector::Zero;
		}

}

void APlay_Item::ItemColCheck(float _Deltatime)
{//Item�� �÷��̾ �浹�ߴ��� 
	

	std::vector<UCollision*> PlayerResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{
		Destroy();
	}
		
}