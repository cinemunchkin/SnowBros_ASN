#include <EnginePlatform/EngineInput.h>
#include <EngineBase\EngineDebug.h>
#include <EngineCore/SceneComponent.h>
#include <iostream>
#include "Play_Item.h"
#include "Play_Monster.h"
#include "Play_Bullet.h"
#include "Play_Player.h"
#include "Play_SnowBall.h"

#include <vector>
#include <string>


/*
Random
UEngineRandom
*/



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
		MonsterRenderer->SetTransform({ {0,-26}, {48 * 1.3f, 48 * 1.3f} });

		//���� �⺻ �ִϸ��̼�
		MonsterRenderer->SetImage("Monster_01_R.png");
		MonsterRenderer->SetImage("Monster_01_L.png");

		//���� ��Ÿ �ִϸ��̼�
		MonsterRenderer->SetImage("Monster_02_R.png");
		MonsterRenderer->SetImage("Monster_02_L.png");

		MonsterRenderer->SetImage("MonFlying_01_L.png");
		MonsterRenderer->SetImage("Monster_Item_01.png");

		MonsterRenderer->SetImage("SnowBomb_01.png");

	}



	{
		BodyCollision = CreateCollision(SnowBrosCollisionOrder::Monster);
		BodyCollision->SetPosition(MonsterRenderer->GetPosition());
		BodyCollision->SetColType(ECollisionType::Rect);
		BodyCollision->SetScale({ 80, 80 });
	}


	{
		MonsterRenderer->CreateAnimation("MonIdle", "Monster_01_R.png", 6, 6, 0.1f, true);

		// ���� �⺻ ; �̵�
		MonsterRenderer->CreateAnimation("MonMove_Right", "Monster_01_R.png", 0, 5, 0.1f, true);
		MonsterRenderer->CreateAnimation("MonMove_Left", "Monster_01_L.png", 0, 5, 0.1f, true);

		//�� �ӿ� ������ ������ �Ƶչٵ�
		MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_02_R.png", 8, 9, 0.07f, true);
		MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_02_L.png", 5, 6, 0.07f, true);
		//MonsterRenderer->CreateAnimation("Snowball_Right", "Monster_01_R.png", 7, 9, 0.05f, true);
		//MonsterRenderer->CreateAnimation("Snowball_Left", "Monster_01_L.png", 8, 10, 0.05f, true);


		//���� ����
		MonsterRenderer->SetTransform({ {0,-26}, {48 * 1.3f, 48 * 1.3f} });
		MonsterRenderer->CreateAnimation("Jump_Left", "Monster_01_L.png", 10, 11, 0.5f, true);
		MonsterRenderer->CreateAnimation("Jump_Right", "Monster_01_R.png", 6, 7, 0.5f, true);



		// ���Ͱ� ����캼�� �¾Ƽ� ���ư��� 
		MonsterRenderer->CreateAnimation("MonFlying_Right", "MonFlying_01_L.png", 0, 3, 0.1f, true);
		MonsterRenderer->CreateAnimation("MonFlying_Left", "MonFlying_01_L.png", 0, 3, 0.1f, true);

		//�� ���� //�� �Ʒ��� ���� ������ ����
		MonsterRenderer->CreateAnimation("SnowBomb_Right", "SnowBomb_01.png", 0, 3, 0.05f, true);
		MonsterRenderer->CreateAnimation("SnowBomb_Left", "SnowBomb_01.png", 0, 3, 0.05f, true);
	}



	{
		SnowBallRenderer = CreateImageRenderer(SnowBrosRenderOrder::Snowball);
		SnowBallRenderer->SetImage("Snowball_01_R.png");
		SnowBallRenderer->SetImage("Rolling_01_R.png");

		SnowBallRenderer->SetTransform({ { +6,-38 }, { 78 * 1.15f,66 * 1.15f } });



		//�� ���� //snowballstack
		SnowBallRenderer->CreateAnimation("SnowStack_Right", "Snowball_01_R.png", 0, 4, 0.5f, true);
		SnowBallRenderer->CreateAnimation("SnowStack_Left", "Snowball_01_R.png", 0, 4, 0.5f, true);

		//�� ���� //snowballrolling
		SnowBallRenderer->CreateAnimation("Rolling_Right", "Rolling_01_R.png", 0, 3, 0.1f, true);
		SnowBallRenderer->CreateAnimation("Rolling_Left", "Rolling_01_R.png", 0, 3, 0.1f, true);


		SnowBallRenderer->ActiveOff();// SnowBallRender�� ó���� Off�صΰ�
	}


	StateChange(EMonsterState::MonMove);

}


