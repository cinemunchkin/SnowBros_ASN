#pragma once
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"


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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	// 상태 보조 함수
	//void GravityCheck(float _DeltaTime);

	// 각 상태마다 언제나 가장 위에 실행되어야 한다.
	void DirCheck();

	

	std::string GetAnimationName(std::string _Name);

	// 상태 주요 업데이트
	void StateChange(EPlayState _State);
	void StateUpdate(float _DeltaTime);
	void Col_Strobe(float _DeltaTime)

	// 상태 함수들
	void CameraFreeMove(float _DeltaTime);
	void FreeMove(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Run(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Fly(float _DeltaTime); // 스테이지 이동할때 쓰네.. 

	// 상태 시작 함수들
	void IdleStart();
	void RunStart();
	void JumpStart();
	void DownJumpStart();

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UCollision* BodyCollision = nullptr;

	UImageRenderer* Renderer = nullptr;
	float AnimationTime = 0.0f;
	int AnimationFrame = 0;


	float AlphaTime = 0.0f;
	bool Dir = false;



	float FreeMoveSpeed = 1000.0f;

	//float JumpHeight = 50.0f;
	//float JumpTiming = 20.0f;
	

	// y는 존재하지 않는다.
	//가속도 필요 없음  + 점프 띠용 자연스럽게 구현 -> 중력만 잘 조절

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 500.0f;
	float MoveMaxSpeed = 500.0f;
	void AddMoveVector(const FVector& _DirDelta);

	FVector GravityAcc = FVector::Down * 2500.0f;
	FVector GravityVector = FVector::Zero;


	FVector JumpPower = FVector::Up * 600.0f;
	FVector JumpVector = FVector::Zero;

	// 내가 나갈 모든 방향의 합
	FVector LastMoveVector = FVector::Zero;

	// 무브 업데이트는 상태가 아닙니다.
	// 이동
	void CalLastMoveVector(float _DeltaTime);
	void CalMoveVector(float _DeltaTime);
	void CalJumpVector(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CamMoveLastMoveVector(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);
	//void GroundUp();


};

