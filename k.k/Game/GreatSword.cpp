#include "stdafx.h"
#include "GreatSword.h"
#include "Brave.h"



namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 BIG_SWORD_COLLISION_SIZE = { 18.0f,150.0f,8.0f };

	const float SKILL_RADIUS = 80.0f;

	const float ADD_FORWARD = 8.0f;

	//ステータス
	const int POWER = 40;

	const float HITTABLE_TIME = 0.5f;

	const float MOVE_FORWARD_SPEED = 150.0f;

	const float NORMAL_ATTACK_1_EFFECT_ANGLE = -270.0f;
	const float NORMAL_ATTACK_2_EFFECT_ANGLE = 80.0f;

	const float NORMAL_ATTACK_1_2_EFFECT_SIZE = 11.0f;
	const float NORMAL_ATTACK_3_EFFECT_SIZE = 18.0f;

	const float SKILL_ATTACK_EFFECT_SIZE = 10.0f;
	const float SKILL_ATTACK_RISING_EFFECT_SIZE = 10.0f;
}

struct IsGroundResult :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//地面とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Ground) {
			//衝突したのは壁ではない。
			isHit = false;
			return 0.0f;
		}
		else
		{
			//地面とぶつかったら。
		//フラグをtrueに。
			isHit = true;
			return 0.0f;
		}
	}
};

GreatSword::GreatSword()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);
}

GreatSword::~GreatSword()
{
	DeleteGO(m_bigSwordCollision);
}

bool GreatSword::Start()
{
	//武器のステータス初期化
	m_status.InitWeaponStatus(GetName());

	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_avoid);

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void GreatSword::Update()
{
	//収納状態の時は処理しない
	if (GetStowedFlag() == true)
	{
		return;
	}

	//多段ヒット可能か判断する。他の武器で処理が途中で終わっているかもしれないから
	m_hitDelection.IsHittable(HITTABLE_TIME);

	MoveWeapon();

	m_modelGreatSword.Update();
	m_bigSwordCollision->Update();
}

void GreatSword::ProcessSkillAttack()
{
	//当たり判定を生成する座標を設定
	m_skillAttackPosition = g_vec3Zero;
	//剣のワールド座標をベクトルに乗算
	m_swordMatrix.Apply(m_skillAttackPosition);
	m_skillAttackPosition.y = 0.0f;
	Vector3 forward;
	forward = m_brave->GetForward();
	forward *= ADD_FORWARD;
	//前方向分を足す
	m_skillAttackPosition += forward;

	//スキル攻撃時の当たり判定の生成
	auto skillCollision = NewGO<CollisionObject>(0, "skillAttack");
	skillCollision->CreateSphere(
		m_skillAttackPosition,
		g_quatIdentity,
		SKILL_RADIUS
	);
	//
	
}

void GreatSword::InitModel()
{
	//剣モデルの初期化
	m_modelGreatSword.Init("Assets/modelData/character/Player/NewHero/BigSowrd.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);

	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void GreatSword::InitCollision()
{
	m_bigSwordCollision = NewGO<CollisionObject>(0, "Attack");
	m_bigSwordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		BIG_SWORD_COLLISION_SIZE
	);
	m_bigSwordCollision->SetIsEnableAutoDelete(false);
	m_bigSwordCollision->SetIsEnable(false);
}

void GreatSword::MoveWeapon()
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

void GreatSword::MoveArmed()
{
	//剣のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();

	m_modelGreatSword.SetWorldMatrix(m_swordMatrix);

	
	//当たり判定の有効化無効化の処理
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_bigSwordCollision->SetIsEnable(true);
	}
	else if (m_bigSwordCollision->IsEnable() != false)
	{
		m_bigSwordCollision->SetIsEnable(false);
	}

	Matrix collisionMatrix = m_swordMatrix;

	//collisionMatrix.

	m_bigSwordCollision->SetWorldMatrix(m_swordMatrix);

}

void GreatSword::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_modelGreatSword.SetPosition(m_swordPos);
	//当たり判定の座標の設定
	m_bigSwordCollision->SetPosition(m_swordPos);
	//当たり判定の無効化
	m_bigSwordCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void GreatSword::PlaySkillAttackEffect()
{
	//エフェクト再生のための座標と回転の設定
	Vector3 pos;
	Quaternion rot;
	SettingEffectInfo(pos, rot, 0.0f);
	//エフェクト再生
	PlayEffect(
		InitEffect::enEffect_GreatSwordSkillAttack,
		pos, SKILL_ATTACK_EFFECT_SIZE
	);
}

void GreatSword::SettingEffectInfo(Vector3& effectPos, Quaternion& rot, float angle)
{
	effectPos = g_vec3Zero;
	m_swordMatrix.Apply(effectPos);
	rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(m_brave->GetForward());
	rot.AddRotationDegZ(angle);
}

void GreatSword::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}
	m_modelGreatSword.Draw(rc);
}

void GreatSword::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//通常攻撃１のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo1Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, NORMAL_ATTACK_1_EFFECT_ANGLE);
		//エフェクト再生
		PlayEffect(InitEffect::enEffect_GreatSwordCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE, rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//通常攻撃２のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo2Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, NORMAL_ATTACK_2_EFFECT_ANGLE);
		//エフェクト再生
		PlayEffect(InitEffect::enEffect_GreatSwordCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE, rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//通常攻撃３のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordPlayCombo3Effect") == 0)
	{
		Vector3 pos;
		Quaternion rot;
		SettingEffectInfo(pos, rot, 0.0f);
		//エフェクト再生
		PlayEffect(InitEffect::enEffect_GreatSwordCombo3,
			pos, NORMAL_ATTACK_3_EFFECT_SIZE, rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordCombo_3,
			g_soundManager->GetSEVolume()
		);
	}
	//スキルの上昇アニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordSkillRising") == 0)
	{
		//エフェクト再生
		PlayEffect(InitEffect::enEffect_GreatSwordSkillRising,
			m_brave->GetPosition(), SKILL_ATTACK_RISING_EFFECT_SIZE
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillRising,
			g_soundManager->GetSEVolume()
		);
	}
	//スキル攻撃のアニメーションキーフレーム
	if (wcscmp(eventName, L"GreatSwordSkillAttack") == 0)
	{
		//メイン武器のスキル攻撃処理
		ProcessSkillAttack();
		//スキル攻撃のエフェクト再生
		PlaySkillAttackEffect();
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_GreatSwordSkillAttack,
			g_soundManager->GetSEVolume()
		);
	}
	
}


