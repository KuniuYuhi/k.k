#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");

	m_status.InitWeaponCommonStatus("BowArrow");

	//初期化処理
	Init();

	return true;
}

void Bow::Update()
{
	if(m_enWeaponState==enArmed)
	{
		MoveArmed();
	}



	m_bowModelRender.Update();

}

void Bow::Init()
{
	//弓モデルの初期化
	m_bowModelRender.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//弓と矢の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

	//武器が収納状態なら
	if (m_enWeaponState == enStowed)
	{
		ChangeStowedState();
	}
	//武器が装備状態なら
	else
	{
		ChangeArmedState();
	}

}

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//収納時の座標に変更
	m_bowModelRender.SetPosition(m_stowedPosition);

}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void Bow::AttackAction()
{
}

void Bow::ProceedComboAttack()
{
	//３コンボ以上なら
	if (m_enComboState >= enCombo_Third)
	{
		//コンボステートをなしにリセットする
		m_enComboState = enCombo_None;
	}

	//コンボを一つ進める
	m_enComboState = static_cast<EnComboState>(m_enComboState + 1);
}

void Bow::ResetComboAttack()
{
	//コンボステートをなしにリセットする
	m_enComboState = enCombo_None;
}

bool Bow::IsEndDefensiveAction()
{

	//アニメーションが終わったら
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		//回避アクションを終わる
		return true;
	}

	return false;
}

void Bow::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//剣のワールド座標を設定
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}
