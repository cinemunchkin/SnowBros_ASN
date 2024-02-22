#pragma once
#include <EngineBase\EngineMath.h>
#include <EngineCore\EngineCore.h>
#include <EngineCore\ImageRenderer.h>
#include <EngineCore\Actor.h>

enum class SnowBrosRenderOrder
{
	Map,
	Monster,
	Player,
	Bullet
};



enum class SnowBrosCollisionOrder
{
	PlayerAttack,
	SnowBall,
	Monster,
	Player,
	Bullet
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
	Run,
	Jump,
	DownJump,
	Strobe,
	Attack,
	Fly
};

enum class EBulletState
{	None,
	NormalAttack,
	ColAttack

};


// Ό³Έν :
class USnowBros_Helper
{
public:
	static UWindowImage* ColMapImage;
	//static AActor* ActorPos; 
	static FVector GetMousePos();

private:
	// constrcuter destructer
	USnowBros_Helper();
	~USnowBros_Helper();
};

