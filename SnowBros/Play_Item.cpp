#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/Level.h>


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


	ItemRenderer->CreateAnimation("MonSushi", "Monster_Item_01.png", 0, 11, 1.0f, true);
	}


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Item);
		BodyCollision->SetPosition(ItemRenderer->GetPosition());
		BodyCollision->SetScale({ 40,40 });
		BodyCollision->SetColType(ECollisionType::Rect);
	}



}

void APlay_Item::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);




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
	


}