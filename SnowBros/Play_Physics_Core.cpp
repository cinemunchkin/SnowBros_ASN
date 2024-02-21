#include "Play_Physics_Core.h"


APlay_Physics_Core::APlay_Physics_Core()
{
}

APlay_Physics_Core::~APlay_Physics_Core()
{
}


void Player_Strobe(float _StrobeTime) // 몬스터랑 충돌하면 깜빡이는 함수 만들기
{
	// 액터는 자신을 죽이면서
	//UTickObject::Destroy(_DestroyTime);

	//// 자신이 관리하고 있는 랜더러들도 다 죽여야 한다.
	//for (UImageRenderer* Renderer : Renderers)
	//{
	//	Renderer->Destroy(_DestroyTime);
	//}
}
