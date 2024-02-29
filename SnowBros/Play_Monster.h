#pragma once
#include <EngineCore\Actor.h>
#include <EngineBase/Transform.h>

#include "SnowBros_Helper.h"
#include "Play_Player.h"

// 설명 :
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

	FVector MonsterDir = FVector::Left;

	
protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void MonsterColPhysics(float _DeltaTime);
	void MonsterGravity(float _DeltaTime);


	void DirCheck();



	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);
	void SetAnimation(std::string _Name);


	// 상태 주요 업데이트
	void StateChange(EMonsterState _State);
	void StateUpdate(float _DeltaTime);


	void Idle(float _DeltaTime);
	void Jump(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Snowball(float _DeltaTime);
	void Rolling(float _DeltaTime);

	void StackSnowball(float _DeltaTime);
	void SnowballStackCheck(float _DeltaTime);
	
	int SnowStack = 0;

	// 상태 시작 함수들
	void IdleStart();
	void JumpStart();
	void DownJumpStart();
	void SnowballStart();
	void RollingStart();

	void MoveCheck(float _DeltaTime);

	EMonsterState State = EMonsterState::None;
	EMonsterDir MonsterDirState = EMonsterDir::Right;
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

