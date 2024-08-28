#include "stdafx.h"
#include "BraveState_DefensiveHit.h"
#include "Brave.h"
#include "BraveStateContext.h"
#include "WeaponBase.h"

void BraveState_DefensiveHit::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_DefensiveHit + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_DefensiveHit::Entry()
{
	//
	m_brave->EntryHitActionProcess();
	//����̊J�n����
	m_brave->GetArmedWeapon()->EntryDefensiveHitProcess();
}

void BraveState_DefensiveHit::Ubdate()
{
	m_brave->GetArmedWeapon()->UpdateDefensiveHitProcess();
	//�A�j���[�V�������I�������
	m_brave->UpdateHitActionProcess();
}

void BraveState_DefensiveHit::Exit()
{
	//������x���Ŏ�邩������Ȃ��̂ŕ���̏�������
	m_brave->GetArmedWeapon()->ExitDefensiveHitProcess();
	//�q�b�g�A�N�V�����𔲂���Ƃ��̏���
	m_brave->ExitHitActionProcess();
}
