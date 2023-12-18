#include "stdafx.h"
#include "SlimeStateChase.h"
#include "Slime.h"

void SlimeStateChase::ManageState()
{
	//プレイヤーを追いかける
	m_slime->MoveChasePlayer(m_slime->GetCharacterController());
}

void SlimeStateChase::PlayAnimation()
{
	m_slime->GetModelRender().PlayAnimation(enAninationClip_Chase,0.1f);
}
