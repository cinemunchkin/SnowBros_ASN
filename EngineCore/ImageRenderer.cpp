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
	// 나의 오더가 바뀌기전에 혹시나 내가 어떤 그룹에 속해있다면
	// 그 그룹에서 빠져 나온다.
	Renderers[GetOrder()].remove(this);

	// 나의 오더가 바뀐다.
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
		// 애니메이션 사이즈가, 현재 프레임보다 작으면 
		// -> 애니메이션이 끝난 것으로 본다.

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
		// 그리고 현재프레임의 애니메이션 리턴. 


}


void UImageRenderer::Render(float _DeltaTime)
{
	if (nullptr == Image)
	{
		MsgBoxAssert("이미지가 존재하지 않는 랜더러 입니다");
	}

	if (nullptr != CurAnimation)
	{
		Image = CurAnimation->Image;
		InfoIndex = CurAnimation->Update(_DeltaTime);
	}

	// 위에서부터 읽어야 합니다.
	FTransform RendererTrans = GetTransform();

	// 액터인데 지금 의미가 없습니다.
	FTransform ActorTrans = GetOwner()->GetTransform();

	// 컴포넌트의 위치는 부모에게서 상대적이기 때문에.
	// 부모의 위치를 더해줘야 한다.
	RendererTrans.AddPosition(ActorTrans.GetPosition());


	if (true == CameraEffect) // 카메라가 따라오는 효과
	{
		AActor* Actor = GetOwner();//return Owner / Actor를 알고
		ULevel* World = Actor->GetWorld();//return World / Actor가 World를 알고
		FVector CameraPos = World->GetCameraPos(); 
		// RendererTrans.AddPosition(CameraPos *= -1.0f);
		RendererTrans.AddPosition(-CameraPos);// RenderTrans = GetTransform
		/*
		카메라가 이동한 만큼 빼준다.. 
	
		이거 수치 바꿔가면서 좀 해봐야겟다 헷갈림ㅁ..

		*/
	}


	
	// TransColor 원리 특정 색상과 1비트도 차이가 나지 않는 색상은 출력을 삭제한다.
	// TransCopy 에서만 
	// Png일 경우에 

	EWIndowImageType ImageType = Image->GetImageType();
	// 이미지 타입을 반환. None, Png, Bmp


	switch (ImageType)
	{
	case EWIndowImageType::IMG_BMP:
		GEngine->MainWindow.GetBackBufferImage()->TransCopy(Image, RendererTrans, InfoIndex, TransColor);
		// bmp일때는 일반적으로 Transcopy로 투명처리를 한다.
		break;
	case EWIndowImageType::IMG_PNG:
		GEngine->MainWindow.GetBackBufferImage()->AlphaCopy(Image, RendererTrans, InfoIndex, TransColor);
		break;
	default:
		MsgBoxAssert("투명처리가 불가능한 이미지 입니다.");
		break;
	}
}

void UImageRenderer::BeginPlay()
{
	// 부모것을 실행시켜주는 습관을 들이자.
	USceneComponent::BeginPlay();
}

void UImageRenderer::SetImage(std::string_view _Name, int _InfoIndex /*= 0*/)
{
	Image = UEngineResourcesManager::GetInst().FindImg(_Name);

	if (nullptr == Image)
	{
		// 예외를 출력하게 하는것도 중요하다.
		MsgBoxAssert(std::string(_Name) + "이미지가 존재하지 않습니다.");
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

	//int  Size = _End -_Start; 없어도 되는 거 같은디
	  
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
		MsgBoxAssert(std::string(_ImageName) + "존재하지 않는 이미지")
			return;
	}

	std::string UpperAniName = UEngineString::ToUpper(_AnimationName);

	if (true == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "이미 존재하는 이름의 이미지")
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	Info.Name = UpperAniName;
	Info.Image = FindImage;
	Info.CurFrame = 0;
	Info.CurTime = 0.0f;
	Info.Loop = _Loop;

	//          12         0
//	int Size = Info.End - Info.Start;
	
	int Size = static_cast<int> (_Indexs.size()); //애니메이션 인덱스의 사이즈를 int로 변환
	
	
	Info.Times.reserve(Size); // 이 함수 뭐지?? 
							//std::vector<float> Times;
							// 아 밑에서 push_back해주려고 reserve한거구나 
	
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
	std::string UpperAniName = UEngineString::ToUpper(_AnimationName); // 대문자 어서오고

	if (false == AnimationInfos.contains(UpperAniName))
	{
		MsgBoxAssert(std::string(UpperAniName) + "라는 이름의 애니메이션 없음.");
		return;
	}

	// 지금 진행중인 애니메이션과 완전히 똑같은 애니메이션을 실행하라고하면 그걸 실행하지 않는다.
	if (false == _IsForce && nullptr != CurAnimation && CurAnimation->Name == UpperAniName)
	{
		return;
	}

	UAnimationInfo& Info = AnimationInfos[UpperAniName];
	CurAnimation = &Info;
	CurAnimation->CurFrame = 0;
	CurAnimation->CurTime = CurAnimation->Times[0];

	if (0.0f >= _Time) 
		// 입력 시간이 0이면 CurAnimation 의 시간을 입력시간으로??????
		// 이거 애니메이션 바꿀 때, 직전 애니메이션의 시간까지 물려받느라고 ..!
		// 맞나??
		//void ATestPlayer::DirCheck 참고
	{
		CurAnimation->CurTime = _Time;
	}
	CurAnimation->IsEnd = false; // 애니메이션 바꿔주고, IsEnd는 다시 false로 초기화 
}

void UImageRenderer::AnimationReset()
{
	CurAnimation = nullptr;
}