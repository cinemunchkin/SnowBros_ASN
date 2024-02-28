#include "Play_Monster.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include <EngineCore/SceneComponent.h>
APlay_Monster::APlay_Monster()
{
}

APlay_Monster::~APlay_Monster()
{
}

void APlay_Monster::BeginPlay()
{
	AActor::BeginPlay();
	

	{
		//UImageRenderer* MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);

		MonsterRenderer = CreateImageRenderer(SnowBrosRenderOrder::Monster);
		MonsterRenderer->SetTransform({ {0,0}, {48, 48} });
		MonsterRenderer->SetImage("Monster_01_R.png");
		MonsterRenderer->SetImage("Monster_01_L.png");
	}
	
	{
		MonsterRenderer->CreateAnimation("Idle_Right", "Monster_01_R.png", 0, 5, 0.1f, true);
		MonsterRenderer->CreateAnimation("Idle_Left", "Monster_01_L.png", 0, 5, 0.1f, true);

		MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_01_R.png", 12, 23, 0.1f, true);
		MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_01_L.png", 12, 23, 0.1f, true);

		StateChange(EMonsterState::Idle);
	}


	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
		BodyCollision->SetColType(ECollisionType::Rect);
		BodyCollision->SetScale({ 80, 80 });
	}



	
}

void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	std::vector<UCollision*> Result;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, Result))
	{// 만약에 플레이어와 collisioncheck를 했을때, true면

		/*BodyCollision->SetActive(true, 0.1f);
		APlay_Player* Player = GetActorLocation();

		*/StateChange(EMonsterState::SnowBall);
		Destroy();

		
	}


	APlay_Player* Player = APlay_Player::GetMainPlayer();

	if (nullptr == Player)
	{
		MsgBoxAssert("플레이어가 존재하지 않습니다.");
	}

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//몬스터 쫓아다니는 함수
	FVector MonsterDir = PlayerPos - MonsterPos;
	 MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	AddActorLocation(MonsterDirNormal * _DeltaTime * 150.0f);
	//----


	// 플레이어를 알아야 한다.
}







void APlay_Monster::DirCheck() //다시 설정 필요
{
	EActorDir Dir = DirState;

/*	if (UEngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

	*/

	if (Dir != DirState)
	{
		DirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		MonsterRenderer->ChangeAnimation(Name, true, MonsterRenderer->GetCurAnimationFrame(), MonsterRenderer->GetCurAnimationTime());
		//특정 프레임입력 => 애니메이션 전체가 아니라, 특정 프레임 넘버만 애니메이션. 
		MonsterRenderer->ChangeAnimation(Name);
	}
}



std::string APlay_Monster::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (DirState)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;

}



void APlay_Monster::StateChange(EMonsterState _State)
{

	if (State != _State)
	{
		switch (_State)
		{
		case EMonsterState::Idle:
			IdleStart();
			break;
		case EMonsterState::Jump:
			JumpStart();
			break;
		case EMonsterState::DownJump:
			DownJumpStart();
			break;
		case EMonsterState::SnowBall:
			SnowballStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}


void APlay_Monster::IdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
}


void APlay_Monster::JumpStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Jump"));
	DirCheck();
}


void APlay_Monster::DownJumpStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("DownJump"));
	DirCheck();
}


void APlay_Monster::SnowballStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	DirCheck();
}



void APlay_Monster::Idle(float _DeltaTime)
{


}



void APlay_Monster::Physics(float _DeltaTime)
{// 중력이랑 이동함수
	
	
	std::vector<UCollision*> ColResult;

	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, ColResult))
		//this(몬스터)가  player 와 충돌했을때
		// 몬스터랑 플레이어랑 부딪혔을때
	{
		UCollision* Collision = ColResult[0];
		AActor* ColPtr = Collision->GetOwner();

		APlay_Player* Player = dynamic_cast<APlay_Player*>(ColPtr);
		FVector PlayerPos = Player->GetActorLocation(); // 플레이어의 현재 포지션
		FVector MonPos = GetActorLocation(); // 몬스터의 현재 위치
		//바꾸기
		if (PlayerPos.X - 10 < MonPos.X  && PlayerPos.X+10 > MonPos.X ) // 
			// 플레이어.x의 -10보다 크거나 
		{
			Player->PlayerColState(EPlayState::Strobe);

			return;
		}
		
	}
	if(true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, ColResult))
	{

		UCollision* Collision = ColResult[0];
		AActor* ColPtr = Collision->GetOwner();

		APlay_Bullet* Bullet = dynamic_cast<APlay_Bullet*>(ColPtr);
		FVector BulletPos = Bullet->GetActorLocation(); // 플레이어의 현재 포지션
		FVector MonPos = GetActorLocation(); // 몬스터의 현재 위치
		
		if (BulletPos.X - 5 < MonPos.X && BulletPos.X + 5 > MonPos.X)
		{
			StateChange(EMonsterState::SnowBall);
			Bullet->BulletColState(EBulletState::BulletCol);


		}

		//BodyCollision->Destroy();-> 총알하고 충돌했을 때 이거 가져가자
		//StateChange(EMonsterState::SnowBall); -> 총알하고 충돌했을 때 이거 가져가자
	}

	/*
	
	if (true == BodyCollision->CollisionCheck(MarioCollisionOrder::Player, Result))
	{
		// 이런식으로 상대를 사용할수 있다.
		UCollision* Collision = Result[0];
		AActor* Ptr = Collision->GetOwner();
		Mario* Player = dynamic_cast<Mario*>(Ptr);
		FVector CurPlayerLocation = Player->GetActorLocation();

		FVector CurLocation = GetActorLocation();
		if (CurPlayerLocation.Y < CurLocation.Y - 32) {
			Player->SetState(MarioState::Interactive);
			BodyCollision->Destroy();
			SetMonsterState(MonsterState::Dead);
			return;
		}
		else {
			Player->Hit();
			return;
		}
	}

	std::vector<UCollision*> MonsterResult;
	if (true == BodyCollision->CollisionCheck(MarioCollisionOrder::Monster, MonsterResult))
	{
		for (UCollision* Collision : MonsterResult) {
			this->ReverseDir();
		}
	}
	*/




}