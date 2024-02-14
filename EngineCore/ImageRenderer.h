#pragma once
#include "SceneComponent.h"
#include <EnginePlatform\WindowImage.h>
#include <map>

class UAnimationInfo
{
public:
	// 애니메이션을 구성할때 이미지는 1장
	UWindowImage* Image = nullptr;
	std::string Name;
	//int Start = -1;-> 필요 없음
	//int End = -1;-> 필요 없음
	int CurFrame = 0;
	float CurTime = 0.0f;
	bool Loop = false;
	bool IsEnd = false;
	std::vector<float> Times;
	std::vector<int> Indexs;

	int Update(float _DeltaTime);
};

class UWindowImage;

class UImageRenderer : public USceneComponent
{

public:
	// constrcuter destructer
	UImageRenderer();
	~UImageRenderer();

	// delete Function
	UImageRenderer(const UImageRenderer& _Other) = delete;
	UImageRenderer(UImageRenderer&& _Other) noexcept = delete;
	UImageRenderer& operator=(const UImageRenderer& _Other) = delete;
	UImageRenderer& operator=(UImageRenderer&& _Other) noexcept = delete;

	void SetOrder(int _Order) override;
	void Render(float _DeltaTime);
	// 이미지를 세팅하는 역할만 하고
	void SetImage(std::string_view _Name, int _InfoIndex = 0);

	void SetImageIndex(int _InfoIndex)
	{
		InfoIndex = _InfoIndex;
	}

	void SetTransform(const FTransform& _Value)
	{
		USceneComponent::SetTransform(_Value);
	}

	void SetImageCuttingTransform(const FTransform& _Value)
	{
		ImageCuttingTransform = _Value;
	}

	void CreateAnimation(
		std::string_view _AnimationName, 
		std::string_view _ImageName, 
		int _Start, 
		int _End, 
		float _Inter, 
		bool Loop = true
	);


	void CreateAnimation(
		std::string_view _AnimaionName,
		std::string_view _imageName,
		std::vector<int> _Indexs,
		float _Inter,
		bool _Loop = true
	);



	void ChangeAnimation(
		std::string_view _AnimationName,
		bool _IsForce = false,
		int _StartIndex = 0,
		float _Time = -1.0f);


	void AnimationReset();

	void SetTransColor(Color8Bit _Color)
	{
		TransColor = _Color;
	}

	// 0~1.0f
	void SetAlpha(float _Alpha)
	{
		if (0.0f >= _Alpha)
		{
			_Alpha = 0.0f;
		}

		if (1.0f <= _Alpha)
		{
			_Alpha = 1.0f;
		}

		TransColor.A = static_cast<char>(_Alpha * 255.0f);
	}

	UWindowImage* GetImage()
	{
		return Image;
	}


	void CameraEffectOff() // UI가 카메라 이동을 따라다니지 않도록
	{
		CameraEffect = false; 

	}

	bool IsCurAnimationEnd() const 
	{
		return CurAnimation->IsEnd;
	}
	// 애니메이션의 마지막 프레임에 도달했는지? true/false 
	// => IsEnd가 마지막 프레임이어야함!! 한 프레임!!!!


	bool GetCurAnimationFrame() const 
	{
		return CurAnimation->CurFrame;
	}
	//현재 애니메이션의 현재 프레임은 ?  ==> 이거 쓰는 함수 void ATestPlayer::DirCheck()


	bool GetCurAnimationimageFrame() const
	{
		const std::vector <int>&Indexs = CurAnimation->Indexs;
		return Indexs[CurAnimation->CurFrame];
	}
// ex 총쏘는 애니메이션 중 좌우 전환 -> 총쏘는 애니메이션은 유지하고 싶다
// 현재 프레임 값을 받아서 물려받는다.
// 나한테 필요한가?? -> 포션먹고 빨리 달리는 애니메이션


	float GetCurAnimationTime() const
	{
		return CurAnimation->CurTime;
	}

protected:
	void BeginPlay() override;

private:
	int InfoIndex = 0;
	UWindowImage* Image = nullptr;
	FTransform ImageCuttingTransform;
	Color8Bit TransColor;

	bool CameraEffect = true;

	std::map<std::string, UAnimationInfo> AnimationInfos;
	UAnimationInfo* CurAnimation = nullptr;
};

