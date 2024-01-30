#include "SnowBros_Player.h"
#include <EngineCore\EngineCore.h>
#include <EnginePlatform\EngineInput.h>
#include "SnowBros_Bullet.h"
#include <EngineBase\EngineTime.h>
#include <vector>
#include <list>

SnowBros_Player::SnowBros_Player()
{
}

SnowBros_Player::~SnowBros_Player()
{
}

void SnowBros_Player::BeginPlay()
{
	AActor::BeginPlay();

	// �÷��̾� 100, 100 => Actor
	// ��ü? 100, 100 - 50 => Renderer
	// ��ü? 100, 100 + 50 => Renderer


	{
		BodyRenderer = CreateImageRenderer(0);
		BodyRenderer->SetPosition({ 0, 30 });
		BodyRenderer->SetImageToScale("CB2.bmp");
		// BodyRenderer->SetScale({ 80, 80 });
	}


	{
		HeadRenderer = CreateImageRenderer(0);
		HeadRenderer->SetPosition({ 0, -25 });
		HeadRenderer->SetImageToScale("CB2.bmp");
	}
	// UImageRenderer* Ptr = CreateRenderer();
	// �÷��̾� ����
	// Ptr->SetPosition({0.0, -50.0f}); // 500, 500
	// Ptr->SetImage("AAAA.png"); // 500, 500
	// Ptr->SetScale();
	// Ptr->SetImageScaleSetting(); 


	// CreateDefaultSubObject<UStaticMeshRenderer>();

	// n�� �����Ҽ� �ְ� ������ �Ѵ�.
	// ��ü�� ��ü�� �����ٰ� �ĺ���.

	//{
	//	EngineTime NewTime;
	//	NewTime.TimeCheckStart();
	//	std::vector<int> NewInt;
	//	for (int i = 0; i < 10000000; i++)
	//	{
	//		NewInt.push_back(i);
	//	}
	//	float Time = NewTime.TimeCheck();
	//	int a = 0;
	//}
	//{
	//	EngineTime NewTime;
	//	NewTime.TimeCheckStart();
	//	std::list<int> NewInt;
	//	for (int i = 0; i < 10000000; i++)
	//	{
	//		NewInt.push_back(i);
	//	}
	//	float Time = NewTime.TimeCheck();
	//	int a = 0;
	//}



	SetActorLocation({ 100, 100 });
	SetActorScale({ 100, 100 });
}

void SnowBros_Player::Tick(float _DeltaTime)
{
	// 1�ʿ� 2���� �����ߴٸ�
	// Tick�� Tick���� �ð���? 
	// 0.5�� �ϲ���.

	// 1�ʿ� 100�� �����ϴ� �ְ� �ִٰ� Ĩ�ô�.
	// 0.5�ʿ� ���ȼ��� �������� �ϳ���?
	// 100 * 0.5

	if (true == EngineInput::IsPress('A'))
	{
		AddActorLocation(FVector::Left * 500.0f * _DeltaTime);
	}

	if (true == EngineInput::IsPress('D'))
	{
		AddActorLocation(FVector::Right * 500.0f * _DeltaTime);
	}

	if (true == EngineInput::IsPress('W'))
	{
		AddActorLocation(FVector::Up * 500.0f * _DeltaTime);
	}


	if (true == EngineInput::IsPress('S'))
	{
		AddActorLocation(FVector::Down * 500.0f * _DeltaTime);
	}

	if (true == EngineInput::IsDown('T'))
	{
		HeadRenderer->Destroy();
	}



	// QSkillCool += 0.1f;
	// QSkillCool += ��¥ �����Ӱ� ������ ���̿� �Ҹ�� ���� �ð��̾�� �Ѵ�.;

	// �ʴ� 2�� �����ߴٰ� �ص�
	// 0.5 0.5 0.5 0.5 
	// 2���� ������ 
	// �ʴ� 2�� ����ȴٰ� Ĩ�ô�.
	if (true == EngineInput::IsPress('Q'))
	{
		ASnowBros_Bullet* NewSnowBros_Bullet = GetWorld()->SpawnActor<ASnowBros_Bullet>();
		NewSnowBros_Bullet->SetActorLocation(GetActorLocation());
		NewSnowBros_Bullet->SetDir(FVector::Right);
	}
}