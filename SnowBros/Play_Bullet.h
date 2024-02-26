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
	{
		BulletDir = _Dir;
	}
	EActorDir GetBulletDir()
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

	////�̰� �� ������ ������ ���ذ� �ȵǳ� 
	//��������������������������� �߰� ���߳� enumclass �ƿ� ��û��



	//������ƼŬ �Ϲ�
	void BulletNormal();
	//������ƼŬ �浹��
	void BulletCol();
	//���� ��ƼŬ �Ҹ�
	void BulletDestroy();

private:
	UCollision* BodyCollision = nullptr;
	UImageRenderer* BulletRenderer = nullptr;

	EActorDir BulletDir = EActorDir::None;


};





