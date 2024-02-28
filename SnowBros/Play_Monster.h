#pragma once
#include <EngineCore\Actor.h>
#include <EngineBase/Transform.h>

#include "SnowBros_Helper.h"
#include "Play_Player.h"

// ���� :
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
	void Physics(float _DeltaTime);

	void DirCheck();



	std::string GetAnimationName(std::string _Name);



	// ���� �ֿ� ������Ʈ
	void StateChange(EMonsterState _State);
	void StateUpdate(float _DeltaTime);


	void Idle(float _DeltaTime);
	void Jump(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Snowball(float _DeltaTime);
	void Rolling(float _DeltaTime);


	// ���� ���� �Լ���
	void IdleStart();
	void JumpStart();
	void DownJumpStart();
	void SnowballStart();
	void RollingStart();

	void MoveCheck(float _DeltaTime);

	EMonsterState State = EMonsterState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";



private:


	FVector JumpVector = FVector::Zero;
	FVector MoveVector = FVector::Zero;
	FVector GravityVector = FVector::Zero;

	FVector GravityAcc = FVector::Down * 2500.0f;

	UCollision* BodyCollision= nullptr;
	UImageRenderer* MonsterRenderer = nullptr;
	FVector TotalLastMoveVector = FVector::Zero;
	

	void MonsterMoveVector(float _DeltaTime);
	void MonsterLastMoveVector(float _DeltaTime);
	void MonsterGravityVector(float _DeltaTime);

};