void APlay_Monster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	APlay_Player* Player = APlay_Player::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("�÷��̾ �������� �ʽ��ϴ�.");
	}

	StateUpdate(_DeltaTime);
}





void APlay_Monster::DirCheck()
// 
{
	EMonsterDir Dir = MonsterDirState;
	PrevDir = Dir;
	APlay_Player* Player = APlay_Player::GetMainPlayer();

	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();


	if (PlayerPos.X > MonsterPos.X)
	{
		Dir = EMonsterDir::Right;
		MonsterDirState = Dir;
		return;
	}
	else
	{
		Dir = EMonsterDir::Left;
		MonsterDirState = Dir;
		return;
	}

	if (Dir != MonsterDirState)
	{
		MonsterDirState = Dir;
		std::string Name = GetAnimationName(CurAnimationName);

		MonsterRenderer->ChangeAnimation(Name, true, MonsterRenderer->GetCurAnimationFrame(), MonsterRenderer->GetCurAnimationTime());
		//Ư�� �������Է� => �ִϸ��̼� ��ü�� �ƴ϶�, Ư�� ������ �ѹ��� �ִϸ��̼�. 
		MonsterRenderer->ChangeAnimation(Name);
	}

}



std::string APlay_Monster::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		DirName = "_Left";
		break;
	case EMonsterDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;

}



void APlay_Monster::StateChange(EMonsterState _State)
{//�ִϸ��̼�

	if (State != _State)
	{
		switch (_State)
		{
		case EMonsterState::MonIdle:
			MonIdleStart();
			break;
		case EMonsterState::MonMove:
			MonMoveStart();
			break;
		case EMonsterState::Jump:
			JumpStart();
			break;
		case EMonsterState::DownJump:
			DownJumpStart();
			break;
		case EMonsterState::Snowball:
			SnowballStart();
			break;
		case EMonsterState::Rolling:
			RollingStart();
			break;
		case EMonsterState::MonFlying:
			MonFlyingStart();
			break;
		case EMonsterState::SnowBomb:
			SnowBombStart();
			break;
		default:
			break;
		}
	}

	State = _State;
}



void APlay_Monster::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EMonsterState::MonIdle:
		MonIdle(_DeltaTime);
		break;
	case EMonsterState::MonMove:
		MonMove(_DeltaTime);
		break;
	case EMonsterState::Jump:
		Jump(_DeltaTime);
		break;
	case EMonsterState::DownJump:
		DownJump(_DeltaTime);
		break;
	case EMonsterState::Snowball:
		Snowball(_DeltaTime);
		break;
	case EMonsterState::Rolling:
		Rolling(_DeltaTime);
		break;
	case EMonsterState::MonFlying:
		MonFlying(_DeltaTime);
		break;
	case EMonsterState::SnowBomb:
		SnowBomb(_DeltaTime);
		break;


	default:
		break;
	}

}




void APlay_Monster::MonIdleStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonIdle"));
	DirCheck();

}

void APlay_Monster::MonMoveStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonMove"));
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


void APlay_Monster::RollingStart()
{
	SnowCollision = CreateCollision(SnowBrosCollisionOrder::Snowball);
	/*SnowCollision->SetPosition(MonsterRenderer->GetPosition());
	SnowCollision->SetColType(ECollisionType::Rect);*/

	BodyCollision->Destroy();
	// ������ �����ϸ�,  Monster�� BodyCollision destroy
	SnowBallRenderer->ChangeAnimation(GetAnimationName("Rolling"));
	DirCheck();
}


