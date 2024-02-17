#pragma once
#include <EngineCore\Actor.h>


// Ό³Έν :
class APlay_Map : public AActor
{
public:
	// constrcuter destructer
	APlay_Map();
	~APlay_Map();

	// delete Function
	APlay_Map(const APlay_Map& _Other) = delete;
	APlay_Map(APlay_Map&& _Other) noexcept = delete;
	APlay_Map& operator=(const APlay_Map& _Other) = delete;
	APlay_Map& operator=(APlay_Map&& _Other) noexcept = delete;

	void SetColMapImage(std::string_view _MapImageName);
	void SetMapImage(std::string_view _MapImageName);

	void SwitchDebug();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	UImageRenderer* Renderer = nullptr;
	UImageRenderer* ColRenderer = nullptr;
};

