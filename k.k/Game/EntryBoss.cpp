#include "stdafx.h"
#include "EntryBoss.h"
#include "Game.h"

namespace {
	const float SCALE_UP = 4.0f;


	const Vector3 L_START_POS = { -707.0f,430.0f,0.0f };
	const Vector3 L_END_POS = { -407.0f,-268.0f,0.0f };

	const Vector3 I_START_POS = { -488.0f,430.0f,0.0f };
	const Vector3 I_END_POS = { -188.0f,-268.0f,0.0f };

	const Vector3 C_START_POS = { 361.0f,430.0f,0.0f };
	const Vector3 C_END_POS = { 61.0f,-268.0f,0.0f };

	const Vector3 H_START_POS = { 676.0f,430.0f,0.0f };
	const Vector3 H_END_POS = { 376.0f,-268.0f,0.0f };

	const Vector3 START_SCALE = { 4.0f,4.0f,4.0f };
	const Vector3 END_SCALE = g_vec3One;


	const Vector3 BOSS_TEXT_POS = { 0.0f,-98.0f,0.0f };
	const Vector3 FIRE_POS = { 0.0f,-492.0f,0.0f };

	const Vector3 PRESS_A_POS = { 723.0f,-501.0f ,0.0f };
	const Vector3 PRESS_A_SCALE = { 0.6f,0.6f ,0.6f };
}

EntryBoss::EntryBoss()
{
}

EntryBoss::~EntryBoss()
{
}

