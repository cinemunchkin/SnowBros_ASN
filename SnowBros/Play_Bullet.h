#pragma once
#include <EngineCore/Collision.h>
#include <EngineCore\Actor.h>

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
	FVector AddBulletDir(FVector _Dir)
	{
		Dir += _Dir;
	}
	void SetBulletDir(const FVector _Dir)
	{
		Dir = _Dir;
	}
	FVector GetBulletDir()
	{
		return Dir;
	}



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DirCheck();

	std::string GetAnimationName(std::string _Name);



	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;
	void StateChange(EBulletState _State);


	//공격파티클 일반
	void BulletNormal();
	//공격파티클 충돌시
	void BulletCol();
	//공격 파티클 소멸
	void BulletDestroy();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;

	FVector Dir = FVector::Zero;


};





