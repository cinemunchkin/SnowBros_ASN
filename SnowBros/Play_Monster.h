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


	EMonsterState GetState()
	{
		return State;
	}

	bool IsRolling()
	{
		return State == EMonsterState::Snowball && SnowStack > 3;
	}

	void Rolling(float _DeltaTime);
	void StateChange(EMonsterState _State);
	void ColMoveUpdate(float _DeltaTime);

	void SetMoveVector(FVector _MoveVector)
	{
		MoveVector = _MoveVector;
	}

	int SnowStack = -1;
	FVector MonsterDir = FVector::Zero;
	EMonsterDir MonsterDirState = EMonsterDir::Right;
	FVector RollingSpeed = FVector::Right * 250.0f;

	EMonsterDir NextDir = EMonsterDir::Left;
	EMonsterDir PrevDir = EMonsterDir::Mid;
	UImageRenderer* SnowBallRenderer = nullptr;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	bool BulletColCheck(float _DeltaTime);

	void MonsterColPhysics(float _DeltaTime);
	void MonsterMoveUpdate(float _DeltaTime);

	void AddMoveVector(const FVector& _DirDelta); // 가속도 -> 등속으로 바꿈

	void DirCheck();



	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);
	void SetAnimation(std::string _Name);


	// 상태 주요 업데이트
	void StateUpdate(float _DeltaTime);


	void MonIdle(float _DeltaTime);
	void MonMove(float _DeltaTime);
	void Jump(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Snowball(float _DeltaTime);
	//void Rolling(float _DeltaTime); -> public에 있으니 안심해!!
	void MonFlying(float _DeltaTime);
	

	//void StackSnowball(float _DeltaTime);
	//void SnowballStackCheck(float _DeltaTime);
	
	

	// 상태 시작 함수들
	void MonIdleStart();
	void MonMoveStart();
	void JumpStart();
	void DownJumpStart();
	void SnowballStart();
	void RollingStart();
	void MonFlyingStart();


	void MoveCheck(float _DeltaTime);

	EMonsterState State = EMonsterState::None;
	
	std::string CurAnimationName = "None";
	


private:

	FVector MoveAcc = FVector::Right * 50.0f;

	FVector JumpVector = FVector::Zero;
	FVector MoveVector = FVector::Zero;
	FVector GravityVector = FVector::Zero;

	FVector GravityAcc = FVector::Down * 2500.0f;

	UCollision* BodyCollision= nullptr;
	UImageRenderer* MonsterRenderer = nullptr;

	FVector TotalLastMoveVector = FVector::Zero;
	
	
	//UCollision* SnowCollision = nullptr;
	EActorDir SnowBallDir = EActorDir::Right;

	void MonsterMoveVector(float _DeltaTime);
	void MonsterLastMoveVector(float _DeltaTime);
	void MonsterGravityVector(float _DeltaTime);
	void MonsterGroundUp(float _DeltaTime);

	void MonsterDeath(float _Deltatime);
	void Spawn_Item();

};

