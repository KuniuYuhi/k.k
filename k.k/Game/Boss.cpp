#include "stdafx.h"
#include "Boss.h"
#include "Lich.h"

//#include "CharactersInfoManager.h"

Boss::Boss()
{
}

Boss::~Boss()
{
	DeleteGO(m_bossBase);
}

bool Boss::Start()
{
	//É{ÉXÇÃê∂ê¨
	CreatBossCharacter();

	return true;
}

void Boss::Update()
{
}

void Boss::CreatBossCharacter()
{
	/*m_sumoner = NewGO<Summoner>(0, "summoner");
	m_sumoner->SetPosition(m_position);*/

	m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition(m_position);
	//
	m_bossBase = m_lich;
}


//void Boss::Render(RenderContext& rc)
//{
//}

