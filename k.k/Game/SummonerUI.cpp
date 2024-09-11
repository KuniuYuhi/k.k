#include "stdafx.h"
#include "SummonerUI.h"

#include "Summoner.h"

namespace {
    //////////////////////////////////////////////////////////////////
    //ゲージ
    ///////////////////////////////////////////////////////////////////
    const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット

    const float WHITE_HP_MUL_SPEED = 60.0f;

    const Vector2 BOSS_HP_CENTER_POS = { 0.0f,450.0f };
    const Vector2 BOSS_HP_FRONT_POS = { -489.0f,450.0f };


    const Vector2 BOSS_HP_FONT_POS = { -200.0f, 486.0f };


    /////////////////////////////////////////////////////////////////////
    //
    /////////////////////////////////////////////////////////////////////
    const int SHAKE_STEWNGTH = 15;
    const float SHAKE_VIBRATO = 12.0f;
    const float SHAKE_TIME_LIMIT = 0.4f;

}

bool SummonerUI::Start()
{
    m_summmoner = FindGO<Summoner>("Summoner");

    m_oldHpValue = m_summmoner->GetCommonStatus().GetCurrentHp();

    Init();

    SetShakeInfo(SHAKE_STEWNGTH, SHAKE_VIBRATO, BOSS_HP_CENTER_POS);

    return true;
}

void SummonerUI::UIUpdate()
{
    UpdateStatusUI();
  

}

void SummonerUI::Draw(RenderContext& rc)
{
    m_spriteList[enSpriteName_HPBar_Back].Draw(rc);
    m_spriteList[enSpriteName_HPBar_White].Draw(rc);
    m_spriteList[enSpriteName_HPBar_Front].Draw(rc);
    m_spriteList[enSpriteName_HPBar_Frame].Draw(rc);

}

void SummonerUI::UpdateStatusUI()
{
    //HPバーの処理
    UpdateSummonerHP();

    //Hpが変動したらバーを揺らす
    if (m_isVariableHp)
    {
        ShakeHPBar();
    }
}

void SummonerUI::UpdateSummonerHP()
{
    //最大HPと現在のHPを取得
    int maxHP = m_summmoner->GetCommonStatus().GetMaxHp();
    int currentHP = m_summmoner->GetCommonStatus().GetCurrentHp();

    UpdateMainHPBar(currentHP, maxHP);

    UpdateWhiteHPBar(currentHP, maxHP);

    m_oldHpValue = currentHP;
}

void SummonerUI::UpdateMainHPBar(int currentHp, int maxHp)
{
    //HPが変動したら
    if (m_oldHpValue != currentHp)
    {
        m_shakeTimer = 0.0f;
        m_isVariableHp = true;
        
    }
    else
    {
        //HPが変わっていないなら処理する必要ない
        return;
    }

    //HPバーの減っていく割合。
    Vector3 HpScale = CalcGaugeRatio(maxHp, currentHp);
    //拡大率を設定
    m_spriteList[enSpriteName_HPBar_Front].SetScale(HpScale);
    //更新
    m_spriteList[enSpriteName_HPBar_Front].Update();
}

void SummonerUI::UpdateWhiteHPBar(int currentHp, int maxHp)
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

void SummonerUI::ShakeHPBar()
{
    m_currentShakeCenterPosition =
        GetUpdateShakePosition(m_shakeTimer, SHAKE_TIME_LIMIT, m_currentShakeCenterPosition);

    Vector2 gaugeAddPostion;
    gaugeAddPostion.x = BOSS_HP_CENTER_POS.x - m_currentShakeCenterPosition.x;
    gaugeAddPostion.y = BOSS_HP_CENTER_POS.y - m_currentShakeCenterPosition.y;

    m_currentShakeGaugePosition.x = BOSS_HP_FRONT_POS.x + gaugeAddPostion.x;
    m_currentShakeGaugePosition.y = BOSS_HP_FRONT_POS.y + gaugeAddPostion.y;


    if (m_shakeTimer > SHAKE_TIME_LIMIT)
    {
        m_isVariableHp = false;
        m_currentShakeCenterPosition = BOSS_HP_CENTER_POS;
        m_currentShakeGaugePosition = BOSS_HP_FRONT_POS;
        m_shakeTimer = 0.0f;
    }
    else
    {
        m_shakeTimer += g_gameTime->GetFrameDeltaTime();
    }

    m_spriteList[enSpriteName_HPBar_Back].SetPosition(m_currentShakeCenterPosition);
    m_spriteList[enSpriteName_HPBar_White].SetPosition(m_currentShakeGaugePosition);
    m_spriteList[enSpriteName_HPBar_Front].SetPosition(m_currentShakeGaugePosition);
    m_spriteList[enSpriteName_HPBar_Frame].SetPosition(m_currentShakeCenterPosition);

    m_spriteList[enSpriteName_HPBar_Back].Update();
    m_spriteList[enSpriteName_HPBar_White].Update();
    m_spriteList[enSpriteName_HPBar_Front].Update();
    m_spriteList[enSpriteName_HPBar_Frame].Update();

}


void SummonerUI::DeleteSelf()
{
}

void SummonerUI::Init()
{
    InitSpriteRenders();
    InitFontRenders();
}

void SummonerUI::InitSpriteRenders()
{
    m_currentShakeCenterPosition = BOSS_HP_CENTER_POS;

    m_currentShakeGaugePosition = BOSS_HP_FRONT_POS;

    //HPフレームの初期化
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Frame],
        "Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 50, BOSS_HP_CENTER_POS);
    //////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの表
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Front],
        "Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 37, BOSS_HP_FRONT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_Front].SetPivot(GAUGE_PIBOT);
    ///////////////////////////////////////////////////////////////////////////////////////////
    //HPが減った時に出てくる白いHPバー
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_White],
        "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 37, BOSS_HP_FRONT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_White].SetPivot(GAUGE_PIBOT);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの裏側
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Back],
        "Assets/sprite/InGame/Character/HP_Back_Boss.DDS", 980, 39, BOSS_HP_CENTER_POS);

    m_spriteList[enSpriteName_HPBar_Back].Update();
    m_spriteList[enSpriteName_HPBar_White].Update();
    m_spriteList[enSpriteName_HPBar_Front].Update();
    m_spriteList[enSpriteName_HPBar_Frame].Update();
}

void SummonerUI::InitFontRenders()
{
}



