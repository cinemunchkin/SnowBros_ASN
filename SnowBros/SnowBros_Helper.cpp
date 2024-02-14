#include "SnowBros_Helper.h"

UWindowImage* USnowBros_Helper::ColMapImage = nullptr;

USnowBros_Helper::USnowBros_Helper()
{
}

USnowBros_Helper::~USnowBros_Helper()
{
}

FVector USnowBros_Helper::GetMousePos()
{
	return GEngine->MainWindow.GetMousePosition();
}