void APlay_Monster::MonFlyingStart()
{
	MonsterRenderer->ChangeAnimation(GetAnimationName("MonFlying"));
	DirCheck();

}

void APlay_Monster::SnowBombStart()
{

	int n = MonsterDir.X;
	int reverse = 0;
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		reverse = (n + 20) * -1;
		break;
	case EMonsterDir::Right:
		reverse = (n + 20) * +1;
		break;
	default:
		break;
	}

	MonsterRenderer->SetTransform({ {reverse,-28}, {48 * 1.7f, 48 * 1.7f} });
	MonsterRenderer->ChangeAnimation(GetAnimationName("SnowBomb"));
	DirCheck();
}





void APlay_Monster::SnowballStart()
{

	MonsterRenderer->SetTransform({ {0,-45}, {48 * 1.4f, 48 * 1.4f} });
	MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
	SnowBallRenderer->SetActive(true); // Begin�Ҷ��� off�صξ��ٰ� 

	DirCheck();
}


void APlay_Monster::MonIdle(float _DeltaTime)
{
	if (true == BulletColMonCheck(_DeltaTime))
	{
		return;
	}

}

void APlay_Monster::MonMove(float _DeltaTime)
{

	if (true == BulletColMonCheck(_DeltaTime))
	{
		StateChange(EMonsterState::Snowball);
		return;
	}

	DirCheck();
	ColMoveUpdate(_DeltaTime);
	//SetAnimation("Idle_Left");
	MoveCheck(_DeltaTime);
	//SetAnimation("Idle_Right");


	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector PlayerPos = Player->GetActorLocation();
	FVector MonsterPos = GetActorLocation();

	//FVector MonsterDir = MonsterDIrState
	// MonsterDir -> case�� �ٲٱ�
	MonsterDir.Y = 0.0f;
	FVector MonsterDirNormal = MonsterDir.Normalize2DReturn();

	//	AddActorLocation(FVector::Right * MonsterDirNormal * MoveAcc * _DeltaTime);

	if (MonsterDirState == EMonsterDir::Left)
	{
		SetAnimation("MonMove_Left");
		AddActorLocation(FVector::Left * _DeltaTime * MoveAcc);
		return;
	}
	if (MonsterDirState == EMonsterDir::Right)
	{
		SetAnimation("MonMove_Right");
		AddActorLocation(FVector::Right * _DeltaTime * MoveAcc);
		return;
	}

	else if (MonsterDirState == EMonsterDir::None)
	{
		SetAnimation("MonIdle");
		return;
	}


}
//
//void APlay_Monster::MonFlying(float _DeltaTime)
//{
//	SnowBallRenderer->SetActive(false);
//	MonFlyingColVector(_DeltaTime);
//
//	AddActorLocation(FVector::Right * _DeltaTime*100);
//}


