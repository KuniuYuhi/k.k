#include "stdafx.h"
#include "BraveState_SkillStart.h"
#include "Brave.h"
#include "WeaponBase.h"

void BraveState_SkillStart::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Skill_Start + m_brave->GetCurrentAnimationStartIndexNo(), 0.2f
	);
}

void BraveState_SkillStart::Entry()
{
	//�A�N�V�������ɂ���
	m_brave->ActionActive();

	m_brave->GetArmedWeapon()->EntrySkillAttackProcess(WeaponBase::enStart);
}

void BraveState_SkillStart::Ubdate()
{
	//����̂ق��ŏ�������
	m_brave->GetArmedWeapon()->UpdateSkillAttackProcess(WeaponBase::enStart);

}

void BraveState_SkillStart::Exit()
{
	//�X�L�����C���X�e�[�g�ɐi�߂Ȃ��Ȃ�
	if (!m_brave->IsProceedSkillMain())
	{
		//�A�N�V�������I���
		m_brave->ActionDeactive();
	}

	//���푤�ŏ���
	//�X�L���U�����s�����Ƃ��͂����ŃA�N�V�������I���
	m_brave->GetArmedWeapon()->ExitSkillAttackProcess(WeaponBase::enStart);
}
