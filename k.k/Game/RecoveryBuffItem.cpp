#include "stdafx.h"
#include "RecoveryBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

RecoveryBuffItem::RecoveryBuffItem()
{
	Init();
}

RecoveryBuffItem::~RecoveryBuffItem()
{
}

void RecoveryBuffItem::Start()
{
}

void RecoveryBuffItem::UpdateComponent()
{
	//�v���C���[��HP���񕜂�����
	if (isRecoveryPlayerHp)
	{
		DeleteProcess();

		delete this;
	}
}

void RecoveryBuffItem::Init()
{
	//�X�e�[�^�X��������
	m_status.InitEffectItemStatus("RecoveryHP");

	m_brave = FindGO<Brave>("Brave");

	//�v���C���[��HP���񕜂�����
	m_brave->GetStatus().RecoveryHp(m_status.GetEffectValue());

	//�G�t�F�N�g����
	m_effect = NewGO<UseEffect>(0, "RecoveryStartEffect");
	m_effect->PlayEffect(
		enEffect_RecoveryStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

	//HP���񕜂���
	isRecoveryPlayerHp = true;
}

void RecoveryBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//�v���C���[�̃R���|�[�l���g���玩�g���폜����
	m_brave->DeleteComponent(this);
}
