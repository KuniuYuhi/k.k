#include "stdafx.h"
#include "DarkSpearObj.h"
#include "InitEffect.h"
#include "GameManager.h"

namespace {
	const float EFFECT_SIZE = 7.0f;

	const float DELETE_TIMETR_LIMMIT = 2.0f;

	const int ATTACK = 25;
}

DarkSpearObj::DarkSpearObj()
{
}

DarkSpearObj::~DarkSpearObj()
{
	if (m_darkSpearEffect != nullptr)
	{
		m_darkSpearEffect->Stop();
	}

	if (m_rangeEffect != nullptr)
	{
		m_rangeEffect->Stop();
	}
}

bool DarkSpearObj::Start()
{
	//攻撃力の設定
	SetAttack(ATTACK);

	PlayEffect();

	m_darkSpearModel.Init("Assets/modelData/character/Lich/DarkSpear.tkm",
		L"Assets/shader/ToonTextrue/lamp_Slime.DDS");
	m_darkSpearModel.SetScale(m_maxScale);
	m_darkSpearModel.SetPosition(m_position);
	m_darkSpearModel.Update();

    return true;
}

void DarkSpearObj::Update()
{
	//勝敗が決まったら削除
	/*if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}*/

	//消去する時間になったら消す
	if (m_deleteTimer > DELETE_TIMETR_LIMMIT)
	{
		DeleteGO(this);
	}
	else
	{
		m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	}

	ChangeSizeDarkSpearObj();

}

void DarkSpearObj::SetMaxScale(EnMaxScaleSize maxScaleSize)
{
	switch (maxScaleSize)
	{
	case DarkSpearObj::enMaxScaleSize_Small:
		m_maxScale *= 1.5f;
		break;
	case DarkSpearObj::enMaxScaleSize_Medium:
		m_maxScale *= 2.0f;
		break;
	case DarkSpearObj::enMaxScaleSize_Large:
		m_maxScale *= 3.0f;
		break;
	default:std::abort();
		break;
	}
}

void DarkSpearObj::SetMaxScale(int sizeNumer)
{
	switch (sizeNumer)
	{
	case DarkSpearObj::enMaxScaleSize_Small:
		m_maxScale *= 1.5f;
		break;
	case DarkSpearObj::enMaxScaleSize_Medium:
		m_maxScale *= 2.0f;
		break;
	case DarkSpearObj::enMaxScaleSize_Large:
		m_maxScale *= 3.0f;
		break;
	default:std::abort();
		break;
	}
}

void DarkSpearObj::PlayEffect()
{
	m_darkSpearEffect = NewGO<EffectEmitter>(0);
	m_darkSpearEffect->Init(InitEffect::enEffect_DarkSpear);
	m_darkSpearEffect->Play();
	m_darkSpearEffect->SetScale(g_vec3One * EFFECT_SIZE);
	m_darkSpearEffect->SetPosition(m_position);
	m_darkSpearEffect->Update();
}

void DarkSpearObj::CreateCollision()
{
	m_collision = NewGO<CollisionObject>(0, GetCollisionName());
	m_collision->CreateMesh(
		g_vec3Zero, 
		g_quatIdentity,
		m_darkSpearModel.GetModel(), 
		m_darkSpearModel.GetModel().GetWorldMatrix()
	);
	//当たり判定を生成した作成者の設定
	m_collision->SetCreatorName(GetName());
	//自動で削除するようにする
	m_collision->SetIsEnableAutoDelete(true);
	m_collision->Update();

}

void DarkSpearObj::ChangeSizeDarkSpearObj()
{
	//現在のサイズを決める
	float mulSize = 0.5f * sin(m_deleteTimer * 3.0f);

	float currentSize = m_maxScale * mulSize * 3.0f;

	m_scale = g_vec3One * currentSize;

	//サイズが0以上なら拡大率の更新と当たり判定を生成
	if (currentSize > 0.0f)
	{
		//当たり判定生成
		CreateCollision();
		//拡大率の更新
		m_darkSpearModel.SetScale(m_scale);
		m_darkSpearModel.Update();
	}
}

void DarkSpearObj::Render(RenderContext& rc)
{
	//ある程度の大きさならモデルを描画
	if (m_scale.y > 0.0f)
	{
		m_darkSpearModel.Draw(rc);
	}
}
