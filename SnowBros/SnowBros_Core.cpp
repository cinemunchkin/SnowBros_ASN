#include "SnowBros_Core.h"
#include "Level_Title.h"
#include "Level_Play.h"
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
	// 800 / 600은 너무 작죠.
	// MainWindow.SetWindowScale({1280, 720});

	// 중요한건 크기가 아니라 비율
	//MainWindow.SetWindowPosition({500, 100});
	//                           1024                960
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

	// 엔진만의 규칙을 정할거냐.
	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}
	

	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Jump_R.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Run_R.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Monster_01.png", 6, 4);

	//UEngineResourcesManager::GetInst().CuttingImage("SnowBros_Test_char.png", 4, 1);
	//UEngineResourcesManager::GetInst().LoadFolder(NewDir.AppendPath("PlayLevel"));

	// 리로스를 여기서 로드할수도 있다.

	// 우리가 제공하는건 Level
	CreateLevel<ULevel_Title>("Title");
	CreateLevel<ULevel_Play>("Play");
	ChangeLevel("Title");
}

void USnowBros_Core::Tick(float _DeltaTime)
{
	UEngineCore::Tick(_DeltaTime);
}


