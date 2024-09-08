#include "stdafx.h"
#include "BeholderEyeState_Die.h"
#include "BeholderEye.h"

BeholderEyeState_Die::~BeholderEyeState_Die()
{
}

void BeholderEyeState_Die::PlayAnimation()
{
	m_beholderEye->GetModelRender().PlayAnimation(enBeholderEyeAnimClip_Die, 0.2f);
}

void BeholderEyeState_Die::Entry()
{
	m_beholderEye->ActionActive();
}

void BeholderEyeState_Die::Ubdate()
{
	//アニメーションが終わったら
	if (m_beholderEye->GetModelRender().IsPlayingAnimation() == false)
	{
		m_beholderEye->DieProcess();
	}

}

void BeholderEyeState_Die::Exit()
{
	m_beholderEye->ActionDeactive();
}
