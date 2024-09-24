#pragma once
#include "GameUIBase.h"

class MobEnemyBase;

class MobEnemyUI : public GameUIBase
{
private:

	enum EnSpriteName
	{
		enSpriteName_HPBar_Front,		//HPバー表
		enSpriteName_HPBar_Back,		//HPバー裏
		enSpriteName_HPBar_White,		//HPが減った時に出てくる白いHPバー
		enSpriteName_HPBar_Frame,		//HPバーのフレーム



		enSprite_Num
	};


public:

	MobEnemyUI() {}
	~MobEnemyUI() {}

	bool Start() override;

	void UIUpdate() override;

	//void Render(RenderContext& rc) override;

	void Draw(RenderContext& rc) override;

	void DeleteSelf() override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="mobEnemy"></param>
	void SettingTargetEnemy(MobEnemyBase* mobEnemy);

private:

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// スプライトを初期化
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// ステータスのUIを更新
	/// </summary>
	void UpdateStatusUI();

	/// <summary>
	/// UI座標を更新
	/// </summary>
	void UpdateUIPosition();

	/// <summary>
	/// HPに関する更新処理
	/// </summary>
	void UpdateEnemyHP();


	void UpdateMainHPBar(int currentHp, int maxHp);

	void UpdateWhiteHPBar(int currentHp, int maxHp);

private:

	MobEnemyBase* m_mobEnemy = nullptr;

	SpriteRender m_spriteList[enSprite_Num];


	Vector2 m_currentShakeCenterPosition = g_vec2Zero;
	Vector2 m_currentShakeGaugePosition = g_vec2Zero;


	int m_whiteHP = 0;

};

