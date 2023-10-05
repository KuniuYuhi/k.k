#include "stdafx.h"
#include "BattleStart.h"

namespace {
    const Vector3 BATTLE_TEXT_START_POS = { 0.0f,336.0f,0.0f };
    const Vector3 BATTLE_TEXT_END_POS = { 1650.0f,336.0f,0.0f };
 
    const Vector3 START_TEXT_START_POS = { 0.0f,-336.0f,0.0f };
    const Vector3 START_TEXT_END_POS = { -1650.0f,-336.0f,0.0f };

}

BattleStart::BattleStart()
{
}

BattleStart::~BattleStart()
{
}

bool BattleStart::Start()
{
    InitSprite();

    return true;
}

void BattleStart::Update()
{
    //見えるようになったらもうやらない
    if (m_calcAlphaEndFlag != true)
    {
        CalcAlpha();
    }
    else
    {
        //BATTLEの移動
        MoveText(m_battleTextRender, BATTLE_TEXT_START_POS, BATTLE_TEXT_END_POS);
        //STARTの移動
        MoveText(m_startTextRender, START_TEXT_START_POS, START_TEXT_END_POS);

        m_timer += g_gameTime->GetFrameDeltaTime() * 0.4f;
    }
   


   
}

void BattleStart::Render(RenderContext& rc)
{
    m_battleTextRender.Draw(rc);
    m_startTextRender.Draw(rc);
}

void BattleStart::MoveText(SpriteRender& spriteRender, Vector3 startPos, Vector3 endPos)
{
    Vector3 pos;
    pos.Lerp(m_timer, startPos, endPos);
    spriteRender.SetPosition(pos);
    spriteRender.Update();
}

void BattleStart::CalcAlpha()
{
    m_alpha += g_gameTime->GetFrameDeltaTime() * 0.7f;

    if (m_alpha > 1.0f)
    {
        //不透明になった
        //バトルスタートの音再生
        g_soundManager->InitAndPlaySoundSource(enSoundName_BattleStart, g_soundManager->GetBGMVolume());
        m_calcAlphaEndFlag = true;
        m_alpha = 1.0f;
    }

    m_color.w = m_alpha;

    m_battleTextRender.SetMulColor(m_color);
    m_startTextRender.SetMulColor(m_color);

    m_battleTextRender.Update();
    m_startTextRender.Update();
}

void BattleStart::InitSprite()
{
    m_battleTextRender.Init("Assets/sprite/InGame/BattleStart/Battle.DDS", 1416, 265);
    m_battleTextRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
    SettingSpriteRender(m_battleTextRender, BATTLE_TEXT_START_POS, g_vec3One, g_quatIdentity);

    m_startTextRender.Init("Assets/sprite/InGame/BattleStart/Start.DDS", 1236, 265);
    SettingSpriteRender(m_startTextRender, START_TEXT_START_POS, g_vec3One, g_quatIdentity);

}
