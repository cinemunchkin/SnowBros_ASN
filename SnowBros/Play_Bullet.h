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
	
	bool IsBulletCol = false;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	

	void StateChange(EBulletState _State);
	std::string GetAnimationName(std::string _Name);
	std::string GetAnimationFullName(std::string _Name);



	EActorDir BulletDirState = EActorDir::Left;

	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;

	////�̰� �� ������ ������ ���ذ� �ȵǳ� 
	//��������������������������� �߰� ���߳� enumclass �ƿ� ��û��



	//������ƼŬ �Ϲ�
	void Bullet(float _DeltaTime);
	//������ƼŬ �浹��
	void BulletCol(float _DeltaTime);
	//���� ��ƼŬ �Ҹ�
	

	void BulletPhysics(float _DeltaTime);

	void BulletStart();
	void BulletColStart();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;
	EActorDir BulletDir = EActorDir::Right;



};





