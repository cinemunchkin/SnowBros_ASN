#pragma 

#include <EngineCore/Collision.h>
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"
#include <EngineBase/Transform.h>



class APlay_Item : public AActor
{
public:
	// constrcuter destructer
	APlay_Item();
	~APlay_Item();

	// delete Function
	APlay_Item(const APlay_Item& _Other) = delete;
	APlay_Item(APlay_Item&& _Other) noexcept = delete;
	APlay_Item& operator=(const APlay_Item& _Other) = delete;
	APlay_Item& operator=(APlay_Item&& _Other) noexcept = delete;

	std::string GetAnimationFullName(std::string _Name);
	FVector ItemMoveVector = FVector::Right;


	void SetAnimation(std::string _Name);

protected:

	EActorDir ItemDirState = EActorDir::Left;	

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	
	void StateChange(EItemState _State);
	void StateUpdate(float _DeltaTime);

	void ItemColCheck(float _Deltatime);
	void ItemPosVector(float _DeltaTime);


	void ItemSushi(float _DeltaTime);
	void ItemScore(float _DeltaTime);


	void ItemSushiStart();
	void ItemScoreStart();

	EItemState State = EItemState::None;


private:

	UImageRenderer* ItemRenderer = nullptr;
	UCollision* BodyCollision = nullptr;

};