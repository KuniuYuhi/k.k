#include "stdafx.h"
#include "DamageFont.h"

namespace {
	const float SCALE_PLAYER = 1.2f;
	const float SCALE_MONSTER = 1.2f;
	const float SCALE_BOSS = 1.5f;
	const float Y_UP_PLAYER =50.0f;
	const float Y_UP_MONSTER = 50.0f;
	const float Y_UP_BOSS = 170.0f;

	const Vector3 BACK_SCALE_PLAYER = { 1.2f,1.2f,0.0f };
	const Vector3 BACK_SCALE_MONSTER = { 1.2f,1.2f,0.0f };
	const Vector3 BACK_SCALE_BOSS = { 1.5f,1.5f,0.0f };
}

DamageFont::DamageFont()
{
}

DamageFont::~DamageFont()
{
}

bool DamageFont::Start()
{
	
	InitFont();


	//todo 与えるダメージの文字サイズによってオフセット
		

	return true;
}

void DamageFont::Update()
{
	//透明になったら消去する
	if (m_color.w <= 0.0f)
	{
		DeleteGO(this);
	}


	TransparencyFont();

	Move();



}

void DamageFont::InitFont()
{
	m_damageBackRender.Init("Assets/sprite/InGame/DamageEffect/Damage.DDS", 150, 150);
	

	switch (m_damageActor)
	{
	case DamageFont::enDamageActor_Player:
		//文字の色を赤にする
		m_color = g_vec4Red;
		m_damageFont.SetColor(m_color);
		m_damageFont.SetScale(SCALE_PLAYER);
		m_damageFont.SetOffset(24.0f, -12.0f);
		m_yUp = Y_UP_PLAYER;
		//画像を拡大
		m_damageBackRender.SetScale(BACK_SCALE_PLAYER);
		break;
	case DamageFont::enDamageActor_Monster:
		//文字の色を白にする
		m_color = g_vec4White;
		m_damageFont.SetColor(m_color);
		m_damageFont.SetScale(SCALE_MONSTER);
		m_damageFont.SetOffset(24.0f, -12.0f);
		m_yUp = Y_UP_MONSTER;
		//画像を拡大
		m_damageBackRender.SetScale(BACK_SCALE_MONSTER);
		break;
	case DamageFont::enDamageActor_Boss:
		//文字の色を白にする
		m_color = g_vec4White;
		m_damageFont.SetColor(m_color);
		m_damageFont.SetScale(SCALE_BOSS);
		m_damageFont.SetOffset(24.0f, -13.0f);
		m_yUp = Y_UP_BOSS;
		//画像を拡大
		m_damageBackRender.SetScale(BACK_SCALE_BOSS);
		//座標をランダムにする
		m_positionRandomFlag = true;
		break;
	default:
		std::abort();
		break;
	}

	Vector3 DamagePosition = m_position;
	DamagePosition.y += m_yUp;
	//座標を変換する
	g_camera3D->CalcScreenPositionFromWorldPosition(m_damagePosition, DamagePosition);

	if (m_positionRandomFlag == true)
	{
		//ランダムな座標にする
		m_damagePosition.x += (rand() % (100 - (-100) + 1)) + (-100);
		m_damagePosition.y += (rand() % (100 - (-100) + 1)) + (-100);
	}
	else
	{
		m_damagePosition.x += (rand() % (50 - (-50) + 1)) + (-50);
		m_damagePosition.y += (rand() % (50 - (-50) + 1)) + (-50);
	}
	

	wchar_t damage[255];
	swprintf_s(damage, 255, L"%d", m_damage);
	m_damageFont.SetText(damage);
	m_damageFont.SetShadowParam(true, 2.0f, g_vec4Black);

	m_damageFont.SetPosition(m_damagePosition);

	//画像
	m_damageBackPosition.x = m_damagePosition.x;
	m_damageBackPosition.y = m_damagePosition.y;

	m_damageBackRender.SetPosition(m_damageBackPosition);
	m_damageBackRender.Update();
}

void DamageFont::Move()
{
	m_damagePosition.y += g_gameTime->GetFrameDeltaTime() * 60.0f;

	m_damageBackPosition.y = m_damagePosition.y;

	m_damageFont.SetPosition(m_damagePosition);

	m_damageBackRender.SetPosition(m_damageBackPosition);
	m_damageBackRender.Update();
}

void DamageFont::TransparencyFont()
{
	m_color.w -= g_gameTime->GetFrameDeltaTime() * 2.2f;
	m_backColor.w = m_color.w;

	m_damageFont.SetColor(m_color);

	m_damageBackRender.SetMulColor(m_backColor);
	m_damageBackRender.Update();
}

void DamageFont::Render(RenderContext& rc)
{
	m_damageBackRender.Draw(rc);

	m_damageFont.Draw(rc);
}
