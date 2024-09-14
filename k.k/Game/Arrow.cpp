#include "stdafx.h"

#include "Arrow.h"
#include "Brave.h"
#include "Bow.h"
#include "DamageProvider.h"
#include "KnockBackInfoManager.h"


#include "UseEffect.h"

using namespace KnockBackInfo;


namespace {
	const float NORMAL_SHOT_ARROW_Y_ROT_OFFSET = 180.0f;	//通常攻撃の矢の回転のオフセット
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	if (m_arrowCollision != nullptr)
	{
		DeleteGO(m_arrowCollision);
	}

}

bool Arrow::Start()
{
	m_brave = FindGO<Brave>("Brave");

	ArrowStatus normalStatus,skillStatus;
	normalStatus.InitArrowStatus("Normal");
	skillStatus.InitArrowStatus("Skill");

	m_statusMap.insert(std::make_pair(enNormalShot, normalStatus));
	m_statusMap.insert(std::make_pair(enSkillShot, skillStatus));


	Init();

	InitComponent();

	return true;
}

void Arrow::Update()
{
	ManageShotPatternState();

	m_damageProvider->UpdateComponent();

	//モデルの更新処理。TRSは移動時に設定
	m_arrowModelRender.Update();
}

void Arrow::Init()
{
	//矢モデルの初期化
	m_arrowModelRender.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//矢の真ん中と先端のボーンIDを取得
	m_arrowCentorBoonId = m_arrowModelRender.FindBoneID(L"Center");
	m_arrowTipBoonId = m_arrowModelRender.FindBoneID(L"tip");

	//矢を持つ座標のボーンIDを取得
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Arrow::InitComponent()
{
	AddComponent<DamageProvider>();
	m_damageProvider = GetComponent<DamageProvider>();
}

void Arrow::ManageShotPatternState()
{
	switch (m_enShotPatternState)
	{
	case Arrow::enNormalShot:
		UpdateNormalShotState();
		break;
	case Arrow::enSkillShot:
		UpdateSkillShotState();
		break;
	case Arrow::enNone:
		UpdateNoneState();
		break;
	default:
		break;
	}
}

void Arrow::UpdateNormalShotState()
{
	if (m_deleteTimer >= 2.0f)
	{
		DeleteArrow();
		return;
	}

	//当たり判定処理による削除
	if (m_damageProvider->IsHit())
	{
		DeleteArrow();
		return;
	}


	m_deleteTimer += g_gameTime->GetFrameDeltaTime();

	ShotArrowMove(enNormalShot);
}

void Arrow::UpdateSkillShotState()
{

	if (m_deleteTimer >= 2.0f)
	{
		DeleteArrow();
	}


	//一定時間ごとにダメージIDを変更して多段ヒットできるようにする
	if (m_attackInfoUpdateTimer > m_attackInfoUpdateTimeLimit)
	{
		//IDを新しく設定
		m_damageProvider->SetAttackId(KnockBackInfoManager::GetInstance()->GetAddAttackId());
		//タイマーリセット
		m_attackInfoUpdateTimer = 0.0f;
	}

	//各種タイマー加算
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	m_attackInfoUpdateTimer+= g_gameTime->GetFrameDeltaTime();

	ShotArrowMove(enSkillShot);

}

void Arrow::UpdateNoneState()
{
	//収納状態なら移動させない
	if (m_enWeaponState == enStowed) return;

	//プレイヤーの手に追従
	MoveArmed();


}

void Arrow::ShotArrowMove(EnShotPatternState shotPattern)
{
	Vector3 moveSpeed = m_forward;
	moveSpeed *= m_statusMap.at(shotPattern).GetArrowSpeed();

	//座標を加算
	m_arrowModelRender.AddPosition(moveSpeed);
	//弓の真ん中のワールド座標を取得
	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();
	//コリジョンにワールド座標を設定、更新
	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);
	m_arrowCollision->Update();

	//エフェクトがないなら処理しない
	if (m_arrowEffect == nullptr) return;

	//初期位置を計算
	Vector3 position = g_vec3Zero;
	m_arrowCenterMatrix.Apply(position);
	//移動座標を設定
	m_arrowEffect->SetMovePosition(position);

}