void APlay_Monster::MonFlying(float _DeltaTime)
{// 
	MonFlyingColVector(_DeltaTime);
	SnowBallRenderer->SetActive(false);
	//����� �°� ���ư� �� -> Snowstack ���̴� ������ ������!
	
	TotalGravity += GravitySpeed * _DeltaTime;
	TotalSpeed = JumpSpeed + TotalGravity;
	HorizonTotal += HorizonRight * _DeltaTime;
	int DirState = static_cast<int>(MonsterDirState);



	if (HorizonLeft.X + HorizonTotal.X>0)
	{ //�ƴ� ���밪�� ���ؾߵǤ��µ� �̰� �� ���ϰ� ���� 
			// 
		if (true == MonFlyingColVector(_DeltaTime))
		{

			MoveVector.X *= DirState;
			MoveVector.Y *= DirState;
			//MoveVector.X *= -1.0f;

		}
		else if (false == MonFlyingColVector(_DeltaTime))
		{
			//HorizonTotal -= HorizonRight * _DeltaTime;
			HorizonLeftTotal += HorizonLeft * _DeltaTime;
			
		}
		MonDeathCheck(_DeltaTime);

	}


	//if(TotalGravity.Y > JumpSpeed.Y)
	//{
	//	if (true == MonFlyingColVector(_DeltaTime))
	//	{
	//		MoveVector.X *= DirState;
	//		MoveVector.Y *= DirState;
	//		//MoveVector.X *= -1.0f;
	//	}
	//}

	//DirCheck();

	TotalXSpeed = HorizonTotal + HorizonLeftTotal;
	//�̷��� �Լ��� ��Ƶΰ� ����!!
	AddActorLocation((TotalXSpeed + TotalSpeed) * _DeltaTime);
	//return;


//


	/*HorizonRightTotal X�������� ���� total
	 == *_Deltatime���� ��� ������ ���� ( 0~ ) */
	                
	/*��ü �ӵ��� �׷� HorizonLeft�� ���ؼ� ���� �پ��Ե�
	������ �پ���� �ε����� �������� Left > Right �� �� ������!*/
	
	// TotalXSpeed => if MonFlyingColVector ( true = -1 / false = 1 )
	//	int DirState = static_cast<int>(MonsterDirState);
	//Left = -1,  Right = 1



	//FVector MonFlyingVector = MonsterPosA - MonsterPosB;
	//FVector MonNormal = MonFlyingVector.Normalize2DReturn();

	/*GravityVector += FlyingGravityAcc * _DeltaTime;*/
	//FVector FlyingVector = MonsterPosB + GravityVector;


	
	
	//FVector MonsterPosA = this->GetActorLocation();
	//FVector MonFlyingVector = { 1.0f, -2.0f, 0.0f, 0.0f };
	//FVector MonFlyingSpeed = MonFlyingVector*100.0f;

	

	//if (MonDeathTime >= 0)
	//{//0���� Ŭ ���� ��� ���̳ʽ�

	//	MonDeathTime -= _DeltaTime ;
	//	AddActorLocation(MonFlyingSpeed * _DeltaTime);
	//}
	//
	//else 
	//{// 0���� �۾�����
	//	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::CyanA);
	//	if (Color == Color8Bit(0, 255, 255, 0))
	//	{
	//		MonDeathCheck(_DeltaTime);
	//		return;
	//	}
	//	//MonDeathTime = 1.5f;

	//}

	//AddActorLocation(MonFlyingSpeed * _DeltaTime);


	
}

bool APlay_Monster::MonFlyingColVector(float _DeltaTime)
{

	FVector CheckPos = GetActorLocation();
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		CheckPos.X -= 1;
		break;
	case EMonsterDir::Right:
		CheckPos.X += 1;
		break;
	default:
		break;
	}
	//CheckPos.Y -= 32.0f;
	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	if (ColorCyan == Color8Bit(0, 255, 255, 0) )
	{
		/*int DirState = static_cast<int>(MonsterDirState);
		MoveVector.X *= DirState;*/
		return true;
	}

	return false;
	// bool -> �ε������� return true �ϵ���!!

}




bool APlay_Monster::MonDeathCheck(float _DeltaTime)
{

	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		MonsterDeath(0.0f);
		return true;
	}

	return false;
}

void APlay_Monster::MonsterDeath(float _DeltaTime)
{
	Spawn_Item();
	Destroy();
}

void APlay_Monster::Spawn_Item()
{

	APlay_Item* Item = GetWorld()->SpawnActor<APlay_Item>();
	Item->SetName("Item");
	Item->SetActorLocation(this->GetActorLocation());

	Item->ItemMoveVector.X = MoveVector.X * -1.0f;

	return;
}



void APlay_Monster::Jump(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);

}

void APlay_Monster::DownJump(float _DeltaTime)
{
	DirCheck();
	MoveCheck(_DeltaTime);
	MonsterColPhysics(_DeltaTime);

}

void APlay_Monster::SnowBomb(float _DeltaTime)
{

	SnowBallRenderer->ActiveOff();
	if (true == MonsterRenderer->IsCurAnimationEnd())
	{
		Destroy();
		return;
	}
}






