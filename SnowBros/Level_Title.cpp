#include "Level_Title.h"
#include "Title_Map.h"
#include "EngineCore\EngineCore.h"

ULevel_Title::ULevel_Title() 
{
}

ULevel_Title::~ULevel_Title() 
{
}

void ULevel_Title::BeginPlay()
{
	ULevel::BeginPlay();

	{
		// ���⼭ ��ġ�� ���������ٰ� �̳༮��ġ�� 0,0�ΰ� �ƴ���
		ATitle_Map* Logo = SpawnActor<ATitle_Map>();

	}


}
void ULevel_Title::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (UEngineInput::IsAnykeyDown())
	{
		GEngine->ChangeLevel("Play");
	}


	
}
void ULevel_Title::LevelStart(ULevel* _Level)
{
	int a = 0;
}
void ULevel_Title::LevelEnd(ULevel* _Level)
{
	int a = 0;
}