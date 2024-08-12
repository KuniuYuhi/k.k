#include "stdafx.h"
#include "BraveState_ChangeWeapon.h"
#include "Brave.h"

void BraveState_ChangeWeapon::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_ChangeWeapon + m_brave->GetCurrentAnimationStartIndexNo()
	);
}

void BraveState_ChangeWeapon::Entry()
{
	//アクション中にする
	m_brave->ActionActive();
}

void BraveState_ChangeWeapon::Ubdate()
{
	//アニメーションの再生が終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		m_brave->ProcessCommonStateTransition();
	}
}

void BraveState_ChangeWeapon::Exit()
{
	//アクションを終わる
	m_brave->ActionDeactive();
	//アニメーションの最初の番号を切り替え後の武器のものに変更する
	m_brave->SetCurrentAnimationStartIndexNoForMainWeaponType();


}
