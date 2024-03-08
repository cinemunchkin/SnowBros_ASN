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
	Bullet,
	Snowball
};



enum class SnowBrosCollisionOrder
{
	PlayerAttack,
	Monster,
	Player,
	Bullet,
	Snowball
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
	Right,
	Mid
};

enum class ESnowballState
{
	None,
	Snow1,
	Snow2,
	Snow3,
	Snow4,
	SnowBall
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
	Fly,

	PlayerRolling,
	PlayerPush,
	
};

enum class EMonsterState
{
	None,
	MonMove,
	MonIdle,
	Jump,
	DownJump,
	Snowball,
	Rolling,
	MonFlying,

};



enum class EBulletState
{	None,
	Bullet,
	BulletCol

};


// ���� :
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

