#pragma once
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"
#include "Play_Player.h"

// Ό³Έν :
class APlay_Monster : public AActor
{
public:
	// constrcuter destructer
	APlay_Monster();
	~APlay_Monster();

	// delete Function
	APlay_Monster(const APlay_Monster& _Other) = delete;
	APlay_Monster(APlay_Monster&& _Other) noexcept = delete;
	APlay_Monster& operator=(const APlay_Monster& _Other) = delete;
	APlay_Monster& operator=(APlay_Monster&& _Other) noexcept = delete;

	
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;




private:
	UCollision* BodyCollision;
};

