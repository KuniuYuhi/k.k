#include "stdafx.h"
#include "AttackBuffItem.h"
#include "Brave.h"
#include "UseEffect.h"

namespace {
	const float EFFECT_SCALE = 12.0f;
}

AttackBuffItem::AttackBuffItem()
{
	Init();
}

AttackBuffItem::~AttackBuffItem()
{
}

void AttackBuffItem::Start()
{
	
}

void AttackBuffItem::UpdateComponent()
{
	//�������ԂɂȂ����玩���I�ɍ폜����
	if (IsDelete())
	{
		DeleteProcess();

		delete this;
	}

}

void AttackBuffItem::Init()
{
	//�X�e�[�^�X��������
	m_status.InitEffectItemStatus("AttackBuff");

	m_brave = FindGO<Brave>("Brave");

	//�v���C���[�̍U���͂𑝉�������
	m_brave->GetStatus().AddPowerToDefaultPower(m_status.GetEffectValue());
	//�U���o�t����ɓ��ꂽ�̂ŃJ�E���g�𑝂₷
	m_brave->CalcAttackBuffCount(true);

	//�G�t�F�N�g����
	m_effect = NewGO<UseEffect>(0, "AttackBuffEffect");
	m_effect->PlayEffect(
		enEffect_AttackUpStart,
		m_brave,
		m_brave->GetPosition(),
		g_vec3One * EFFECT_SCALE,
		Quaternion::Identity,
		true
	);

}

void AttackBuffItem::DeleteProcess()
{
	m_effect = nullptr;

	//�v���C���[�̍U���͂𑝉�������������������
	m_brave->GetStatus().AddPowerToDefaultPower(-m_status.GetEffectValue());

	//�U���o�t���I���̂ŃJ�E���g�����炷
	m_brave->CalcAttackBuffCount(false);

	//�v���C���[�̃R���|�[�l���g���玩�g���폜����
	m_brave->DeleteComponent(this);
}

bool AttackBuffItem::IsDelete()
{
	if (m_deleteTimer >= m_status.GetDuration())
	{
		return true;
	}
	//�^�C�}�[�����Z
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();

	return false;
}
