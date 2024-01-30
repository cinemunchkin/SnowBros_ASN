#pragma once
#include <EngineCore\Actor.h>

class SnowBros_Player : public AActor
{
public:
	// constrcuter destructer
	SnowBros_Player();
	~SnowBros_Player();

	// delete Function
	SnowBros_Player(const SnowBros_Player& _Other) = delete;
	SnowBros_Player(SnowBros_Player&& _Other) noexcept = delete;
	SnowBros_Player& operator=(const SnowBros_Player& _Other) = delete;
	SnowBros_Player& operator=(SnowBros_Player&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	float QSkillCool = 0.0f;

	UImageRenderer* HeadRenderer;
	UImageRenderer* BodyRenderer;

	// std::vector<UImageRenderer*>
};

