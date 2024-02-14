#pragma once
#include <EngineBase\EngineMath.h>
#include <EngineCore\EngineCore.h>
#include <EngineCore\ImageRenderer.h>

enum SnowBrosRenderOrder
{
	Map,
	Monster,
	Player
};

enum class EActorDir
{
	Left,
	Right,
	Jump
};

enum class EPlayState
{
	None,
	CameraFreeMove,
	FreeMove,
	Idle,
	Move,
	Jump,
};



// Ό³Έν :
class USnowBros_Helper
{
public:
	static UWindowImage* ColMapImage;
	static FVector GetMousePos();

private:
	// constrcuter destructer
	USnowBros_Helper();
	~USnowBros_Helper();
};