void APlay_Monster::Snowball(float _DeltaTime)
{
	// ���� ; �������  -1�� �Ǵµ�,  ���ʹ� �Ƶչٵ��� �ȸ���..

	if (SnowStackOutTime >= 0)
	{//0���� Ŭ ���� ��� ���̳ʽ�
		SnowStackOutTime -= _DeltaTime*1.5;
	}
	else
	{//0���� �۾����� 3.0���� �ʱ�ȭ 
		SnowStackOutTime = 3.0f;
		if (SnowStack >= 0)
		{
			SnowStack -= 1; //0���� �۾��� ������ 

				if (SnowStack != -1) // snowstack�� �ϴ� -1�� �ƴϸ�, active�ϰ�, 
				{
					SnowBallRenderer->SetActive(true); 
					//MonsterRenderer->ChangeAnimation(GetAnimationName("Snowball"));
					SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack);
				}
				else if (SnowStack == -1) // snowstack�� -1�� �����Ǹ�, 
				{
					SnowBallRenderer->SetActive(false); 
					MonsterRenderer->SetTransform({ {0,-26}, {48 * 1.3f, 48 * 1.3f} });
					StateChange(EMonsterState::MonMove);
				}
				else if (SnowStack >= 3)// snowstack�� 3�̻��� �Ǹ�, 
				{
				SnowBallRenderer->SetActive(true);
				MonsterRenderer->SetTransform({ {0,-45}, {48 * 1.4f, 48 * 1.4f} });
				}
				

		}
		else if (SnowStack == -1) 
		{
			SnowBallRenderer->SetActive(false); 
			StateChange(EMonsterState::MonMove);
		}

	}

	int StackNum = 4;

	if (true == BulletColMonCheck(_DeltaTime))
	{ //Snowball���¿��� BulletColCheck -> true�� �׳� �״�� return;
		return;
	}
	DirCheck();

	SnowBallRenderer->SetImage("Snowball_01_R.png", SnowStack); // SnowStack n��°
	if (SnowStack < StackNum)
	{// SnowStack�� 3 under
		return;
	}
	else 
	{// SnowStack�� 

		SnowBallRenderer->SetImage("Snowball_01_R.png", 3);
		MonsterRenderer->SetTransform({ this->GetActorLocation(), {48 * 0.1f, 48 * 0.1f} });
		MonsterColPhysics(_DeltaTime);
	}
	return;
}




void APlay_Monster::Rolling(float _DeltaTime)
// Rolling���� ���ͼ�, Snowball�����̹���
// �÷��̾  push���¸� ���� �� �ֵ���
{
	DirCheck();
	MoveCheck(_DeltaTime); // ���� ���⼭ Rolling�� ������ ? �׳� Movecheck���� �����ִ� �����ε�
	std::vector<UCollision*> Result;

	// ��� ���⼭ Snowball�ϰ� Monster �ε����� -> Death
	if (SnowCollision->CollisionCheck(SnowBrosCollisionOrder::Monster, Result))
	{
		
		for (UCollision* Collision : Result)
		{
			APlay_Monster* Monster1 = static_cast<APlay_Monster*>(Collision->GetOwner());
			
			//Monster->MonsterDeath(0.0f);
			Monster1->StateChange(EMonsterState::MonFlying);
			// snowball�� �ƴ϶�, monster (collision�� getowner)
		}
	}
}


void APlay_Monster::MoveCheck(float _DeltaTime)
{
	MonsterMoveUpdate(_DeltaTime);
}

void APlay_Monster::ColMoveUpdate(float _DeltaTime) // ���Ͱ� snowball������ ��, �÷��̾ �� �� ����
{
	APlay_Player* Player = APlay_Player::GetMainPlayer();
	FVector CurPlayerPos = Player->GetActorLocation();
	FVector CurMonsterPos = GetActorLocation();
	FVector PlayerSpeed = Player->PlayerRollingSpeed;

}


