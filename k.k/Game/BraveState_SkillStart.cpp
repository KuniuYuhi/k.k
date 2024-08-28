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
	//アクション中にする
	m_brave->ActionActive();

	m_brave->GetArmedWeapon()->EntrySkillAttackProcess(WeaponBase::enStart);
}

void BraveState_SkillStart::Ubdate()
{
	//武器のほうで処理する
	m_brave->GetArmedWeapon()->UpdateSkillAttackProcess(WeaponBase::enStart);

}

void BraveState_SkillStart::Exit()
{
	//スキルメインステートに進めないなら
	if (!m_brave->IsProceedSkillMain())
	{
		//アクションを終わる
		m_brave->ActionDeactive();
	}

	//武器側で処理
	//スキル攻撃失敗したときはここでアクションを終わる
	m_brave->GetArmedWeapon()->ExitSkillAttackProcess(WeaponBase::enStart);
}
