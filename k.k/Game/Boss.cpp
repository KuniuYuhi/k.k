#include "stdafx.h"
#include "Boss.h"
#include "Lich.h"

#include "GameManager.h"

Boss::Boss()
{
}

Boss::~Boss()
{
	DeleteGO(m_bossBase);
}

bool Boss::Start()
{
	//�{�X�̐���
	CreatBossCharacter();

	return true;
}

void Boss::Update()
{
	IsDeleteBoss();


}

void Boss::CreatBossCharacter()
{
	/*m_sumoner = NewGO<Summoner>(0, "summoner");
	m_sumoner->SetPosition(m_position);
	m_bossBase = m_sumoner;*/

	m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition(m_position);
	m_bossBase = m_lich;
	
}

void Boss::IsDeleteBoss()
{
	//�{�X�������폜����Ă��悢�Ȃ�
	if (GameManager::GetInstance()->GetBossDeleteOkFlag() == true)
	{
		//�{�X���폜
		DeleteGO(this);
	}
}

