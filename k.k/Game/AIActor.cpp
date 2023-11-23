#include "stdafx.h"
#include "AIActor.h"

AIActor::AIActor()
{
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

void AIActor::SetTargetPosition()
{
	//ターゲット(プレイヤー)の座標を取得
	m_targetPosition = m_player->GetPosition();
}

void AIActor::PlayAttackSound()
{
	g_soundManager->InitAndPlaySoundSource(enSoundName_Attack1, g_soundManager->GetSEVolume());
}