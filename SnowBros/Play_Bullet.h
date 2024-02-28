#pragma once
#include <EngineCore/Collision.h>
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"
#include <EngineBase/Transform.h>

class APlay_Bullet :public UCollision , public AActor
{
	
public:
	// constrcuter destructer
	APlay_Bullet();
	~APlay_Bullet();

	// delete Function
	APlay_Bullet(const APlay_Bullet& _Other) = delete;
	APlay_Bullet(APlay_Bullet&& _Other) noexcept = delete;
	APlay_Bullet& operator=(const APlay_Bullet& _Other) = delete;
	APlay_Bullet& operator=(APlay_Bullet&& _Other) noexcept = delete;
	
	
	

	//공격파티클 방향설정
	/*FVector AddBulletDir(FVector _Dir)
	{
		BulletDir += _Dir;
	}*/
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
	void BulletColState(EBulletState _State);


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	

	void StateChange(EBulletState _State);
	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);



	EActorDir BulletDirState = EActorDir::Left;

	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;

	////이게 왜 문제가 잡힐까 이해가 안되네 
	//ㅁㅊㅁㅊㅁㅊㅁㅊㅁㅊㅁ헤더를 추가 안했네 enumclass 아오 멍청이



	//공격파티클 일반
	void Bullet();
	//공격파티클 충돌시
	void BulletCol();
	//공격 파티클 소멸
	void BulletDestroy();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;
	EActorDir BulletDir = EActorDir::Right;



};





