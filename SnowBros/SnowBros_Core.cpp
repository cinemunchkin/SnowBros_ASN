#include "SnowBros_Core.h"
#include "SnowBros_Player.h"
#include "Level_Title.h"
#include "Level_Play.h"

SnowBros_Core::SnowBros_Core()
	: EngineCore()
{
}

SnowBros_Core::~SnowBros_Core()
{
}

// ���ӽ���
void SnowBros_Core::BeginPlay()
{
	SetFrame(60);

	// �̰����� �ʿ��� ��� ���ҽ��� �α׸� �ε��ϴ� ��� <= ��õ���� �ʴ´�.
	// ���ҽ� ���� �ϰڴٴ°� ��������� ���� ���ҽ��� �̸��� �ε��� ���ڴ�.
	// ���� �����̶�� ���� ���� ����ϴ� ���ҽ��� ����ϰ� �ε��ϴ� ����� ������ �����ϸ鼭
	// ���ÿ� �װ� ����ڰ� ��ġä�� ���ϰ� �ؾ� �Ѵ�. 

	// "Title Level" + "�� ����ٰ� ������ �����ϴ�";

	CreateLevel<ULevel_Title>("Level_Title");
	CreateLevel<ULevel_Play>("Level_Play");

	ChangeLevel("Level_Play");
}

void SnowBros_Core::Tick(float _DeltaTime)
{
	// �÷��̾� �������� �Ѵ�.
}

void SnowBros_Core::End()
{
}