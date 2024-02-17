#pragma once
#include <EngineCore\Actor.h>
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


	void DirCheck();



	std::string GetAnimationName(std::string _Name);



	// ���� �ֿ� ������Ʈ
	void StateChange(EPlayState _State);
	void StateUpdate(float _DeltaTime);


	void Idle(float _DeltaTime);

	// ���� ���� �Լ���
	void IdleStart();
	

	EPlayState State = EPlayState::None;
	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";



private:
	//UCollision* BodyCollision;


	UImageRenderer* MonsterRenderer = nullptr;

	void MoveUpdate(float _DeltaTime);
};

