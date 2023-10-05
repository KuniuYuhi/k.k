#include "stdafx.h"
#include "FlamePillar.h"
#include "Wizard.h"
#include "InitEffect.h"

namespace {
    const float CIRCLE_SIZE = 12.0f;
}

FlamePillar::FlamePillar()
{
}

FlamePillar::~FlamePillar()
{
    m_circleEffect->Stop();

    if (m_flamePillarEffect != nullptr)
    {
        m_flamePillarEffect->Stop();
    }
    

    DeleteGO(m_pillarCollision);
}

bool FlamePillar::Start()
{
    m_position = m_wizard->GetPosition();
    m_rotation = m_wizard->GetRotation();

    Vector3 AddPos = Vector3::AxisZ;
    m_rotation.Apply(AddPos);
    //生成する座標の決定
    m_position += AddPos * m_distance;

    Vector3 effectPos = m_position;
    effectPos.y += 5.0f;
    //魔法陣エフェクト再生
    m_circleEffect = NewGO<EffectEmitter>(0);
    m_circleEffect->Init(InitEffect::enEffect_FlamePillar_Circle);
    m_circleEffect->Play();
    m_circleEffect->SetScale(g_vec3One * CIRCLE_SIZE);
    m_circleEffect->SetPosition(effectPos);
    m_circleEffect->Update();

    return true;
}

void FlamePillar::Update()
{
    if (m_startFlamePllarFlag == true)
    {
        StartFlamePillar();
    }

    CalcDamageTime();
    Timer();
}

void FlamePillar::Timer()
{
    //制限時間に達したとき
    if (m_pillarLimitTime < m_pillarTimer)
    {
        DeleteGO(this);
    }
    else
    {
        m_pillarTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void FlamePillar::CalcDamageTime()
{
    if (m_canDamageFlag != true)
    {
        return;
    }

    if (m_canDamageTime < m_canDamageTimer)
    {
        m_canDamageTimer = 0.0f;
        m_canDamageFlag = false;
    }
    else
    {
        m_canDamageTimer += g_gameTime->GetFrameDeltaTime();
    }
}

void FlamePillar::StartFlamePillar()
{
    Vector3 pillarPos = m_position;
    pillarPos.y += 30.0f;

    //当たり判定作成
    m_pillarCollision = NewGO<CollisionObject>(0, "flamepillar");
    m_pillarCollision->CreateCapsule(
        pillarPos,
        Quaternion::Identity,
        15.0f,
        40.0f
    );
    //当たり判定はなし
    m_pillarCollision->SetIsEnableAutoDelete(false);
    m_pillarCollision->SetPosition(pillarPos);
    m_pillarCollision->Update();

    //エフェクト再生
    m_flamePillarEffect = NewGO<EffectEmitter>(0);
    m_flamePillarEffect->Init(InitEffect::enEffect_FlamePillar);
    m_flamePillarEffect->Play();
    m_flamePillarEffect->SetScale(g_vec3One * CIRCLE_SIZE);
    m_flamePillarEffect->SetPosition(m_position);
    m_flamePillarEffect->Update();
    //音再生
    g_soundManager->InitAndPlaySoundSource(enSoundName_FlamePillar, g_soundManager->GetSEVolume());

    //フレイムピラーを出したのでフラグをfalseにする
    m_startFlamePllarFlag = false;
}
