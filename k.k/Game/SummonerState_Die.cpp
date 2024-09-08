#include "stdafx.h"
#include "SummonerState_Die.h"
#include "Summoner.h"

#include "GameSceneManager.h"

void SummonerState_Die::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(enSummonerAnimClip_Die, 0.2f);
}

void SummonerState_Die::Entry()
{
	//�A�j���[�V�������x��x������
	m_summoner->GetModelRender().SetAnimationSpeed(0.5f);

	m_summoner->ActionActive();
	//�����ɓ��������_�ŏ��s�͌��܂��Ă���̂�
	//�v���C���[�����������Ƃ�`����
	GameSceneManager::GetInstance()->SetOutComePlayerWin();

}

void SummonerState_Die::Ubdate()
{
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//���S��̏���
		m_summoner->AfterDieProcess();
	}
}

void SummonerState_Die::Exit()
{

	m_summoner->ActionDeactive();
}
