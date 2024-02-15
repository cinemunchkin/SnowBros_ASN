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

	// ���� ���� �Լ�
	//void GravityCheck(float _DeltaTime);

	// �� ���¸��� ������ ���� ���� ����Ǿ�� �Ѵ�.
	void DirCheck();

	

	std::string GetAnimationName(std::string _Name);

	// ���� �ֿ� ������Ʈ
	void StateChange(EPlayState _State);
	void StateUpdate(float _DeltaTime);

	// ���� �Լ���
	void CameraFreeMove(float _DeltaTime);
	void FreeMove(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Jump(float _DeltaTime);
	void Run(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Fly(float _DeltaTime); // �������� �̵��Ҷ� ����.. 

	// ���� ���� �Լ���
	void IdleStart();
	void MoveStart();
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
	

	// y�� �������� �ʴ´�.
	//���ӵ� �ʿ� ����  + ���� ��� �ڿ������� ���� -> �߷¸� �� ����

	FVector MoveVector = FVector::Zero;
	FVector MoveAcc = FVector::Right * 500.0f;
	float MoveMaxSpeed = 500.0f;
	void AddMoveVector(const FVector& _DirDelta);

	FVector GravityAcc = FVector::Down * 2000.0f;
	FVector GravityVector = FVector::Zero;


	FVector JumpPower = FVector::Up * 1000;
	FVector JumpVector = FVector::Zero;

	// ���� ���� ��� ������ ��
	FVector LastMoveVector = FVector::Zero;

	// ���� ������Ʈ�� ���°� �ƴմϴ�.
	// �̵�
	void CalLastMoveVector(float _DeltaTime);
	void CalMoveVector(float _DeltaTime);
	void CalJumpVector(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	//void CamMoveLastMoveVector(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);
	//void GroundUp();


};
