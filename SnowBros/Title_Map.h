#pragma once
#include <EngineCore\Actor.h>

// Ό³Έν :
class ATitle_Map : public AActor
{
public:
	// constrcuter destructer
	ATitle_Map();
	~ATitle_Map();

	// delete Function
	ATitle_Map(const ATitle_Map& _Other) = delete;
	ATitle_Map(ATitle_Map&& _Other) noexcept = delete;
	ATitle_Map& operator=(const ATitle_Map& _Other) = delete;
	ATitle_Map& operator=(ATitle_Map&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