void Arrow::FixedAttaackArrowTransform()
{
	//ワールド座標をローカル座標に代入
	m_moveAttackArrowPosition.x = m_arrowMatrix.m[3][0];
	m_moveAttackArrowPosition.y = m_arrowMatrix.m[3][1];
	m_moveAttackArrowPosition.z = m_arrowMatrix.m[3][2];
	//前方向のほうに回転
	Quaternion rotation;
	rotation.SetRotationYFromDirectionXZ(m_forward);
	//このままだと回転がおかしいのでオフセットをかける
	rotation.AddRotationDegY(NORMAL_SHOT_ARROW_Y_ROT_OFFSET);
	//トランスフォームの設定
	m_arrowModelRender.SetTransform(m_moveAttackArrowPosition, rotation, g_vec3One);
}

void Arrow::CreateCollision(
	EnShotPatternState shotPatternState, Vector3 createPosition, Quaternion rotation)
{
	//矢の通常攻撃用の当たり判定の生成
	m_arrowCollision = NewGO<CollisionObject>(
		0, g_collisionObjectManager->m_playerAttackCollisionName);
	//コリジョンの制作者を自身に設定
	m_arrowCollision->SetCreatorName(GetName());
	//コリジョンの形状はボックス
	m_arrowCollision->CreateBox(
		createPosition,
		rotation,
		m_statusMap.at(shotPatternState).GetCollisionSize()
	);
	m_arrowCollision->SetIsEnableAutoDelete(false);
	//矢の真ん中のボーンを持ってくる
	m_arrowCenterMatrix = m_arrowModelRender.GetBone(m_arrowCentorBoonId)->GetWorldMatrix();
	m_arrowCollision->SetWorldMatrix(m_arrowCenterMatrix);
	m_arrowCollision->Update();

}

bool Arrow::IsHitCollision()
{


	return false;
}

void Arrow::PlayArrowEffect(EnShotPatternState shotpatternState)
{
	//初期位置を計算
	Vector3 position = g_vec3Zero;
	m_arrowCenterMatrix.Apply(position);

	//回転方向は前方向のほう
	Quaternion rot = Quaternion::Identity;
	rot.SetRotationYFromDirectionXZ(m_forward);

	//エフェクト番号
	EnEFK effect = enEffect_ArrowCharge1;

	float mulScale = 1.0f;

	if (shotpatternState == enNormalShot)
	{
		effect = enEffect_Arrow;
	}
	else
	{
		effect = enEffect_BowArrowSkillShot;
	}

	
	//
	m_arrowEffect = NewGO<UseEffect>(0, "ArrowEffect");
	m_arrowEffect->PlayEffect(effect,
		position, g_vec3One * 15.0f, rot, false);



}

void Arrow::DeleteArrow()
{
	//エフェクトを削除する
	if (m_arrowEffect != nullptr)
	{
		m_arrowEffect->Delete();
		m_arrowEffect = nullptr;
	}

	DeleteGO(this);

}

void Arrow::SetShotArrowParameters(
	EnShotPatternState shotpatternState, Vector3 forward)
{
	//攻撃パターンを設定
	m_enShotPatternState = shotpatternState;
	//前方向の設定と正規化
	m_forward = forward;
	m_forward.Normalize();

	//矢のトランスフォームの修正
	FixedAttaackArrowTransform();
	
	//通常攻撃かスキル攻撃に応じた当たり判定の作成
	CreateCollision(
		shotpatternState,
		m_arrowModelRender.GetPosition(),
		m_arrowModelRender.GetRotation()
	);

	//エフェクトを生成
	PlayArrowEffect(shotpatternState);

}

void Arrow::ChangeStowed()
{
	SetWeaponState(enStowed);
	//武器の位置を変更
	MoveStowed();
}

void Arrow::ChangeArmed()
{
	SetWeaponState(enArmed);
}

void Arrow::MoveStowed()
{
	//収納状態の座標に設定
	m_arrowModelRender.SetPosition(m_stowedPosition);
}

void Arrow::MoveArmed()
{
	m_arrowMatrix = m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	//ワールド座標を設定
	m_arrowModelRender.SetWorldMatrix(m_arrowMatrix);

}

void Arrow::Render(RenderContext& rc)
{
	m_arrowModelRender.Draw(rc);
}
