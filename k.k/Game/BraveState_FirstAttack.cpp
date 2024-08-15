#include "stdafx.h"
#include "BraveState_FirstAttack.h"
#include "Brave.h"
#include "PlayerController.h"

void BraveState_FirstAttack::PlayAnimation()
{
	m_brave->GetModelRender().PlayAnimation(
		enBraveAnimClip_FirstAttack + m_brave->GetCurrentAnimationStartIndexNo(),0.1f
	);
}

void BraveState_FirstAttack::Entry()
{
	if (m_playerController == nullptr)
	{
		//プレイヤーコントローラーコンポーネントを取得
		m_playerController = m_brave->GetComponent<PlayerController>();
	}

	//アクション中にする
	m_brave->ActionActive();
	//トリガーチェックフラグをリセット
	m_isTrigger = false;
	//遅延フラグリセット
	m_isDelay = false;
}

void BraveState_FirstAttack::Ubdate()
{
	//攻撃中に通常攻撃ボタンを押したなら
	if (m_playerController->IsTriggerNromalAttackButton()&& !m_isTrigger&& m_isDelay)
	{
		m_isTrigger = true;
	}

	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//ボタンを押していたら
		if (m_isTrigger)
		{
			//次のコンボ処理をする
			m_brave->NormalAttackProcess();
		}
		else
		{
			//ステートの共通処理
			m_brave->ProcessCommonStateTransition();
		}
		
	}

	//遅延フラグを立てる
	m_isDelay = true;
}

void BraveState_FirstAttack::Exit()
{
	//ボタンを押していなかったら
	if (!m_isTrigger)
	{
		//アクションを終わる
		m_brave->ExitAttackAction();
	}
	
	//トリガーチェックフラグをリセット
	m_isTrigger = false;
	//遅延フラグリセット
	m_isDelay = false;
}
