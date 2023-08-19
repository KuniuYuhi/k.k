#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

Title::Title()
{
}

Title::~Title()
{
}

bool Title::Start()
{
    m_titleSprite.Init("Assets/sprite/title.DDS", 1920, 1080);
   /* m_titleSprite.SetRoundWipe(true);
    m_titleSprite.SetRoundWipeStartPosition(1920.0f / 2, 1080.0f / 2);
    m_titleSprite.SetWipeSize(m_wipeSize);
    m_titleSprite.Update();*/

    m_fade = FindGO<Fade>("fade");

    return true;
}

void Title::Update()
{
    /*m_titleSprite.SetWipeSize(m_wipeSize);
    m_wipeSize -= 5.0f;*/

    //フェードが終わったら消す
    if (m_fade->GetCurrentAlpha() >= 1.0f)
    {
        Game* game = NewGO<Game>(0, "game");
        DeleteGO(this);
    }


    if (g_pad[0]->IsTrigger(enButtonA))
    {
        
        //フェード開始
        m_fade->StartFadeIn(2.0f);

        
    }
}

void Title::Render(RenderContext& rc)
{
    m_titleSprite.Draw(rc);
}