bool EntryBoss::Start()
{

	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/Victory.tka");
	m_animationClip[enAnimClip_Victory].SetLoopFlag(true);

	m_model.Init(
		"Assets/modelData/character/Lich/Lich.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	m_position.y -= 380.0f;
	m_scale *= SCALE_UP;

	m_rotation.SetRotationDegY(180.0f);
	m_model.SetTransform(m_position, m_rotation, m_scale);
	m_model.Update();

	Vector3 pointLightPos = m_position;
	pointLightPos.y = 200.0f;
	//ポイントライト
	/*g_renderingEngine->SetPointLight(
		pointLightPos,
		Vector3(15.0f, 0.0f, 12.0f),
		Vector3(700.0f, 3.0f, 0.0f)
	);*/

	/*g_renderingEngine->SetSpotLight(
		pointLightPos,
		Vector3(100.0f,0.0f,100.0f),
		Vector3(500.0f, 10.0f, 0.0f),
		Vector3(1.0f,-1.0f,0.0f),
		Vector3(90.0f, 40.0f, 0.0f)
	);*/


	InitSprite();
	InitLerpPosition();

	return true;
}

void EntryBoss::Update()
{
	if (m_completeFlag == true)
	{
		CompleteTime();
	}
	else
	{
		positionUp();

		//0になるちょと前から画像動かす
		if (m_gOTextFlag == true)
		{
			SpriteMove();
		}

		//「L」「I」「C」「H」が表示されたら
		if (m_FireDrawFlag == true)
		{
			FireMove();
		}
	}
	
	m_model.PlayAnimation(enAnimClip_Idle);
	m_model.Update();
}

void EntryBoss::positionUp()
{
	

	if (m_position.y >= 0.0f)
	{
		m_position.y = 0.0f;
		
	}
	else
	{
		//画像の表示を始める
		if (m_position.y > -20.0f)
		{
			m_gOTextFlag = true;
		}

		m_position.y += g_gameTime->GetFrameDeltaTime() * 45.0f;
	}

	m_model.SetPosition(m_position);

}

void EntryBoss::InitSprite()
{
	
	m_lichCharInfo[L].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/L.DDS", 243, 290);
	SettingSpriteRender(m_lichCharInfo[L].m_CharRender, L_START_POS, START_SCALE, g_quatIdentity);
	

	m_lichCharInfo[I].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/I.DDS", 167, 290);
	SettingSpriteRender(m_lichCharInfo[I].m_CharRender, I_START_POS, START_SCALE, g_quatIdentity);
	

	m_lichCharInfo[C].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/C.DDS", 280, 290);
	SettingSpriteRender(m_lichCharInfo[C].m_CharRender, C_START_POS, START_SCALE, g_quatIdentity);


	m_lichCharInfo[H].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/H.DDS", 307, 290);
	SettingSpriteRender(m_lichCharInfo[H].m_CharRender, H_START_POS, START_SCALE, g_quatIdentity);
	




	m_bossTextRender.Init("Assets/sprite/InGame/BossAppearance/Boss.DDS", 284, 84);
	SettingSpriteRender(m_bossTextRender, BOSS_TEXT_POS, g_vec3One, g_quatIdentity);

	m_FireRender.Init("Assets/sprite/InGame/BossAppearance/Back.DDS", 1650, 800);
	SettingSpriteRender(m_FireRender, FIRE_POS, g_vec3Zero, g_quatIdentity);

	m_PressAButton.Init("Assets/sprite/InGame/BossAppearance/SkipText.DDS", 720, 76);
	SettingSpriteRender(m_PressAButton, PRESS_A_POS, PRESS_A_SCALE, g_quatIdentity);
	
}

void EntryBoss::InitLerpPosition()
{
	//「L」の始点と終点
	m_lichCharInfo[L].m_startPosition = L_START_POS;
	m_lichCharInfo[L].m_endPosition = L_END_POS;

	//「I」の始点と終点
	m_lichCharInfo[I].m_startPosition = I_START_POS;
	m_lichCharInfo[I].m_endPosition = I_END_POS;

	//「C」の始点と終点
	m_lichCharInfo[C].m_startPosition = C_START_POS;
	m_lichCharInfo[C].m_endPosition = C_END_POS;

	//「I」の始点と終点
	m_lichCharInfo[H].m_startPosition = H_START_POS;
	m_lichCharInfo[H].m_endPosition = H_END_POS;

}


void EntryBoss::SpriteMove()
{
	if (m_lichTextCount == END)
	{
		m_FireDrawFlag = true;
		return;
	}

	m_time += g_gameTime->GetFrameDeltaTime() * 3.0f;

	//文字の座標の線形補間
	Vector3 LerpPos;
	LerpPos.Lerp(m_time, m_lichCharInfo[m_lichTextCount].m_startPosition, m_lichCharInfo[m_lichTextCount].m_endPosition);
	//文字のサイズの線形補間
	Vector3 size;
	size.Lerp(m_time, START_SCALE, END_SCALE);
	//回転
	Quaternion rot = g_quatIdentity;

	//更新
	m_lichCharInfo[m_lichTextCount].m_CharRender.SetTransForm(LerpPos, size, rot);
	m_lichCharInfo[m_lichTextCount].m_CharRender.Update();


	//一秒超えたら次の文字に切り替える
	if (m_time >= 1.0f)
	{
		m_lichTextCount++;
		m_time = 0.0f;
	}

}

void EntryBoss::FireMove()
{
	if (m_time > 1.0f)
	{
		//やること終わり
		//しばらくそのままにする
		m_completeFlag = true;

		
		return;
	}

	m_time += g_gameTime->GetFrameDeltaTime() * m_mulValue;

	Vector3 scale;
	scale.Lerp(m_time, g_vec3Zero, g_vec3One);

	m_FireRender.SetScale(scale);
	m_FireRender.Update();

	m_mulValue *= 1.7f;
}

void EntryBoss::CompleteTime()
{
	if (m_endTime < m_endTimer)
	{
		//登場ムービーが終わったことを伝える
		m_game->SetBossMovieFlag(true);
	}
	else
	{
		m_endTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void EntryBoss::Render(RenderContext& rc)
{
	m_model.Draw(rc);

	if (m_FireDrawFlag == true)
	{
		m_bossTextRender.Draw(rc);
		m_FireRender.Draw(rc);
	}
	else
	{
		m_PressAButton.Draw(rc);
	}
	

	if (m_gOTextFlag != true)
	{
		return;
	}

	if (m_lichTextCount >= L)
	{
		m_lichCharInfo[L].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= I)
	{
		m_lichCharInfo[I].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= C)
	{
		m_lichCharInfo[C].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= H)
	{
		m_lichCharInfo[H].m_CharRender.Draw(rc);
	}

	
	
	
	

	//カウントされた数画像を表示する
	/*for (int i = 0; i < m_lichTextCount; i++)
	{
		m_lichCharInfo[i].m_CharRender.Draw(rc);
	}*/
}
