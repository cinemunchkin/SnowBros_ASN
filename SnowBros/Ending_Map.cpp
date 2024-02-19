#include "Ending_Map.h"
#include "SnowBros_Helper.h"
#include <EngineCore\EngineResourcesManager.h>




AEnding_Map::AEnding_Map()
{
}

AEnding_Map::~AEnding_Map()
{
}


void AEnding_Map::BeginPlay()
{
	//
	//Renderer->SetImage("Title_Ending_01.png");
	//// 이미지가 나올 위치
	//SetActorLocation({ 720 / 2, 498 / 2 });
	//Renderer->SetTransform({ {0,0}, {1440 / 2,996 / 2} });
	//Renderer->SetImageCuttingTransform({ {0,0}, {1440, 996/*720/2,498/2*/} });


	Renderer = CreateImageRenderer(SnowBrosRenderOrder::Map);

	//Renderer = CreateImageRenderer(SnowBrosRenderOrder::Map);
}

void AEnding_Map::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	Destroy();

	int a = 0;
}

void AEnding_Map::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale });

}

