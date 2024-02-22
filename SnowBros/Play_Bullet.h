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

	
	
	void Bullet();
	void Bullet_Destroy();

	FVector AddDirBullet(float _Dir);
	FVector GetDirBullet();

	FVector Dir = FVector::Zero;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DirCheck();

	std::string GetAnimationName(std::string _Name);



	EActorDir DirState = EActorDir::Right;
	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;
	void StateChange(EBulletState _State);


	void BulletFire();
	void BulletCol();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;


};





