#pragma once
#include <EngineCore/Collision.h>
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"
#include <EngineBase/Transform.h>


class APlay_Monster;

class APlay_Bullet : public AActor
{
	friend APlay_Monster;
public:
	// constrcuter destructer
	APlay_Bullet();
	~APlay_Bullet();

	// delete Function
	APlay_Bullet(const APlay_Bullet& _Other) = delete;
	APlay_Bullet(APlay_Bullet&& _Other) noexcept = delete;
	APlay_Bullet& operator=(const APlay_Bullet& _Other) = delete;
	APlay_Bullet& operator=(APlay_Bullet&& _Other) noexcept = delete;
	
	
	

	void SetBulletDir(EActorDir _Dir)
	{
		BulletDir = _Dir;
	}
	EActorDir GetBulletDir()
	{
		return BulletDir;
	}

	FVector Dir = FVector::Left;

	void SetAnimation(std::string _Name);
	
	bool IsBulletBomb = false;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	

	void StateChange(EBulletState _State);
	void StateUpdate(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);



	EActorDir BulletDirState = EActorDir::Left;

	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;

	FVector BulletMoveVector = FVector::Zero;
	FVector BulletGravityVector = FVector::Zero;
	FVector BulletGravityAcc = FVector::Down * 1000.0f;
	FVector TotalLastMoveVector = FVector::Zero;


	////이게 왜 문제가 잡힐까 이해가 안되네 
	//ㅁㅊㅁㅊㅁㅊㅁㅊㅁㅊㅁ헤더를 추가 안했네 enumclass 아오 멍청이



	//공격파티클 일반
	void Bullet(float _DeltaTime);
	//공격파티클 충돌시
	void BulletBomb(float _DeltaTime);
	

	bool BulletColCheck(float _DeltaTime);
	void BulletMoveUpdate(float _DeltaTime);
	void BulletGravityCheck(float _DeltaTime);
	void BulletLastMoveVector(float _DeltaTime);



	void BulletStart();
	void BulletBombStart();


private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;
	EActorDir BulletDir = EActorDir::Right;



};





