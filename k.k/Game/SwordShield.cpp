#include "stdafx.h"
#include "SwordShield.h"
#include "Brave.h"

#include "IAttackObject.h"


namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };

	const Vector3 SWORD_COLLISION_SIZE = { 12.0f,150.0f,5.0f };
	const Vector3 SHIELD_COLLISION_SIZE = { 22.0f,40.0f,16.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	//ステータス
	const int POWER = 0;
	const int SKILL_POWER = 50;
	const int ENDURANCE = 70;		//武器の耐久力(盾の耐久力)。

	const float MOVE_FORWARD_SPEED = 200.0f;

	const float ATTACK_1COMBO_KNOCKBACK_POWER = 140.0f;
	const float ATTACK_2COMBO_KNOCKBACK_POWER = 140.0f;
	const float ATTACK_3COMBO_KNOCKBACK_POWER = 140.0f;
	const float SKILL_KNOCKBACK_POWER = 250.0f;

	const float NORMAL_ATTACK_1_EFFECT_ANGLE = 225.0f;
	
	const float NORMAL_ATTACK_1_2_EFFECT_SIZE = 11.0f;
	const float NORMAL_ATTACK_3_EFFECT_SIZE = 18.0f;

	const float SKILL_ATTACK_EFFECT_SIZE = 17.0f;
	const float SKILL_ATTACK_RISING_EFFECT_SIZE = 10.0f;

	const float HITTABLE_TIME = 0.1f;
}

SwordShield::SwordShield()
{
	SetMoveForwardSpeed(MOVE_FORWARD_SPEED);
	SetWeaponPower(POWER);

	m_knockPower_1combo = ATTACK_1COMBO_KNOCKBACK_POWER;
	m_knockPower_2combo = ATTACK_2COMBO_KNOCKBACK_POWER;
	m_knockPower_3combo = ATTACK_3COMBO_KNOCKBACK_POWER;
	m_knockPower_Skill = SKILL_KNOCKBACK_POWER;
}

SwordShield::~SwordShield()
{
	DeleteGO(m_swordCollision);
	DeleteGO(m_shieldCollision);
}

bool SwordShield::Start()
{
	//武器のステータス初期化
	m_status.InitWeaponStatus(
		POWER, SKILL_POWER, ENDURANCE
	);

	m_status.InitWeaponStatus(GetName());

	//勇者のインスタンスを探す
	m_brave = FindGO<Brave>("brave");

	InitModel();
	InitCollision();

	//防御タイプの設定
	SetEnDefendTipe(enDefendTipe_Defence);

	//アニメーションイベント用の関数を設定する。
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void SwordShield::Update()
{
	//収納状態なら
	if (GetStowedFlag() == true)
	{
		return;
	}

	MoveWeapon();

	//多段ヒット可能か判断する。他の武器で処理が途中で終わっているかもしれないから
	m_hitDelection.IsHittable(HITTABLE_TIME);

	m_modelSword.Update();
	m_modelShield.Update();
	m_swordCollision->Update();
	m_shieldCollision->Update();
}

bool SwordShield::IsHitCollision()
{
	//アタックオブジェクトの当たり判定
	const auto& DarkBallCollisions =
		g_collisionObjectManager->FindCollisionObjects("attackobject");
	//コリジョンの配列をfor文で回す
	for (auto collision : DarkBallCollisions)
	{
		//自身の当たり判定と衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{
			//当たった
			return true;
		}
	}

	//モンスターの攻撃の当たり判定
	const auto& MonsterCollisions = g_collisionObjectManager->FindCollisionObjects("monsterattack");
	//コリジョンの配列をfor文で回す
	for (auto collision : MonsterCollisions)
	{
		//自身の当たり判定と衝突したら
		if (collision->IsHit(m_shieldCollision) == true)
		{
			//当たった
			return true;
		}
	}

	//ここまで来たら当たってない
	return false;
}

void SwordShield::ProcessSkillAttack()
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
}

void SwordShield::InitModel()
{
	//剣モデルの初期化
	m_modelSword.Init("Assets/modelData/character/Player/NewHero/OneHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	
	//盾モデルの初期化
	m_modelShield.Init("Assets/modelData/character/Player/NewHero/Shield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ);
	//剣と盾の座標に対応するボーンIDを取得
	//装備状態の時のボーンID
	m_armedSwordBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");
	m_armedShieldBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
}

void SwordShield::InitCollision()
{
	InitSwordCollision();
	InitShieldCollision();
}

void SwordShield::InitSwordCollision()
{
	//剣の当たり判定
	m_swordCollision = NewGO<CollisionObject>(0, "Attack");
	/*m_swordCollision->CreateMesh(
		g_vec3Zero,
		g_quatIdentity,
		m_modelSword.GetModel(),
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix()
	);*/
	m_swordCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SWORD_COLLISION_SIZE
	);
	//当たり判定を自動で削除しないようにする
	m_swordCollision->SetIsEnableAutoDelete(false);
	//当たり判定を無効化
	m_swordCollision->SetIsEnable(false);
}

void SwordShield::InitShieldCollision()
{
	//盾の当たり判定
	m_shieldCollision = NewGO<CollisionObject>(0, "defence");
	m_shieldCollision->CreateBox(
		STOWEDS_POSITION,
		Quaternion(0.0f, 90.0f, 180.0f, 1.0f),
		SHIELD_COLLISION_SIZE
	);
	m_shieldCollision->SetIsEnableAutoDelete(false);
	m_shieldCollision->SetIsEnable(false);
}

void SwordShield::MoveWeapon()
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

void SwordShield::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//剣と盾のワールド座標を設定
	m_swordMatrix =
		m_brave->GetModelRender().GetBone(m_armedSwordBoonId)->GetWorldMatrix();
	m_modelSword.SetWorldMatrix(m_swordMatrix);


	m_shieldMatrix =
		m_brave->GetModelRender().GetBone(m_armedShieldBoonId)->GetWorldMatrix();
	m_modelShield.SetWorldMatrix(m_shieldMatrix);


	//防御可能かつYボタンを押している間は防御
	if (GetIsDefendEnableFlag()!=false && 
		g_pad[0]->IsPress(enButtonY) == true)
	{
		//防御している間は回転可能
		SetRotationDelectionFlag(true);
	}
	else
	{
		//防御していないのでフラグをリセット
		SetRotationDelectionFlag(false);
	}



	//当たり判定の有効化無効化の処理
	if (m_brave->GetIsCollisionPossibleFlag() == true)
	{
		m_swordCollision->SetIsEnable(true);
	}
	else if (m_swordCollision->IsEnable() != false)
	{
		m_swordCollision->SetIsEnable(false);
	}

	m_swordCollision->SetWorldMatrix(m_swordMatrix);
	m_shieldCollision->SetWorldMatrix(m_shieldMatrix);
}

