#pragma once
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"

// FSM에 대해서 알아보자.
// 한번에 한가지 상태만을 가진다.
// 플레이어가 한번에 1개의 함수만을 실행한다.

// 설명 :
class APlay_Player : public AActor
{
public:
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
	void GravityCheck(float _DeltaTime);

	// 각 상태마다 언제나 가장 위에 실행되어야 한다.
	void DirCheck();

	void GravityOff(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);

	// 상태 주요 업데이트
	void StateChange(EPlayState _State);
	void StateUpdate(float _DeltaTime);

	// 상태 함수들
	void CameraFreeMove(float _DeltaTime);
	void FreeMove(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Move(float _DeltaTime);
	void DownJump(float _DeltaTime);

	// 상태 시작 함수들
	void IdleStart();
	void MoveStart();
	void JumpStart();

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

private:
	UImageRenderer* Renderer = nullptr;
	float AnimationTime = 0.0f;
	int AnimationFrame = 0;

	float AlphaTime = 0.0f;
	bool Dir = false;

	float FreeMoveSpeed = 1000.0f;
	float JumpHeight = 50.0f;
	float JumpTiming = 20.0f;
	

	float MoveSpeed = 500.0f;
	float Gravity = 500.0f;
};

