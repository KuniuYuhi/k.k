#include "stdafx.h"
#include "GameStatusUI.h"

#include "GameSceneManager.h"

namespace {
    const Vector2 TIMER_POS = { -910.0f,530.0f };
    const Vector2 TIMER_FLAME_POS = { -790.0f,479.0f };
}

bool GameStatusUI::Start()
{
    Init();

    return true;
}

void GameStatusUI::UIUpdate()
{
    UpdateGameTimer();
}

void GameStatusUI::Draw(RenderContext& rc)
{
    m_spriteList[enSpriteName_TimerFlame].Draw(rc);

    m_fontList[enFontName_timer].Draw(rc);
}

void GameStatusUI::UpdateGameTimer()
{
    //ゲームシーンマネージャーから時間を取得
    int miunte = (int)GameSceneManager::GetInstance()->GetGameTimerMinute();
    int second = (int)GameSceneManager::GetInstance()->GetGameTimerSecond();


    if (miunte <= 1.0f)
    {
        m_fontList[enFontName_timer].SetColor(g_vec4Red);
    }

    wchar_t time[256];
    swprintf_s(time, 256, L"%d:%02d", miunte, second);

    m_fontList[enFontName_timer].SetText(time);
}

void GameStatusUI::DeleteSelf()
{
    DeleteGO(this);
}

void GameStatusUI::Init()
{
    InitSpriteRenders();
    InitFontRenders();
}

void GameStatusUI::InitSpriteRenders()
{
    //サブ武器2のフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_TimerFlame],
        "Assets/sprite/InGame/Character/TimeFlame2.DDS",
        380, 100, TIMER_FLAME_POS
    );

}

void GameStatusUI::InitFontRenders()
{
    InitFontRender(
        m_fontList[enFontName_timer],
        TIMER_POS,
        1.5f,
        g_vec4White
    );
}


