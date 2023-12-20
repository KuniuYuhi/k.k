#include "stdafx.h"
#include "CactusStateChase.h"
#include "Cactus.h"

void CactusStateChase::ManageState()
{
	//’Ç‚¢‚©‚¯‚é
	m_cactus->MoveChasePlayer(m_cactus->GetCharacterController());
}

void CactusStateChase::PlayAnimation()
{
	m_cactus->GetModelRender().PlayAnimation(enAnimationClip_Chase, 0.3f);
}