void SwordShield::MoveStowed()
{
	m_swordPos = STOWEDS_POSITION;
	m_shieldPos = STOWEDS_POSITION;

	m_modelSword.SetPosition(m_swordPos);
	m_modelShield.SetPosition(m_shieldPos);
	//当たり判定の座標の設定
	m_swordCollision->SetPosition(m_swordPos);
	m_shieldCollision->SetPosition(m_shieldPos);
	//当たり判定の無効化
	m_swordCollision->SetIsEnable(false);
	m_shieldCollision->SetIsEnable(false);
	SetStowedFlag(true);
}

void SwordShield::SettingEffectInfo(
	Vector3& effectPos, Quaternion& rot, float angle)
{
	effectPos = g_vec3Zero;
	Vector3 forwardPos = m_brave->GetForward();
	m_swordMatrix.Apply(effectPos);
	rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(forwardPos);
	rot.AddRotationDegZ(angle);
}

void SwordShield::PlaySkillAttackEffect()
{
	//エフェクト再生
	PlayEffect(
		InitEffect::enEffect_SwordShieldSkillAttack,
		m_skillAttackPosition, SKILL_ATTACK_EFFECT_SIZE
	);
	//スキルの音再生
	g_soundManager->InitAndPlaySoundSource(
		enSoundName_SwordShieldSkillAttack,
		g_soundManager->GetSEVolume()
	);
}

void SwordShield::Render(RenderContext& rc)
{
	//収納状態なら表示しない
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelSword.Draw(rc);

	//防御可能でないなら表示しない
	if (GetIsDefendEnableFlag() != true)
	{
		return;
	}
	m_modelShield.Draw(rc);
}

void SwordShield::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//通常攻撃１のアニメーションキーフレーム
	if (wcscmp(eventName, L"PlayCombo1Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		Vector3 forwardPos = m_brave->GetForward();
		SettingEffectInfo(pos, rot, NORMAL_ATTACK_1_EFFECT_ANGLE);
		forwardPos *= 15.0f;
		pos.Add(forwardPos);
		//エフェクト再生
		PlayEffect(
			InitEffect::enEffect_SwordShieldCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE,rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//通常攻撃２のアニメーションキーフレーム
	if (wcscmp(eventName, L"PlayCombo2Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(pos, rot, 0.0f);
		//エフェクト再生
		PlayEffect(
			InitEffect::enEffect_SwordShieldCombo12,
			pos, NORMAL_ATTACK_1_2_EFFECT_SIZE, rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldCombo_1_2,
			g_soundManager->GetSEVolume()
		);
	}
	//通常攻撃３のアニメーションキーフレーム
	if (wcscmp(eventName, L"PlayCombo3Effect") == 0)
	{
		//エフェクト再生のための座標と回転設定
		Vector3 pos = g_vec3Zero;
		Quaternion rot = g_quatIdentity;
		SettingEffectInfo(pos, rot, 0.0f);
		//エフェクト再生
		PlayEffect(
			InitEffect::enEffect_SwordShieldCombo3,
			pos, NORMAL_ATTACK_3_EFFECT_SIZE,rot
		);
		//音再生
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_SwordShieldCombo_3,
			g_soundManager->GetSEVolume()
		);
	}
	//スキル攻撃のアニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldSkillAttack") == 0)
	{
		//メイン武器のスキル攻撃処理
		ProcessSkillAttack();
		//スキル攻撃のエフェクト再生
		PlaySkillAttackEffect();
	}
	//スキルの上昇アニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldSkillRising") == 0)
	{
		//エフェクト再生
		PlayEffect(
			InitEffect::enEffect_SwordShieldSkillRising,
			m_brave->GetPosition(), SKILL_ATTACK_RISING_EFFECT_SIZE
		);
	}
	//防御ヒットのアニメーションキーフレーム
	if (wcscmp(eventName, L"SwordShieldDifendHit") == 0)
	{
		Vector3 pos = g_vec3Zero;
		m_swordMatrix.Apply(pos);
		Quaternion rot = g_quatIdentity;
		rot.SetRotationYFromDirectionXZ(m_brave->GetForward());
		rot.AddRotationDegZ(225.0f);

		//メイン武器のスキル攻撃処理
		EffectEmitter* hitEffect = NewGO<EffectEmitter>(0);
		hitEffect->Init(InitEffect::enEffect_SwordShieldCombo12);
		hitEffect->Play();
		hitEffect->SetPosition(pos);
		hitEffect->SetScale(g_vec3One * 11.0f);
		hitEffect->SetRotation(rot);
		hitEffect->Update();
	}

}