bool APlay_Monster::BulletColMonCheck(float _DeltaTime)
{
	std::vector<UCollision*> BulletResult;
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Bullet, BulletResult))
	{
		APlay_Bullet* Bullet = (APlay_Bullet*)BulletResult[0]->GetOwner();
		if (SnowStack < 4) 
		{
			SnowStack++;// �� ���ƴ� 
		}
		else if(SnowStack >= 4)
		{
			SnowStack = 3;
		}

		//Bullet->BulletColCheck(_DeltaTime);
		Bullet->Destroy();
		return true;
	}

	return false;
}





void APlay_Monster::MonsterColPhysics(float _DeltaTime)
{

	std::vector<UCollision*> PlayerResult;
	// ���Ͱ� �÷��̾�� �浹������ , 
	if (true == BodyCollision->CollisionCheck(SnowBrosCollisionOrder::Player, PlayerResult))
	{

		APlay_Player* Player = APlay_Player::GetMainPlayer();
		if (EMonsterState::Snowball == this->GetState()
			&& EPlayState::PlayerPush == Player->GetState())
		{	//���� ���� ���°� ����캼�̰�, �÷��̾ Push �����̸�
			switch (Player->DirState)
			{
			case EActorDir::Left:
				MonsterDir = FVector::Left;
				break;
			case EActorDir::Right:
				MonsterDir = FVector::Right;
				break;
			default:
				break;
			}


		}


	}
}


void APlay_Monster::AddMoveVector(const FVector& _DirDelta) // ���ӵ� -> ������� �ٲ�
{
	MoveVector = _DirDelta * MoveAcc;
}


void APlay_Monster::MonsterMoveUpdate(float _DeltaTime)
{
	MonsterMoveVector(_DeltaTime);
	MonsterGravityVector(_DeltaTime);
	MonsterLastMoveVector(_DeltaTime);
	AddActorLocation(TotalLastMoveVector * _DeltaTime);
}


void APlay_Monster::MonsterGroundUp(float _DeltaTime)
{
	while (true)
	{
		FVector Location = GetActorLocation();
		Location.Y -= 1.0f;
		Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(Location.iX(), Location.iY(), Color8Bit::CyanA);
		if (Color == Color8Bit(0, 255, 255, 0))
		{
			AddActorLocation(FVector::Up);
			//	this->StateChange(EMonsterState::Jump);
		}
		else
		{
			break;
		}
	}
}




void APlay_Monster::MonsterMoveVector(float _DeltaTime)
{

	FVector CheckPos = GetActorLocation();
	switch (MonsterDirState)
	{
	case EMonsterDir::Left:
		CheckPos.X -= 1;
		break;
	case EMonsterDir::Right:
		CheckPos.X += 1;
		break;
	default:
		break;
	}
	CheckPos.Y -= 32.0f;
	Color8Bit ColorCyan = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::CyanA);
	Color8Bit ColorMagenta = USnowBros_Helper::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (ColorCyan == Color8Bit(0, 255, 255, 0))
	{
		if (ColorMagenta == Color8Bit(255, 0, 255, 0))
		{
			StateChange(EMonsterState::SnowBomb);
		}
		MoveVector.X *= -1.0f;
	}


}

void APlay_Monster::MonsterGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime; // �߷°��ӵ��� ���� ������ ��ġ. 

	Color8Bit Color = USnowBros_Helper::ColMapImage->GetColor(GetActorLocation().iX(),
		GetActorLocation().iY(), Color8Bit::CyanA);
	if (Color == Color8Bit(0, 255, 255, 0))
	{
		GravityVector = FVector::Zero;// ���� ������ 0���� �ʱ�ȭ 
	}
}

void APlay_Monster::MonsterLastMoveVector(float _DeltaTime)
{
	TotalLastMoveVector = FVector::Zero;
	TotalLastMoveVector = TotalLastMoveVector + MoveVector;
	TotalLastMoveVector = TotalLastMoveVector + GravityVector;
	TotalLastMoveVector = TotalLastMoveVector + JumpVector;
}



void APlay_Monster::SetAnimation(std::string _Name)
{
	std::string FullAniName = GetAnimationFullName(_Name);
	MonsterRenderer->ChangeAnimation(FullAniName);
}

std::string APlay_Monster::GetAnimationFullName(std::string _Name)
{
	return _Name;

}
