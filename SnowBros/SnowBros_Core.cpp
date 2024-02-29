#include "SnowBros_Core.h"
#include "Level_Title.h"
#include "Level_Play.h"
#include "Level_End.h"
#include <EngineBase\EngineDirectory.h>
#include <EngineBase\EngineFile.h>
#include <EngineCore\EngineResourcesManager.h>
#include "SnowBros_Helper.h"

USnowBros_Core::USnowBros_Core()
{
}

USnowBros_Core::~USnowBros_Core()
{
}

void USnowBros_Core::BeginPlay()
{
	// 800 / 600�� �ʹ� ����.
	// MainWindow.SetWindowScale({1280, 720});

	// �߿��Ѱ� ũ�Ⱑ �ƴ϶� ����
	//MainWindow.SetWindowPosition({500, 100});
	//                         // ���� ���� �����ϰ� �������� �����
	MainWindow.SetWindowScale({ 1440 / 2/* * 1.5f*/, 996 / 2/* * 1.5f*/ });
	MainWindow.SetWindowPosition({ 200/* * 1.5f*/, 0/* * 1.5f*/ });



	UEngineCore::BeginPlay();

	// D:\Project\GM\WIn\App
	UEngineDirectory NewDir;

	// D:\Project\GM\WIn
	NewDir.MoveParent();

	// D:\Project\GM\WIn\ContentsResources
	NewDir.Move("SnowBros_Resources");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	// �������� ��Ģ�� ���Ұų�.
	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}
	

	//�÷��̾� �ִϸ��̼�
	///����
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Jump_R.png", 8, 1);

	///Idle
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Idle_R.png", 1, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Idle_L.png", 1, 1);

	///�޸���
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Run_R.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Run_L.png", 4, 1);

	///����
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Attack_R.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Attack_L.png", 4, 1);


	///����
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Melt.png", 7, 1);

	
	//���� �ִϸ��̼�
	///�⺻
	UEngineResourcesManager::GetInst().CuttingImage("Monster_01_R.png", 6, 4);
	UEngineResourcesManager::GetInst().CuttingImage("Monster_01_L.png", 6, 4);

	//����캼
	UEngineResourcesManager::GetInst().CuttingImage("Snowball_01_R.png", 5, 1);
	


	//��Ÿ �ִϸ��̼�
	///���� ��ƼŬ
	////�⺻
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Bullet_R.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Bullet_L.png", 2, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_BulletCol_R.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_BulletCol_L.png", 7, 1);

	
	// 
	// 
	// 
	// ���ν��� ���⼭ �ε��Ҽ��� �ִ�.

	// �츮�� �����ϴ°� Level
	CreateLevel<ULevel_Title>("Title");
	CreateLevel<ULevel_Play>("Play");
	CreateLevel<ULevel_End>("End");


	ChangeLevel("Play");
}

void USnowBros_Core::Tick(float _DeltaTime)
{
	UEngineCore::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_F2))
	{
		GEngine->EngineDebugSwitch();
	}
}




