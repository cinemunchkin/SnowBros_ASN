#include "Ending_Map.h"
#include "SnowBros_Helper.h"
#include <EngineCore\EngineResourcesManager.h>




AEnding_Map::AEnding_Map()
{
}

AEnding_Map::~AEnding_Map()
{
}

void AEnding_Map::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}



void AEnding_Map::BeginPlay()
{
	
	Renderer = CreateImageRenderer(SnowBrosRenderOrder::Map);


	{
		UImageRenderer* Renderer = CreateImageRenderer();
		Renderer->SetImage("Title_Ending_01.png");

		SetActorLocation({ 720 / 2,498 / 2 });
		Renderer->SetTransform({ {0,0}, {1440 / 2,996 / 2} });
		Renderer->SetImageCuttingTransform({ {0,0}, {1440, 996/*720/2,498/2*/} });


	}




}

void AEnding_Map::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	int a = 0;
}

