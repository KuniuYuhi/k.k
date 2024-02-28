#include "stdafx.h"
#include "DarkWall.h"
#include "InitEffect.h"

#include "Summoner.h"

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
    //ボスのインスタンスを探す
    m_summoner = FindGO<Summoner>("summoner");
    //攻撃力の設定
    SetAttack(m_summoner->GetStatus().GetAtk());

    //ボーンIDを取得
    m_darkWallBoonId = m_summoner->GetDarkWallBoonId();

    m_position = AppltMatrixToPosition();
    m_rotation = m_summoner->GetRotation();
    
    //当たり判定生成
    CreateCollision();
    //エフェクトの再生
    PlayDarkWallEffect();

    return true;
}

void DarkWall::Update()
{
    //消去する時間に達したら削除
    if (m_deleteTime < m_deleteTimer)
    {
        DeleteGO(this);
    }
    else
    {
        //タイマーを加算
        m_deleteTimer += g_gameTime->GetFrameDeltaTime();
    }


    //当たり判定のＹ座標を下げる
    m_collisionPosition.y -= DOWN;
    //ボーンのワールド行列を座標に乗算
    m_position = AppltMatrixToPosition();
    //ボスの回転を設定
    m_rotation = m_summoner->GetRotation();

    //エフェクトの座標、回転の設定と更新
    m_darkWallEffect->SetRotation(m_rotation);
    m_darkWallEffect->SetPosition(m_position);
    m_darkWallEffect->Update();
    //当たり判定の座標の設定と更新
    m_collision->SetPosition(m_collisionPosition);
    m_collision->Update();
}

void DarkWall::PlayDarkWallEffect()
{
    m_darkWallEffect = NewGO<EffectEmitter>(0);
    m_darkWallEffect->Init(InitEffect::enEffect_DarkWall);
    m_darkWallEffect->Play();
    m_darkWallEffect->SetPosition(m_position);
    m_darkWallEffect->SetScale(g_vec3One * EFFECT_SCALE);
    m_darkWallEffect->SetRotation(m_rotation);
    m_darkWallEffect->Update();
}

void DarkWall::CreateCollision()
{
    //当たり判定用の座標取得
    m_collisionPosition = m_position;
    //当たり判定作成
    m_collision = NewGO<CollisionObject>(0,GetCollisionName());
    m_collision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        80.0f
    );
    //当たり判定を生成した作成者の設定
    m_collision->SetCreatorName(GetName());
    //自動で削除するようにする
    m_collision->SetIsEnableAutoDelete(false);
    
    m_collision->SetPosition(m_collisionPosition);
    m_collision->Update();
}

Vector3 DarkWall::AppltMatrixToPosition()
{
    Vector3 pos = g_vec3Zero;
    //ボーンの座標の取得
    Matrix matrix = m_summoner->GetModelRender().GetBone(m_darkWallBoonId)->GetWorldMatrix();
    //ベクトルに行列を乗算
    matrix.Apply(pos);
    return pos;
}
