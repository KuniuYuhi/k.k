#include "stdafx.h"
#include "DarkBall.h"
#include "InitEffect.h"

#include "Player.h"

namespace {
    const int ADD_CREATE_POS = 100;
    const float SPEED = 600.0f;
    const float RADIUS = 45.0f;
    const float Y_UP = 60.0f;

    const float EXPLOSION_SIZE = 9.0f;

    const float DARK_BALL_SIZE = 12.0f;

    const float DELETE_LENGTH = 1200.0f;

    const float HIT_LENGTH = 80.0f;

    const int ATTACK = 20;

}

DarkBall::DarkBall()
{
    m_speed = SPEED;
}

DarkBall::~DarkBall()
{
    if (m_BallCollision != nullptr)
    {
        DeleteGO(m_BallCollision);
    }

    if (m_darkBallEffect != nullptr)
    {
        m_darkBallEffect->Stop();
    }
}

bool DarkBall::Start()
{
    //プレイヤーのインスタンスを探す
    m_player = FindGO<Player>("player");

    //移動方向の設定
    SetMoveSpeed();
    //生成する座標の決定
    m_position += m_moveSpeed * ADD_CREATE_POS;
    //速度を決める
    m_moveSpeed *= SPEED;
    //生成開始座標を設定
    SetStartPosition(m_position);

    //当たり判定の設定
    SettingCollision();
    //エフェクト再生
    PlayDarkBallEffect();
    
    //ダークボール生成音再生
    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall, g_soundManager->GetSEVolume());

	return true;
}

void DarkBall::Update()
{
    //ヒットしたら爆発する
    if (m_hitFlag == true)
    {
        //爆発処理
        Explosion();
        return;
    }

    //削除する距離に達してないなら
    if (IsForceDelete(m_position, DELETE_LENGTH) != true)
    {
        //移動処理
        Move();
        //プレイヤーにヒットしたか(距離判定)
        if (IsHit() == true)
        {
            //ヒットした
            m_hitFlag = true;
        }
    }
    else
    {
        //削除する
        //爆発処理
        Explosion();
    }
}

void DarkBall::Move()
{
    //直進処理
    MoveStraight();
  
    //当たり判定をオブジェクトと同じ座標に移動
    m_position.y = Y_UP;

    //エフェクトの移動の設定と更新
    m_darkBallEffect->SetPosition(m_position);
    m_darkBallEffect->Update();
    //当たり判定の移動の設定と更新
    m_BallCollision->SetPosition(m_position);
    m_BallCollision->Update();
}

void DarkBall::SettingCollision()
{
    //当たり判定の座標の設定
    //m_collisionPosition = m_position;

    //当たり判定作成
    m_BallCollision = NewGO<CollisionObject>(0, GetCollisionName());
    m_BallCollision->CreateSphere(
        m_position,
        Quaternion::Identity,
        RADIUS
    );
    //当たり判定を生成した作成者の設定
    m_BallCollision->SetCreatorName(GetName());
    //自動で削除しないようにする
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y = Y_UP;
    m_BallCollision->SetPosition(m_position);
    m_BallCollision->Update();
}

void DarkBall::Explosion()
{
    //ダークボールのエフェクト停止
    m_darkBallEffect->Stop();
    //爆発音再生
    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall_Explosion, g_soundManager->GetSEVolume());

    //爆発エフェクト再生
    EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
    explosionEffect->Init(InitEffect::enEffect_DarkBall_Explosion);
    explosionEffect->Play();
    explosionEffect->SetScale(g_vec3One * EXPLOSION_SIZE);
    explosionEffect->SetPosition(m_position);
    explosionEffect->Update();
    //自身を削除
    DeleteGO(this);
}

void DarkBall::PlayDarkBallEffect()
{
    //エフェクトの設定
    m_darkBallEffect = NewGO<EffectEmitter>(0, "DarkBall");
    m_darkBallEffect->Init(InitEffect::enEffect_DarkBall);
    m_darkBallEffect->Play();
    m_darkBallEffect->SetScale(g_vec3One * DARK_BALL_SIZE);
    m_darkBallEffect->SetPosition(m_position);
    m_darkBallEffect->SetRotation(m_rotation);
    m_darkBallEffect->Update();
}

bool DarkBall::IsHit()
{
    //自身の座標からプレイヤーい向かうベクトルを計算
    Vector3 diff = m_player->GetPosition() - m_position;
    //ベクトルの長さが一定値以下なら
    if (diff.Length() < HIT_LENGTH)
    {
        //当たった
        return true;
    }
    //当たらなかった
    return false;
}
