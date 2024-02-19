#include "Level_End.h"
#include "Ending_Map.h"
#include <EngineCore\EngineCore.h>

ULevel_End::ULevel_End()
{
}

ULevel_End::~ULevel_End()
{
}


void ULevel_End::BeginPlay()
{
	ULevel::BeginPlay();

	AEnding_Map* EMap = SpawnActor<AEnding_Map>();
	EMap->SetMapImage("SnowBros_Lv_1.png");
	
	//AEnding_Map* EndingMap = SpawnActor<AEnding_Map>();
	//EndingMap->SetMapImage("Title_Ending_01.png");

	
}

void ULevel_End::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	
	
}

void ULevel_End::LevelStart(ULevel* _Level)
{
	int a = 0;
}
void ULevel_End::LevelEnd(ULevel* _Level)
{
	int a = 0;
}