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



	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 500.0f;

	bool IsSnowballComplete = false; // Snowball이 5가 되면 true로 두자

protected:


	void DirCheck();

	void StateChange(ESnowballState _State);
	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);

	EActorDir SnowballDirState = EActorDir::Left;
	ESnowballState State = ESnowballState::None;
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	std::string CurAnimationName = "None";

	void AddMoveVector(const FVector& _DirDelta);



	void SnowballPhysics(float _DeltaTime);
	void SnowballMove(float _DeltaTime);
	void SnowballStateUpdate(float _DeltaTime);
	void SnowballStack(float _DeltaTime);



	void Snowball05_Start();
	void Snowball01_Start();
	void Snowball02_Start();
	void Snowball03_Start();
	void Snowball04_Start();



private:

	UImageRenderer* SnowBallRenderer = nullptr;
	UCollision* SnowCollision = nullptr;
	EActorDir SnowBallDir = EActorDir::Right;
};