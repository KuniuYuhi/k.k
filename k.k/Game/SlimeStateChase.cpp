#include "stdafx.h"
#include "SlimeStateChase.h"
#include "Slime.h"

void SlimeStateChase::ManageState()
{
	//ƒvƒŒƒCƒ„[‚ð’Ç‚¢‚©‚¯‚é
	m_slime->MoveChasePlayer(m_slime->GetCharacterController());
}

void SlimeStateChase::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAnimationClip_Chase,0.1f);
}
