#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "Arrow.h"

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	//ステータス
	const int POWER = 10;
	const int ENDURANCE = 10;		//武器の耐久力(矢のストック)。

	const float HITTABLE_TIME = 0.15f;

	const float CHARGE_COMPLETE_TIME = 1.0f;

	const float SHOT_ARROW_ANGLE = 30.0f;			//矢を撃つときの角度
}

Bow::Bow()
{
	SetWeaponPower(POWER);
}

Bow::~Bow()
{
	if (m_arrow != nullptr)
	{
		DeleteGO(m_arrow);
	}
}

bool Bow::Start()
{
	//武器のステータス初期化
	m_status.InitWeaponStatus(
		POWER, ENDURANCE
	);

	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	//矢のオブジェクトを生成
	CreateArrow();

	InitModel();

	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);


	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void Bow::Update()
{
	//収納状態なら
	if (GetStowedFlag() == true)
	{
		return;
	}

	//矢の数が残っているなら、矢を生成
	if (m_status.endurance > 0)
	{
		IsCreatArrow();
	}

	//ヒット可能か判断する
	m_hitDelection.IsHittable(HITTABLE_TIME);

	MoveWeapon();

	m_modelBow.Update();
}

void Bow::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://収納状態
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://装備状態
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://なし
		break;
	default:
		break;
	}
}

void Bow::ProcessSkillAttack()
{
	//矢のストックがないならスキル発動できない
	if (m_status.endurance <= 0)
	{
		return;
	}

	//ボタンを押している間チャージ
	if (g_pad[0]->IsPress(enButtonB) == true)
	{
		//チャージしている間は回転可能
		SetRotationDelectionFlag(true);
		//チャージタイマーを加算
		m_ChargeTimer += g_gameTime->GetFrameDeltaTime()*2.4f;

	}
	//離した時にチャージタイマーがチャージ完了タイムを超えていたら
	//矢を撃つ
	else if (m_ChargeTimer >= CHARGE_COMPLETE_TIME)
	{
		//チャージタイマーをリセット
		m_ChargeTimer = 0.0f;
		//矢を撃つので回転可能フラグをリセット
		SetRotationDelectionFlag(false);
		//勇者のアニメーションをスキルメインに切り替え
		m_brave->SetNextAnimationState(Brave::enAnimationState_Skill_Main);
	}
	//チャージ完了できなかったら
	else
	{
		m_brave->SetAllInfoAboutActionFlag(false);
		m_brave->ProcessCommonStateTransition();
	}
}

void Bow::SetAttackHitFlag(bool flag)
{
	m_brave->SetAttackHitFlag(flag);
}

const bool& Bow::GetAttackHitFlag() const
{
	return m_brave->GetAttackHitFlag();
}

void Bow::IsCreatArrow()
{
	//アニメーション終了時に矢を持っていなかったら、矢を生成
	if (m_brave->GetModelRender().IsPlayingAnimation() == false)
	{
		if (GetStockArrowFlag() != true)
		{
			CreateArrow();
		}
	}
}

void Bow::InitModel()
{
	//弓モデルの初期化
	m_modelBow.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);

	//弓と矢の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::MoveArmed()
{
	//弓のワールド座標を設定
	m_bowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);
	//矢のワールド座標を設定
	m_arrowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);

	if (m_arrow != nullptr)
	{
		//矢のステートを設定
		m_arrow->SetWeaponState(enWeaponState_Armed);
	}
	
}

void Bow::MoveStowed()
{
	//弓の座標を設定
	m_bowPos = STOWEDS_POSITION;
	m_modelBow.SetPosition(m_bowPos);

	if (m_arrow != nullptr)
	{
		//矢のステートを設定
		m_arrow->SetWeaponState(enWeaponState_Stowed);
	}

	SetStowedFlag(true);
}

void Bow::ProcessLongRangeAttack()
{
	//矢がないと撃てない
	if (m_arrow != nullptr)
	{
		//矢を発射
		m_arrow->SetShotArrowSetting(
			true,
			m_brave->GetForward(),
			m_arrow->GetPosition(),
			SHOT_ARROW_ANGLE,
			Arrow::enShotPatternState_Normal
		);
		//矢を放ったので、今の矢を保持フラグをリセットする。矢を持っていない状態
		SetStockArrowFlag(false);
		//矢のストックを減らす(耐久値を減らす)
		CalcEndurance(1, false);
		m_arrow = nullptr;
	}
}

void Bow::SkillShot()
{
	if (m_arrow != nullptr)
	{
		//矢を発射
		m_arrow->SetShotArrowSetting(
			true,
			m_brave->GetForward(),
			m_arrow->GetPosition(),
			0.0f,
			Arrow::enShotPatternState_Skill
		);
		//矢を放ったので、今の矢を保持フラグをリセットする。矢を持っていない状態
		SetStockArrowFlag(false);
		m_arrow = nullptr;
	}
}

void Bow::CreateArrow()
{
	//矢のオブジェクトを生成
	m_arrow = NewGO<Arrow>(0, "arrow");
	m_arrow->SetBow(this);
	//矢を持っているので、保持フラグをセット
	SetStockArrowFlag(true);
}

void Bow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelBow.Draw(rc);
}

void Bow::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//遠距離攻撃処理
	if (wcscmp(eventName, L"LongRangeAttack") == 0)
	{
		ProcessLongRangeAttack();
	}

	//スキルを撃った瞬間
	if (wcscmp(eventName, L"SkillShot") == 0)
	{
		SkillShot();
	}

	
}
