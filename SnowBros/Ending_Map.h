#pragma once
#include <EngineCore\Actor.h>

class AEnding_Map : public AActor
{
public:
	// constrcuter destructer
	AEnding_Map();
	~AEnding_Map();

	// delete Function
	AEnding_Map(const AEnding_Map& _Other) = delete;
	AEnding_Map(AEnding_Map&& _Other) noexcept = delete;
	AEnding_Map& operator=(const AEnding_Map& _Other) = delete;
	AEnding_Map& operator=(AEnding_Map&& _Other) noexcept = delete;

	
	void SetMapImage(std::string_view _MapImageName);




protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:


	UImageRenderer* Renderer = nullptr;
	UImageRenderer* ColRenderer = nullptr;
};