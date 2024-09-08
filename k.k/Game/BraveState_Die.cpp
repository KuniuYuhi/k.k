#include "stdafx.h"
#include "BraveState_Die.h"
#include "Brave.h"
#include "GameSceneManager.h"

void BraveState_Die::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_Die + m_brave->GetCurrentAnimationStartIndexNo(), 0.2f
	);
}

void BraveState_Die::Entry()
{
	m_brave->ActionActive();
	//アニメーション速度を落とす
	m_brave->GetModelRender().SetAnimationSpeed(0.5f);
	//プレイヤーの敗北を知らせる
	GameSceneManager::GetInstance()->SetOutComPlayerLose();
}

void BraveState_Die::Ubdate()
{

}

void BraveState_Die::Exit()
{
}
