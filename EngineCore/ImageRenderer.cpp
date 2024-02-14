#include "ImageRenderer.h"
#include "EngineCore.h"
#include "Actor.h"
#include "Level.h"
#include <EngineCore\EngineResourcesManager.h>

UImageRenderer::UImageRenderer() 
{
}

UImageRenderer::~UImageRenderer() 
{
}

void UImageRenderer::SetOrder(int _Order)
{
	AActor* Owner = GetOwner();
	ULevel* Level = Owner->GetWorld();

	std::map<int, std::list<UImageRenderer*>>& Renderers = Level->Renderers;
	// ���� ������ �ٲ������ Ȥ�ó� ���� � �׷쿡 �����ִٸ�
	// �� �׷쿡�� ���� ���´�.
	Renderers[GetOrder()].remove(this);

	// ���� ������ �ٲ��.
	UTickObject::SetOrder(_Order);

	Renderers[GetOrder()].push_back(this);
}

int UAnimationInfo::Update(float _DeltaTime)
{
	//UWindowImage* Image = nullptr;
	//int Start = -1;
	//int End = -1;
	//int CurFrame = 0;
	//float CurTime = 0.0f;
	//std::vector<float> Times;
	
	//std::vector<int> Indexs;
	//    CurFrame = 25
	// 
	// //            0  1  2  3  4  5 
	//    Indexs => 20 21 22 23 24 25
	



	IsEnd = false;
	CurTime -= _DeltaTime;

	if (0.0f >= CurTime)
	{
		CurTime = Times[CurFrame];
		++CurFrame;
	}

	if (Indexs.size() <= CurFrame)
	{
		IsEnd = true; 
		// �ִϸ��̼� �����, ���� �����Ӻ��� ������ 
		// -> �ִϸ��̼��� ���� ������ ����.

		if (true == Loop)
		{
			CurFrame = 0;
		}
		else
		{
			--CurFrame;
		}

	}
		int Index = Indexs[CurFrame];

		return Index;
		// �׸��� ������������ �ִϸ��̼� ����. 


}


void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("�̹����� �������� �ʴ� ������ �Դϴ�");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	// ���������� �о�� �մϴ�.
	FTransform RendererTrans = GetTransform();

	// �����ε� ���� �ǹ̰� �����ϴ�.
	FTransform ActorTrans = GetOwner()->GetTransform();

	// ������Ʈ�� ��ġ�� �θ𿡰Լ� ������̱� ������.
	// �θ��� ��ġ�� ������� �Ѵ�.
	RendererTrans.AddPosition(ActorTrans.GetPosition());


	if (true == CameraEffect) // ī�޶� ������� ȿ��
	{
		AActor* Actor = GetOwner();//return Owner / Actor�� �˰�
		ULevel* World = Actor->GetWorld();//return World / Actor�� World�� �˰�
		FVector CameraPos = World->GetCameraPos(); 
		// RendererTrans.AddPosition(CameraPos *= -1.0f);
		RendererTrans.AddPosition(-CameraPos);// RenderTrans = GetTransform
		/*
		ī�޶� �̵��� ��ŭ ���ش�.. 
	
		�̰� ��ġ �ٲ㰡�鼭 �� �غ��߰ٴ� �򰥸���..

		*/
	}


	
	// TransColor ���� Ư�� ����� 1��Ʈ�� ���̰� ���� �ʴ� ������ ����� �����Ѵ�.
	// TransCopy ������ 
	// Png�� ��쿡 

	EWIndowImageType ImageType = Image->GetImageType();
	// �̹��� Ÿ���� ��ȯ. None, Png, Bmp


	switch (ImageType)
	{
	case EWIndowImageType::IMG_BMP:
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		// bmp�϶��� �Ϲ������� Transcopy�� ����ó���� �Ѵ�.
		break;
	case EWIndowImageType::IMG_PNG:
		GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("����ó���� �Ұ����� �̹��� �Դϴ�.");
		break;
	}
}

void UImageRenderer::BeginPlay()
{
	// �θ���� ��������ִ� ������ ������.
	USceneComponent::BeginPlay();
}

void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex /*= 0*/)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		// ���ܸ� ����ϰ� �ϴ°͵� �߿��ϴ�.
		MsgBoxAssert(std::string(_Name) + "�̹����� �������� �ʽ��ϴ�.");
		return;
	}

	InfoIndex = _InfoIndex;
}


void UImageRenderer::CreateAnimation( std::string_view _AnimationName,
	std::string_view _ImageName,
	int _Start,
	int _End,
	float _Inter,
	bool _Loop /*= true*/ )
{
	std::vector<int> Indexs;

	//int  Size = _End -_Start; ��� �Ǵ� �� ������
	  
	for (int i = _Start; i <= _End; i++)
	{
		Indexs.push_back(i);
	}

	CreateAnimation(_AnimationName, _ImageName, Indexs, _Inter, _Loop);
}


void UImageRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _ImageName,
	std::vector<int> _Indexs,
	float _Inter,
	bool _Loop
)
{
	UWindowImage* FindImage = UEngineResourcesManager::GetInst().FindImg(_ImageName);
	// FindImage 
	if (nullptr == FindImage)
	{
		MsgBoxAssert(std::string(_ImageName) + "�������� �ʴ� �̹���")
			return;
	}

	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (true == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "�̹� �����ϴ� �̸��� �̹���")
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	Info.Name = UpperAniName;
	Info.Image = FindImage;
	Info.CurFrame = 0;
	Info.CurTime = 0.0f;
	Info.Loop = _Loop;

	//          12         0
//	int Size = Info.End - Info.Start;
	
	int Size = static_cast<int> (_Indexs.size()); //�ִϸ��̼� �ε����� ����� int�� ��ȯ
	
	
	Info.Times.reserve(Size); // �� �Լ� ����?? 
							//std::vector<float> Times;
							// �� �ؿ��� push_back���ַ��� reserve�Ѱű��� 
	
	for(int i = 0; i <=Size; i++)
	{
		Info.Times.push_back(_Inter);
	}

}






void UImageRenderer::ChangeAnimation(
	std::string_view _AnimationName,
	bool _IsForce,
	int _StartIndex,
	float _Time)
{
	std::string UpperAniName = UEngineString::ToUpper(_AnimationName); // �빮�� �����

	if (false == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "��� �̸��� �ִϸ��̼� ����.");
		return;
	}

	// ���� �������� �ִϸ��̼ǰ� ������ �Ȱ��� �ִϸ��̼��� �����϶���ϸ� �װ� �������� �ʴ´�.
	if (false == _IsForce && nullptr != CurAnimation && CurAnimation->Name == UpperAniName)
	{
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	CurAnimation = &Info;
	CurAnimation->CurFrame = 0;
	CurAnimation->CurTime = CurAnimation->Times[0];

	if (0.0f >= _Time) 
		// �Է� �ð��� 0�̸� CurAnimation �� �ð��� �Է½ð�����??????
		// �̰� �ִϸ��̼� �ٲ� ��, ���� �ִϸ��̼��� �ð����� �����޴���� ..!
		// �³�??
		//void ATestPlayer::DirCheck ����
	{
		CurAnimation->CurTime = _Time;
	}
	CurAnimation->IsEnd = false; // �ִϸ��̼� �ٲ��ְ�, IsEnd�� �ٽ� false�� �ʱ�ȭ 
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}