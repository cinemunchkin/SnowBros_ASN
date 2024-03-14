#pragma once
#include <EngineBase\EngineMath.h>
#include <EngineCore\EngineCore.h>
#include <EngineCore\ImageRenderer.h>
#include <EngineCore\Actor.h>

enum class SnowBrosRenderOrder
{
	Map,
	UI,
	Monster,
	Player,
	Snowball,
	Bullet,
	Item,
};



enum class SnowBrosCollisionOrder
{
	PlayerAttack,
	Monster,
	Player,
	Bullet,
	Snowball,
	Item,
	Rolling
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
	Left = -1,
	Right = 1,
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
	PlayerStuck
	
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
	SnowBomb

};

enum class EItemState
{
	None,
	ItemSushi,
	ItemScore,
	

};



enum class EBulletState
{	None,
	Bullet,
	BulletBomb

};

enum class EUIState
{
	None,
	TotalScore,
	PlayerScore,
	PlayerLife
};


// Ό³Έν :
class USnowBros_Helper
{
public:
	static UWindowImage* ColMapImage;
	//static AActor* ActorPos; 
	static FVector GetMousePos();

	static int SnowbrosScore;

private:
	// constrcuter destructer
	USnowBros_Helper();
	~USnowBros_Helper();
};

