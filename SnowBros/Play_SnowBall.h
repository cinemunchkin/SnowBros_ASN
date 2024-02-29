#pragma once
#include <EngineCore\Level.h>
#include <EngineCore\Actor.h>

class APlay_SnowBall : public ULevel, public AActor
{
public:
	// constrcuter destructer
	APlay_SnowBall();
	~APlay_SnowBall();

	// delete Function
	APlay_SnowBall(const APlay_SnowBall& _Other) = delete;
	APlay_SnowBall(APlay_SnowBall&& _Other) noexcept = delete;
	APlay_SnowBall& operator=(const APlay_SnowBall& _Other) = delete;
	APlay_SnowBall& operator=(APlay_SnowBall&& _Other) noexcept = delete;

	void SetAnimation(std::string _Name);

protected:

	void StateChange(ESnowballState _State);
	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);

	EActorDir SnowballDirState = EActorDir::Left;
	ESnowballState State = ESnowballState::None;
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	std::string CurAnimationName = "None";




	void SnowballStart();



private:

	UImageRenderer* SnowBallRenderer = nullptr;
	UCollision* BodyCollision = nullptr;
	EActorDir SnowBallDir = EActorDir::Right;
};