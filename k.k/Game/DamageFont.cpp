#include "stdafx.h"
#include "DamageFont.h"

namespace {
	const float SCALE_PLAYER = 1.2f;
	const float SCALE_MONSTER = 1.2f;
	const float SCALE_BOSS = 1.5f;
	const float Y_UP_PLAYER =50.0f;
	const float Y_UP_MONSTER = 50.0f;
	const float Y_UP_BOSS = 170.0f;

	const float BACK_SPRITE_SCALE_PLAYER = 1.2f;
	const float BACK_SPRITE_SCALE_MOBMONSTER = 1.2f;
	const float BACK_SPRITE_SCALE_BOSS = 1.5f;

	const float DEFAULT_Y_OFFSET = 18.0f;
	const float DEFAULT_X_OFFSET = -25.0f;

	const int RAMDOM_RANGE = 50.0f;
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

	//透明にしていく
	TransparencyFont();
	//移動処理
	Move();
}

void DamageFont::InitFont()
{
	//背景の画像の初期化
	m_damageBackRender.Init(
		"Assets/sprite/InGame/DamageEffect/Damage.DDS", 150, 150
	);

	//ダメージフォントのオフセットを計算
	Vector2 fontOffset = g_vec2Zero;


	//ダメージを受けるアクターによって設定
	switch (m_damageActor)
	{
	case DamageFont::enDamageActor_Player:
		//ダメージフォントのオフセットを計算
		fontOffset = CalcDamageFontOffset(SCALE_PLAYER);
		//ダメージフォントと背景の設定
		SetDamageSpriteAndFont(
			g_vec4Red, SCALE_PLAYER, fontOffset,
			Y_UP_PLAYER, BACK_SPRITE_SCALE_PLAYER
		);
		break;
	case DamageFont::enDamageActor_Monster:
		//ダメージフォントのオフセットを計算
		fontOffset = CalcDamageFontOffset(SCALE_MONSTER);
		//ダメージフォントと背景の設定
		SetDamageSpriteAndFont(
			g_vec4White, SCALE_MONSTER, fontOffset,
			Y_UP_MONSTER, BACK_SPRITE_SCALE_MOBMONSTER
		);
		break;
	case DamageFont::enDamageActor_Boss:
		//ダメージフォントのオフセットを計算
		fontOffset = CalcDamageFontOffset(SCALE_BOSS);
		//ダメージフォントと背景の設定
		SetDamageSpriteAndFont(
			g_vec4White, SCALE_BOSS, fontOffset,
			Y_UP_MONSTER, BACK_SPRITE_SCALE_BOSS
		);
		//座標をランダムにする
		m_largeRandomPositionFlag = true;
		break;
	default:
		std::abort();
		break;
	}

	//ダメージフォントを生成する座標を計算
	m_damagePosition = CalcDamagePosition();
	
	wchar_t damage[255];
	swprintf_s(damage, 255, L"%d", m_damage);
	//ダメージ量を設定
	m_damageFont.SetText(damage);
	//影をつける
	m_damageFont.SetShadowParam(true, 2.0f, g_vec4Black);
	//座標を設定
	m_damageFont.SetPosition(m_damagePosition);

	//ダメージフォントの背景の座標を設定
	m_damageBackPosition.x = m_damagePosition.x;
	m_damageBackPosition.y = m_damagePosition.y;
	m_damageBackRender.SetPosition(m_damageBackPosition);
	//更新
	m_damageBackRender.Update();
}

void DamageFont::Move()
{
	//上昇させる
	m_damagePosition.y += g_gameTime->GetFrameDeltaTime() * 60.0f;
	//背景のY座標をフォントと合わせる
	m_damageBackPosition.y = m_damagePosition.y;
	//フォントの座標を設定
	m_damageFont.SetPosition(m_damagePosition);
	//背景の座標を設定と更新
	m_damageBackRender.SetPosition(m_damageBackPosition);
	m_damageBackRender.Update();
}

void DamageFont::TransparencyFont()
{
	//色を透明にしていく
	m_color.w -= g_gameTime->GetFrameDeltaTime() * 2.2f;
	m_backColor.w = m_color.w;
	//フォントの色の設定
	m_damageFont.SetColor(m_color);
	//背景の色の設定と更新
	m_damageBackRender.SetMulColor(m_backColor);
	m_damageBackRender.Update();
}

void DamageFont::SetDamageSpriteAndFont(
	Vector4 fontColor, 
	float fontScale, Vector2 fontOffset, float yUp, 
	float backSpriteScale)
{
	//文字の色を設定
	m_color = fontColor;
	SetDamageFontInfo(m_color, fontScale, fontOffset);
	//Y座標を少し上げる
	m_yUp = yUp;
	//画像を拡大
	m_damageBackRender.SetScale(g_vec3One * backSpriteScale);
}

void DamageFont::SetDamageFontInfo(Vector4 color, float scale, Vector2 offset)
{
	m_damageFont.SetColor(color);
	m_damageFont.SetScale(scale);
	m_damageFont.SetOffset(offset);
}

Vector2 DamageFont::CalcDamagePosition()
{
	//ダメージを受けるキャラクターの座標のY座標を少し上げる
	Vector3 DamagePosition = m_position;
	DamagePosition.y += m_yUp;

	Vector2 finalPos = g_vec2Zero;

	//座標をワールド座標からスクリーン座標に変換
	g_camera3D->CalcScreenPositionFromWorldPosition(finalPos, DamagePosition);
	//広い範囲で座標をランダムな場所にしたいなら
	if (m_largeRandomPositionFlag == true)
	{
		finalPos.x += (rand() % (RAMDOM_RANGE*2 - (-RAMDOM_RANGE*2) + 1)) + (-RAMDOM_RANGE*2);
		finalPos.y += (rand() % (RAMDOM_RANGE*2 - (-RAMDOM_RANGE*2) + 1)) + (-RAMDOM_RANGE*2);
	}
	else
	{
		finalPos.x += (rand() % (RAMDOM_RANGE - (-RAMDOM_RANGE) + 1)) + (-RAMDOM_RANGE);
		finalPos.y += (rand() % (RAMDOM_RANGE - (-RAMDOM_RANGE) + 1)) + (-RAMDOM_RANGE);
	}

	return finalPos;
}

Vector2 DamageFont::CalcDamageFontOffset(float fontScale)
{
	//ダメージの桁数を取得する
	int digitCount = static_cast<int>(log10(m_damage) + 1);
	Vector2 finalOffset = g_vec2Zero;
	//Yオフセット
	finalOffset.y = DEFAULT_X_OFFSET * fontScale;
	//Xオフセット
	finalOffset.x = digitCount * DEFAULT_Y_OFFSET * fontScale;

	return finalOffset;
}

void DamageFont::Render(RenderContext& rc)
{
	m_damageBackRender.Draw(rc);

	m_damageFont.Draw(rc);
}
