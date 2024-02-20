#include "Level_Play.h"
#include "Play_Player.h"
#include "Play_Map.h"
#include <EngineCore\EngineCore.h>
#include "Play_Monster.h"

ULevel_Play::ULevel_Play()
{
}

ULevel_Play::~ULevel_Play()
{
}

void ULevel_Play::BeginPlay()
{
	ULevel::BeginPlay();

	APlay_Map* Map = SpawnActor<APlay_Map>();
	Map->SetMapImage("SnowBros_Lv_1.png");
	Map->SetColMapImage("SnowBros_Lv_1_Col.png");
	
	// Map->SwitchDebug();

	//APlay_UIMap* UIMap = SpawnActor<APlay_UIMap>();
	//UIMap->SetMapImage("SnowBros_UI.png");
 
	 {
		 APlay_Player* Player = SpawnActor<APlay_Player>(); // �ƿ� �̰� ��� APlay_Map���� ������
		 Player->SetActorLocation({ 100, 300 });
		 Player->SetName("Player");
	 }

	 {
		 APlay_Monster* Monster = SpawnActor<APlay_Monster>();
		 Monster->SetName("Monster");
		 Monster->SetActorLocation({ 100, 150 });
	 }



			// ���͵���, �÷��̾� ���� ����

}

void ULevel_Play::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (UEngineInput::IsDown('E'))
	{
		GEngine->ChangeLevel("End");
	
		

	}

	

}

void ULevel_Play::LevelStart(ULevel* _Level)
{
	// ���⼭ ���ҽ��� �ε��ϰ�
	// ���͵� ���⼭ �����
	int a = 0;
}
void ULevel_Play::LevelEnd(ULevel* _Level)
{
	// ���ҽ��� ������.
	// ���͸� �����Ѵ�.
	int a = 0;
}