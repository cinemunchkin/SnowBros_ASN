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
	FVector ItemMoveVector = FVector::Zero;
	FVector ItemGravityVector = FVector::Zero;
	FVector ItemGravityAcc = FVector::Down * 1000.0f;


	void SetAnimation(std::string _Name);

protected:

	EActorDir ItemDirState = EActorDir::Left;	

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	
	void StateChange(EItemState _State);
	void StateUpdate(float _DeltaTime);

	void ItemColCheck(float _Deltatime);
	void ItemMoveUpdate(float _DeltaTime);
	void ItemLastMoveVector(float _DeltaTime);
	void ItemGravityCheck(float _DeltaTime);


	void ItemSushi(float _DeltaTime);
	void ItemScore(float _DeltaTime);


	void ItemSushiStart();
	void ItemScoreStart();

	EItemState State = EItemState::None;


private:
	FVector GravityVector = FVector::Zero;
	FVector TotalLastMoveVector = FVector::Zero;

	UImageRenderer* ItemRenderer = nullptr;
	UCollision* BodyCollision = nullptr;

};