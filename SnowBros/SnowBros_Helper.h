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
	None,
	Left,
	Right,
	Jump
};

enum class EMonsterDir
{
	None,
	Left,
	Right
};




enum class EPlayState
{
	None,
	CameraFreeMove,
	FreeMove,
	
	Idle,
	Run,
	Attack,
	Jump,
	DownJump,
	
	Strobe,
	FastRun,
	Fly
};

enum class EMonsterState
{
	None,
	Idle,
	Jump,
	DownJump,
	SnowBall,
	Rolling


};



enum class EBulletState
{	None,
	Bullet,
	BulletCol

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

