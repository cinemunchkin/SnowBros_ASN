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

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	
	void PlayerColState(EPlayState _State);

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
	void PlayerColPhysics(float _DeltaTime);

	// ���� �Լ���
	//void CameraFreeMove(float _DeltaTime);
	//void FreeMove(float _DeltaTime);
	void Idle(float _DeltaTime);
	void Run(float _DeltaTime);
	void FastRun(float _DeltaTime);
	
	void Jump(float _DeltaTime);
	void DownJump(float _DeltaTime);
	void Attack(float _DeltaTime);

	void Strobe(float _StrobeTime);
	void Fly(float _DeltaTime); // �������� �̵��Ҷ� ����.. 

	// ���� ���� �Լ���
	void IdleStart();
	void RunStart();
	void FastRunStart();
	
	void JumpStart();
	void DownJumpStart();
	void AttackStart();

	void StrobeStart();
	void FlyStart();

	bool IsStrobeUpdate = false;

	//void PlayerColPhysics(float _DeltaTime);
	
	std::string CurAnimationName = "None";

private:
	UCollision* BodyCollision = nullptr;
	UCollision* BulletCollision = nullptr;

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

	FVector JumpPower = FVector::Up * 650.0f;
	FVector JumpVector = FVector::Zero;
	

	FVector GravityAcc = FVector::Down * 2500.0f;
	FVector GravityVector = FVector::Zero;



	// ���� ���� ��� ������ ��
	FVector LastMoveVector = FVector::Zero;

	// ���� ������Ʈ�� ���°� �ƴմϴ�.
	// �̵�
	void CalLastMoveVector(float _DeltaTime);
	void CalMoveVector(float _DeltaTime);
	void CalJumpVector(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CamMoveLastMoveVector(float _DeltaTime);
	void MoveUpdate(float _DeltaTime);
	void StrobeUpdate(float _DeltaTime);
	
	void Fire_Bullet();
	
	//void GroundUp();


};

