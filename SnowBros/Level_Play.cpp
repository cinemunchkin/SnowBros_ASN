#include "Level_Play.h"
#include "SnowBros_Player.h"
#include <EngineCore\EngineResourcesManager.h>
#include <EngineBase\EngineDirectory.h>
#include <EngineBase\EngineFile.h>

ULevel_Play::ULevel_Play()
{
}

ULevel_Play::~ULevel_Play()
{
}


void ULevel_Play::BeginPlay()
{
	// �����ΰ� ���;� �Ѵٰ� �����մϴ�.
	UEngineDirectory NewPath;

	// NewPath.Move("ContentsResources");
	NewPath.MoveParent();

	NewPath.Move("ContentsResources");
	NewPath.Move("Texture");


	// Ȯ���ڵ� �������� ��ҹ��� ������ ������ �빮�ڷ� �ٲ㼭 ã�����̴�..
	std::list<UEngineFile> AllFileList = NewPath.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : AllFileList)
	{
		std::string FullPath = File.GetFullPath();
		// �̱��� ���� ����� �Ϻη� GetInst�� ����ϰڽ��ϴ�.
		UEngineResourcesManager::GetInst().LoadImg(FullPath);
	}

	// CherryBomb_0.png

	// ULevel* const This = this;
	this->SpawnActor<SnowBros_Player>();

	// SpawnActor<SnowBros_Player>();

	// ������ 
	// �÷��̾�
	// ���
	// ����
	// ������ �ڵ�� ���⼭ �� ������ �Ѵ�.
}