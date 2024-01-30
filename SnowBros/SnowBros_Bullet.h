#pragma once
#include <EngineCore\Actor.h>

// Ό³Έν :
class ASnowBros_Bullet : public AActor
{
public:
	// constrcuter destructer
	ASnowBros_Bullet();
	~ASnowBros_Bullet();

	// delete Function
	ASnowBros_Bullet(const ASnowBros_Bullet& _Other) = delete;
	ASnowBros_Bullet(ASnowBros_Bullet&& _Other) noexcept = delete;
	ASnowBros_Bullet& operator=(const ASnowBros_Bullet& _Other) = delete;
	ASnowBros_Bullet& operator=(ASnowBros_Bullet&& _Other) noexcept = delete;

	void SetDir(const FVector& _Dir)
	{
		Dir = _Dir;
	}

protected:
	void Tick(float _DeltaTime) override;
	void BeginPlay() override;

private:
	FVector Dir = FVector::Zero;
	float Speed = 50.0f;
};

