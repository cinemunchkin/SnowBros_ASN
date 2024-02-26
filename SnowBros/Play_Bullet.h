#pragma once
#include <EngineCore/Collision.h>
#include <EngineCore\Actor.h>
#include "SnowBros_Helper.h"

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
	
	
	

	//因維督銅適 号狽竺舛
	FVector AddBulletDir(FVector _Dir)
	{
		BulletDir += _Dir;
	}
	void SetBulletDir(const FVector _Dir)
	{
		BulletDir = _Dir;
	}
	FVector GetBulletDir()
	{
		return BulletDir;
	}



protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DirCheck();

	std::string GetAnimationName(std::string _Name);



	EBulletDir BulletDirState = EBulletDir::Right;

	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;
	void StateChange(EBulletState _State);

	////戚惟 訊 庚薦亜 説枇猿 戚背亜 照鞠革 
	//けずけずけずけずけずけ伯希研 蓄亜 照梅革 enumclass 焼神 雇短戚



	//因維督銅適 析鋼
	void BulletNormal();
	//因維督銅適 中宜獣
	void BulletCol();
	//因維 督銅適 社瑚
	void BulletDestroy();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;

	FVector BulletDir = FVector::Zero;


};





