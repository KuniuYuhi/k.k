#include "stdafx.h"
#include "AIActor.h"

AIActor::AIActor()
{
	//todo ‚±‚ê‚â‚ß‚é
	m_player = FindGO<Player>("player");
}

AIActor::~AIActor()
{
}

void AIActor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void AIActor::SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

void AIActor::PlayAttackSound()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Attack1, g_soundManager->GetSEVolume());
}