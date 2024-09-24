#pragma once
#include "GameUIBase.h"

class MobEnemyBase;

class MobEnemyUI : public GameUIBase
{
private:

	enum EnSpriteName
	{
		enSpriteName_HPBar_Front,		//HP�o�[�\
		enSpriteName_HPBar_Back,		//HP�o�[��
		enSpriteName_HPBar_White,		//HP�����������ɏo�Ă��锒��HP�o�[
		enSpriteName_HPBar_Frame,		//HP�o�[�̃t���[��



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
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�v���C�g��������
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// �X�e�[�^�X��UI���X�V
	/// </summary>
	void UpdateStatusUI();

	/// <summary>
	/// UI���W���X�V
	/// </summary>
	void UpdateUIPosition();

	/// <summary>
	/// HP�Ɋւ���X�V����
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

