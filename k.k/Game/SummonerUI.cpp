#include "stdafx.h"
#include "SummonerUI.h"

#include "Summoner.h"

namespace {
    //////////////////////////////////////////////////////////////////
    //�Q�[�W
    ///////////////////////////////////////////////////////////////////
    const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HP��MP�̃s�{�b�g

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
    //HP�o�[�̏���
    UpdateSummonerHP();

    //Hp���ϓ�������o�[��h�炷
    if (m_isVariableHp)
    {
        ShakeHPBar();
    }
}

void SummonerUI::UpdateSummonerHP()
{
    //�ő�HP�ƌ��݂�HP���擾
    int maxHP = m_summmoner->GetCommonStatus().GetMaxHp();
    int currentHP = m_summmoner->GetCommonStatus().GetCurrentHp();

    UpdateMainHPBar(currentHP, maxHP);

    UpdateWhiteHPBar(currentHP, maxHP);

    m_oldHpValue = currentHP;
}

void SummonerUI::UpdateMainHPBar(int currentHp, int maxHp)
{
    //HP���ϓ�������
    if (m_oldHpValue != currentHp)
    {
        m_shakeTimer = 0.0f;
        m_isVariableHp = true;
        
    }
    else
    {
        //HP���ς���Ă��Ȃ��Ȃ珈������K�v�Ȃ�
        return;
    }

    //HP�o�[�̌����Ă��������B
    Vector3 HpScale = CalcGaugeRatio(maxHp, currentHp);
    //�g�嗦��ݒ�
    m_spriteList[enSpriteName_HPBar_Front].SetScale(HpScale);
    //�X�V
    m_spriteList[enSpriteName_HPBar_Front].Update();
}

void SummonerUI::UpdateWhiteHPBar(int currentHp, int maxHp)
{
    //����HP�����݂�HP��菬�����Ȃ�//����HP�Ɍ��݂�HP��������
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

    //HP�t���[���̏�����
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Frame],
        "Assets/sprite/InGame/Character/HP_Flame_Boss.DDS", 1000, 50, BOSS_HP_CENTER_POS);
    //////////////////////////////////////////////////////////////////////////////////////////
    //HP�o�[�̕\
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Front],
        "Assets/sprite/InGame/Character/HP_Front_Boss.DDS", 978, 37, BOSS_HP_FRONT_POS);
    //�s�{�b�g�̐ݒ�
    m_spriteList[enSpriteName_HPBar_Front].SetPivot(GAUGE_PIBOT);
    ///////////////////////////////////////////////////////////////////////////////////////////
    //HP�����������ɏo�Ă��锒��HP�o�[
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_White],
        "Assets/sprite/InGame/Character/HP_White_Boss.DDS", 978, 37, BOSS_HP_FRONT_POS);
    //�s�{�b�g�̐ݒ�
    m_spriteList[enSpriteName_HPBar_White].SetPivot(GAUGE_PIBOT);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //HP�o�[�̗���
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



