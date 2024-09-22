#include "stdafx.h"
#include "StaminaRecoveryBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

StaminaRecoveryBuffItem::StaminaRecoveryBuffItem()
{
	Init();
}

StaminaRecoveryBuffItem::~StaminaRecoveryBuffItem()
{
}

void StaminaRecoveryBuffItem::Start()
{
}

void StaminaRecoveryBuffItem::UpdateComponent()
{
	//�v���C���[�̃X�^�~�i���񕜂�����
	if (isRecoveryPlayerStamina)
	{
		DeleteProcess();

		delete this;
	}
}

void StaminaRecoveryBuffItem::Init()
{
	//�X�e�[�^�X��������
	m_status.InitEffectItemStatus("RecoveryStamina");

	m_brave = FindGO<Brave>("Brave");

	//�v���C���[�̃X�^�~�i���񕜂�����
	m_brave->GetStatus().RecoveryStamina(m_status.GetEffectValue());

	//�G�t�F�N�g����
	m_effect = NewGO<UseEffect>(0, "RecoveryStaminaStartEffect");
	m_effect->PlayEffect(
		enEffect_StaminaRecoveryStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

	//���ʉ��Đ�
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_ItemStaminaRecovery,
		g_soundManager->GetSEVolume()
	);

	//�X�^�~�i���񕜂���
	isRecoveryPlayerStamina = true;
}

void StaminaRecoveryBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//�v���C���[�̃R���|�[�l���g���玩�g���폜����
	m_brave->DeleteComponent(this);
}

