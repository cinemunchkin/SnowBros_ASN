#pragma once
#include <EngineCore\EngineCore.h>

// Ό³Έν :
class USnowBros_Core : public UEngineCore
{
public:
	// constrcuter destructer
	USnowBros_Core();
	~USnowBros_Core();

	// delete Function
	USnowBros_Core(const USnowBros_Core& _Other) = delete;
	USnowBros_Core(USnowBros_Core&& _Other) noexcept = delete;
	USnowBros_Core& operator=(const USnowBros_Core& _Other) = delete;
	USnowBros_Core& operator=(USnowBros_Core&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:

};

