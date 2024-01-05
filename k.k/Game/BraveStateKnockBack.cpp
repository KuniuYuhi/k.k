#include "stdafx.h"
#include "BraveStateKnockBack.h"
#include "Brave.h"

namespace {
	const float STIFFNESS_LIMMIT = 0.4f;
}

void BraveStateKnockBack::ManageState()
{
	//ノックバックエンドフラグがセットされているなら
	if (m_knockBackEndFlag == true)
	{
		//少しだけ硬直する
		//硬直が終わったら処理終わり
		if (IsStiffness() == false)
		{
			//ステート共通の状態遷移処理に遷移
			m_brave->ProcessKnockBackStateTransition();
		}
		return;
	}

	if (m_brave->IsKnockBack() != true)
	{
		//ノックバックがおわったのでフラグをセット
		m_knockBackEndFlag = true;
	}
}

void BraveStateKnockBack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_KnockBack + m_brave->GetCurrentMainWeaponAnimationStartIndexNo()
	);
}

bool BraveStateKnockBack::IsStiffness()
{
	if (m_stiffnessTimer > STIFFNESS_LIMMIT)
	{
		return false;
	}
	else
	{
		m_stiffnessTimer += g_gameTime->GetFrameDeltaTime();
	}
	
	return true;
}
