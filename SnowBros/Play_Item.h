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

	void SetAnimation(std::string _Name);

protected:


	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	EActorDir ItemDirState = EActorDir::Left;	

	void ItemColCheck(float _Deltatime);

private:

	UImageRenderer* ItemRenderer = nullptr;
	UCollision* BodyCollision = nullptr;
	EActorDir ItemDir = EActorDir::Right;


};