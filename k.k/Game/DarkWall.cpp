#include "stdafx.h"
#include "DarkWall.h"
#include "Lich.h"
#include "InitEffect.h"

namespace {
    const Vector3 SCALE = { 2.0f,2.0f,2.0f };
    const float EFFECT_SCALE = 10.0f;
    const float DOWN = 3.0f;

    const Vector3 ADD_POS = { 0.0f,80.0f,0.0f };
}

DarkWall::DarkWall()
{
}

DarkWall::~DarkWall()
{
    DeleteGO(m_collision);
    m_darkWallEffect->Stop();
}

bool DarkWall::Start()
{
    //攻撃力を設定
    m_attak = m_lich->GetStatus().atk;

    m_position = AppltMatrixToPosition();
    m_rotation = m_lich->GetRotation();
    //当たり判定用の座標取得
    m_collisionPosition = m_position;
    //当たり判定作成
    m_collision = NewGO<CollisionObject>(0, "DarkWall");
    m_collision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        80.0f
    );
    m_collision->SetIsEnableAutoDelete(false);
    m_collision->SetPosition(m_collisionPosition);
    m_collision->Update();
    
    m_darkWallEffect = NewGO<EffectEmitter>(0);
    m_darkWallEffect->Init(InitEffect::enEffect_DarkWall);
    m_darkWallEffect->Play();
    m_darkWallEffect->SetPosition(m_position);
    m_darkWallEffect->SetScale(g_vec3One * EFFECT_SCALE);
    m_darkWallEffect->SetRotation(m_rotation);
    m_darkWallEffect->Update();

    return true;
}

void DarkWall::Update()
{
    if (m_deleteTime < m_deleteTimer)
    {
        DeleteGO(this);
    }
    else
    {
        m_deleteTimer += g_gameTime->GetFrameDeltaTime();
    }

    m_collisionPosition.y -= DOWN;

    m_position = AppltMatrixToPosition();
    m_rotation = m_lich->GetRotation();


    m_darkWallEffect->SetRotation(m_rotation);
    m_darkWallEffect->SetPosition(m_position);
    m_darkWallEffect->Update();

    m_collision->SetPosition(m_collisionPosition);
    m_collision->Update();
}

Vector3 DarkWall::AppltMatrixToPosition()
{
    Vector3 pos = g_vec3Zero;
    //ボーンの座標の取得
    Matrix matrix = m_lich->GetModelRender().GetBone(m_lich->GetDarkWallBoonId())->GetWorldMatrix();
    //ベクトルに行列を乗算
    matrix.Apply(pos);
    return pos;
}
