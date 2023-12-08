#include "stdafx.h"
#include "SummonerState_Die.h"
#include "Summoner.h"

#include "InitEffect.h"
#include "GameManager.h"

void SummonerState_Die::ManageState()
{
	//���S�G�t�F�N�g���Đ�����n�߂���
	if (m_isdeadEffectPlayFlag == true)
	{
		//���S�̍Đ����I�������
		if (m_deadEffect->IsPlay() != true)
		{
			//�S�Ă̏������I�������폜����Ă��悢
			GameManager::GetInstance()->SetBossDeleteOkFlag(true);
		}
		//�ȍ~�̏����͂���K�v���Ȃ��̂�return
		return;
	}
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//���ꂽ�玀�S�G�t�F�N�g�Đ�
		CreateDeadEffect();
		//���f���h���[�t���O�����Z�b�g
		m_summoner->SetIsDrawModelFlag(false);
		//���u�����X�^�[���폜
		m_summoner->DeleteMobMonsters();
	}
}

void SummonerState_Die::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Die);
}

void SummonerState_Die::CreateDeadEffect()
{
	m_deadEffect = NewGO<EffectEmitter>(0);
	m_deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	m_deadEffect->Play();
	m_deadEffect->SetPosition(m_summoner->GetPosition());
	m_deadEffect->SetScale(g_vec3One * 8.0f);
	m_deadEffect->Update();
	//�t���[�����[�g��߂�
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);

	//�G�t�F�N�g�Đ��t���O���Z�b�g
	m_isdeadEffectPlayFlag = true;
}
