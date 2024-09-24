#include "stdafx.h"
#include "MobEnemyUI.h"

#include "MobEnemyBase.h"

#include "Brave.h"


namespace {
    const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット

    const float WHITE_HP_MUL_SPEED = 90.0f;

    const Vector2 BOSS_HP_CENTER_POS = { 0.0f,000.0f };
    const Vector2 BOSS_HP_FRONT_POS = { -48.0f,000.0f };


    const Vector2 BOSS_HP_FONT_POS = { -200.0f, 486.0f };


    /////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////
    const int SHAKE_STEWNGTH = 10;
    const float SHAKE_VIBRATO = 8.0f;
    const float SHAKE_TIME_LIMIT = 0.2f;
}

bool MobEnemyUI::Start()
{
    m_player = FindGO<Brave>("Brave");

    Init();

    SetShakeInfo(SHAKE_STEWNGTH, SHAKE_VIBRATO, BOSS_HP_CENTER_POS);

    return true;
}

void MobEnemyUI::UIUpdate()
{
    if (m_mobEnemy == nullptr) return;

    UpdateStatusUI();

}

void MobEnemyUI::Draw(RenderContext& rc)
{
    if (m_mobEnemy == nullptr) return;

    if (!m_viewHpGauge) return;

    m_spriteList[enSpriteName_HPBar_Back].Draw(rc);
    m_spriteList[enSpriteName_HPBar_White].Draw(rc);
    m_spriteList[enSpriteName_HPBar_Front].Draw(rc);
    m_spriteList[enSpriteName_HPBar_Frame].Draw(rc);
}

void MobEnemyUI::UpdateStatusUI()
{
    m_viewHpGauge = true;

    //HPバーの処理
    UpdateEnemyHP();
    //HPバーの座標
    UpdateUIPosition();

    Vector3 camera = g_camera3D->GetForward();
    camera.y = 0.0f;
    camera.Normalize();
    Vector3 enemy = m_mobEnemy->GetPosition() - g_camera3D->GetPosition();
    enemy.y = 0.0f;
    enemy.Normalize();

    float cos = Dot(camera, enemy);
    float angle = acos(cos);

    if (fabsf(angle) > Math::DegToRad(50.0f))
    {
        m_viewHpGauge = false;
    }


    m_spriteList[enSpriteName_HPBar_Back].Update();
    m_spriteList[enSpriteName_HPBar_White].Update();
    m_spriteList[enSpriteName_HPBar_Front].Update();
    m_spriteList[enSpriteName_HPBar_Frame].Update();
}

void MobEnemyUI::UpdateUIPosition()
{
    Vector3 position = m_mobEnemy->GetPosition();

    position.y = 80.0f;

    Vector2 finalPos = g_vec2Zero;

    //座標をワールド座標からスクリーン座標に変換
    g_camera3D->CalcScreenPositionFromWorldPosition(finalPos, position);

    //座標を設定
    m_currentShakeCenterPosition = finalPos;

    m_currentShakeGaugePosition = finalPos;
    m_currentShakeGaugePosition.x += BOSS_HP_FRONT_POS.x;

    m_spriteList[enSpriteName_HPBar_Frame].SetPosition(m_currentShakeCenterPosition);
    m_spriteList[enSpriteName_HPBar_Back].SetPosition(m_currentShakeCenterPosition);

    m_spriteList[enSpriteName_HPBar_Front].SetPosition(m_currentShakeGaugePosition);
    m_spriteList[enSpriteName_HPBar_White].SetPosition(m_currentShakeGaugePosition);

}


void MobEnemyUI::UpdateEnemyHP()
{
    //最大HPと現在のHPを取得
    int maxHP = m_mobEnemy->GetCommonStatus().GetMaxHp();
    int currentHP = m_mobEnemy->GetCommonStatus().GetCurrentHp();

    UpdateMainHPBar(currentHP, maxHP);

    UpdateWhiteHPBar(currentHP, maxHP);
}

void MobEnemyUI::UpdateMainHPBar(int currentHp, int maxHp)
{
    //HPバーの減っていく割合。
    Vector3 HpScale = CalcGaugeRatio(maxHp, currentHp);
    //拡大率を設定
    m_spriteList[enSpriteName_HPBar_Front].SetScale(HpScale);
    //更新
    m_spriteList[enSpriteName_HPBar_Front].Update();
}

void MobEnemyUI::UpdateWhiteHPBar(int currentHp, int maxHp)
{
    //白いHPが現在のHPより小さいなら//白いHPに現在のHPを代入する
    if (m_whiteHP <= currentHp)
    {
        m_whiteHP = currentHp;
    }
    else
    {
        m_whiteHP -= g_gameTime->GetFrameDeltaTime() * WHITE_HP_MUL_SPEED;
    }

    Vector3 whiteBarScale = CalcGaugeRatio(maxHp, m_whiteHP);

    m_spriteList[enSpriteName_HPBar_White].SetScale(whiteBarScale);

    m_spriteList[enSpriteName_HPBar_White].Update();
}

void MobEnemyUI::DeleteSelf()
{
    DeleteGO(this);
}

void MobEnemyUI::SettingTargetEnemy(MobEnemyBase* mobEnemy)
{
    //インスタンスを設定
    m_mobEnemy = mobEnemy;

    if (m_mobEnemy != nullptr)
    {
        //モブエネミーのインスタンスがあるならHPを設定
        m_whiteHP = m_mobEnemy->GetCommonStatus().GetCurrentHp();
    }

}

void MobEnemyUI::Init()
{
    InitSpriteRenders();
}

void MobEnemyUI::InitSpriteRenders()
{
    m_currentShakeCenterPosition = BOSS_HP_CENTER_POS;

    m_currentShakeGaugePosition = BOSS_HP_FRONT_POS;

    //HPフレームの初期化
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Frame],
        "Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 100, 20, 
        BOSS_HP_CENTER_POS);
    //////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの表
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Front],
        "Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 96, 16, 
        BOSS_HP_FRONT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_Front].SetPivot(GAUGE_PIBOT);
    ///////////////////////////////////////////////////////////////////////////////////////////
    //HPが減った時に出てくる白いHPバー
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_White],
        "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 96, 16, 
        BOSS_HP_FRONT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_White].SetPivot(GAUGE_PIBOT);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの裏側
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Back],
        "Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 99, 19, 
        BOSS_HP_CENTER_POS);


    m_spriteList[enSpriteName_HPBar_Back].Update();
    m_spriteList[enSpriteName_HPBar_White].Update();
    m_spriteList[enSpriteName_HPBar_Front].Update();
    m_spriteList[enSpriteName_HPBar_Frame].Update();
}



