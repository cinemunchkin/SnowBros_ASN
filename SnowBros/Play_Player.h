#pragma once
#include <EngineCore\Actor.h>
#include <EngineBase/Transform.h>
#include "SnowBros_Helper.h"
#include "Play_Monster.h"
#include "Play_Bullet.h"

class APlay_Player : public AActor
{
	

private:
	static APlay_Player* MainPlayer;


public:
	
	static APlay_Player* GetMainPlayer();
	
	// constrcuter destructer
	APlay_Player();
	~APlay_Player();

	// delete Function
	APlay_Player(const APlay_Player& _Other) = delete;
	APlay_Player(APlay_Player&& _Other) noexcept = delete;
	APlay_Player& operator=(const APlay_Player& _Other) = delete;
	APlay_Player& operator=(APlay_Player&& _Other) noexcept = delete;

	EPlayState GetState()
	{
		return State;
	}
	
	void PlayerColState(EPlayState _State);
	void DirCheck();

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	FVector PlayerRollingSpeed = FVector::Right * 150.0f;
	UImageRenderer* Renderer = nullptr;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 상태 보조 함수
	//void GravityCheck(float _DeltaTime);

	// 각 상태마다 언제나 가장 위에 실행되어야 한다.
	

	FVector MonsterDir = FVector::Left;

	std::string GetAnimationName(std::string _Name);

	//int SnowStackNum = -1;

	// 상태 주요 업데이트
	void StateChange(EPlayState _State);
	void StateUpdate(float _DeltaTime);
	void PlayerColPhysics(float _DeltaTime);
	void StrobeColCheck(float _DeltaTime);


	// 상태 함수들
	//void CameraFreeMove(float _DeltaTime);
	//void FreeMove(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Run(float _DeltaTime);
	void FastRun(float _DeltaTime);
	
	void Jump(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Attack(float _DeltaTime);

	void Strobe(float _DeltaTime);
	void Fly(float _DeltaTime); // 스테이지 이동할때 쓰네.. 

	void PlayerRolling(float _DeltaTime);
	void PlayerPush(float _DeltaTime);
	void PlayerStuck(float _DeltaTime);

	// 상태 시작 함수들
	void IdleStart();
	void RunStart();
	void FastRunStart();
	
	void JumpStart();
	void DownJumpStart();
	void AttackStart();

	void StrobeStart();
	void FlyStart();

	void PlayerRollingStart();
	void PlayerPushStart();
	void PlayerStuckStart();


	bool IsStrobeUpdate = false;
	

	//void PlayerColPhysics(float _DeltaTime);
	
	std::string CurAnimationName = "None";

private:
	UCollision* PushCheckCollision = nullptr;
	UCollision* BodyCollision = nullptr;
	UCollision* BulletCollision = nullptr;


	float AnimationTime = 0.0f;
	int AnimationFrame = 0;


	float AlphaTime = 0.0f;
	bool Dir = false;


	FVector GetPlayerDir()
	{
		return PlayerDir;
	}

	
	


	float FreeMoveSpeed = 1000.0f;

	//float JumpHeight = 50.0f;
	//float JumpTiming = 20.0f;
	

	// y는 존재하지 않는다.
	//가속도 필요 없음  + 점프 띠용 자연스럽게 구현 -> 중력만 잘 조절

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 500.0f;
	float MoveMaxSpeed = 500.0f;
	void AddMoveVector(const FVector& _DirDelta);

	FVector JumpPower = FVector::Up * 650.0f;
	FVector JumpVector = FVector::Zero;
	

	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector GravityVector = FVector::Zero;

	int CollisionNum = 0;


	// 내가 나갈 모든 방향의 합
	FVector LastMoveVector = FVector::Zero;

	bool DamageCheck();

	bool PushCheck();
	bool RollingCheck();

	// 무브 업데이트는 상태가 아닙니다.
	// 이동
	void CalLastMoveVector(float _DeltaTime);
	void CalMoveVector(float _DeltaTime);
//	void CalJumpVector(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CamMoveLastMoveVector(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);
	
	void Fire_Bullet();
	FVector PlayerDir = FVector::Zero;
	
	void GroundUp(float _DeltaTime);


};

