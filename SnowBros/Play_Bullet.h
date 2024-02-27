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
	
	
	

	//������ƼŬ ���⼳��
	/*FVector AddBulletDir(FVector _Dir)
	{
		BulletDir += _Dir;
	}*/
	void SetBulletDir(EActorDir _Dir)
		//(Player->DirState);
	{
		BulletDir = _Dir;
	}
	EActorDir GetBulletDir()
	{
		return BulletDir;
	}

	FVector Dir = FVector::Left;


protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	

	std::string GetAnimationName(std::string _Name);



	EBulletDir BulletDirState = EBulletDir::Left;

	std::string CurAnimationName = "None";

	EBulletState State = EBulletState::None;
	void StateChange(EBulletState _State);

	////�̰� �� ������ ������ ���ذ� �ȵǳ� 
	//��������������������������� �߰� ���߳� enumclass �ƿ� ��û��



	//������ƼŬ �Ϲ�
	void Bullet();
	//������ƼŬ �浹��
	void BulletCol();
	//���� ��ƼŬ �Ҹ�
	void BulletDestroy();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;
	EActorDir BulletDir = EActorDir::None;



};




