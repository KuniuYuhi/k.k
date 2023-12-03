#pragma once
#include "MyWeapon.h"

class Game;
class Player;
class Lich;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	struct WeaponSprits
	{
		SpriteRender* m_weaponSprite=nullptr;
	};

	/// <summary>
	/// �v���C���[�C���X�^���X�̎擾
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// ���b�`(�{�X)�̃C���X�^���X�̎擾
	/// </summary>
	/// <param name="lich"></param>
	void GetLich(Lich* lich)
	{
		m_lich = lich;
	}
	/// <summary>
	/// �Q�[���C���X�^���X�̎擾
	/// </summary>
	/// <param name="game"></param>
	void GetGame(Game* game)
	{
		m_game = game;
	}

	
private:
	/// <summary>
	/// �v���C���[��UI�̏�����
	/// </summary>
	void InitPlayerUI();
	/// <summary>
	/// �����X�^�[��UI�̏�����
	/// </summary>
	void InitMonsterUI();

	/// <summary>
	/// �v���C���[UI�̏���
	/// </summary>
	void PlayerUIUpdate();
	/// <summary>
	/// ���C���L�����̃X�e�[�^�X�̍X�V����
	/// </summary>
	void UpdateMainStatus();
	/// <summary>
	/// �L�����A�C�R���̍X�V����
	/// </summary>
	void UpdateCharaIcon();
	/// <summary>
	/// ����̍X�V����
	/// </summary>
	void UpdateWeapon();
	/// <summary>
	/// HP��MP�Ȃǂ̃Q�[�W�̃X�P�[�����v�Z����
	/// </summary>
	/// <param name="Maxvalue">�ő�l</param>
	/// <param name="value">�v�Z�������l</param>
	/// <returns>�T�C�Y</returns>
	Vector3 CalcGaugeScale(float Maxvalue, float value);

	/// <summary>
	/// �����X�^�[��UI�̏���
	/// </summary>
	void MonsterUIUpdate();

	/// <summary>
	/// �^�C�}�[��UI�̏���
	/// </summary>
	void TimerUIUpdate();

	void DrawPlayerUI(RenderContext& rc);

	void DrawMonsterUI(RenderContext& rc);

	/// <summary>
	/// �v���C���[��HP�̏���
	/// </summary>
	void ProcessPlayerHp();
	/// <summary>
	/// �v���C���[��MP�̏���
	/// </summary>
	void ProcessPlayerMp();

	/// <summary>
	/// �{�X��HP�̏���
	/// </summary>
	void ProcessBossHP();



	/// <summary>
	/// ����؂�ւ�����
	/// </summary>
	void ChangeWeapon(
		WeaponSprits& changeWeaponSprite
	);


	/// <summary>
	/// �X�v���C�g�����_�[�̏������B
	/// </summary>
	/// <param name="spriterender">�������������X�v���C�g�����_�[</param>
	/// <param name="filePath">�摜�̃t�@�C���p�X</param>
	/// <param name="width">�摜�̕�</param>
	/// <param name="height">�摜�̍���</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void InitSpriteRender(
		SpriteRender& spriterender,
		const char* filePath,
		const float width, const float height,
		Vector3 position,
		Vector3 scale = g_vec3One,
		Quaternion rotation = g_quatIdentity
	);
	/// <summary>
	/// �t�H���g�����_�[�̏������B
	/// </summary>
	/// <param name="fontRender">�������������t�H���g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="color">�J���[</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="isShadowParam">�e(�֊s)�����邩</param>
	/// <param name="shadowOffset">�I�t�Z�b�g��</param>
	/// <param name="shadowColor">�e�̃J���[</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);

private:

	struct MonsterUI
	{
		FontRender m_hpFont;					//HP�̒l
		FontRender m_AccumulationDamageFont;	//

		SpriteRender m_IconRender;				//�L�����A�C�R��
		SpriteRender m_HpFlameRender;			//HP�̃t���[��
		SpriteRender m_HpFrontRender;			//�ϓ�����HP�o�[
		SpriteRender m_HpWhiteRender;			//�x��Č��炷����HP�o�[
		SpriteRender m_HpBackRender;			//HP�o�[�̗���
	};

	//�v���C���[��UI
	struct PlayerUI
	{
		FontRender m_hpFont;						//HP�̒l
		FontRender m_mpFont;						//MP�̒l

		SpriteRender m_characterIconRender;			//�L�����A�C�R��

		SpriteRender m_iconBaseRender;				//�L�����A�C�R���̃x�[�X

		SpriteRender m_statusBarRender;			//�X�e�[�^�X�o�[

		SpriteRender m_hpFrontRender;			//�ϓ�����HP�o�[
		SpriteRender m_hpBackRender;			//HP�o�[�̗���
		SpriteRender m_hpWhiteRender;			//�x��Č��炷����HP�o�[
		SpriteRender m_mpFrontRender;			//�ϓ�����MP�o�[
		SpriteRender m_mpBackRender;			//MP�o�[�̗���

		SpriteRender m_Skill_1FlameRender;			//�X�L��1�̃t���[��

		SpriteRender m_mainWeaponFlameRender;		//���C������̃t���[��

		SpriteRender m_subWeaponFlameRender;		//�T�u����P�̃t���[��
		SpriteRender m_subWeaponCommandRender;		//�T�u����P�̃R�}���h

		SpriteRender m_sub2WeaponFlameRender;		//�T�u����Q�̃t���[��
		SpriteRender m_sub2WeaponCommandRender;		//�T�u����Q�̃R�}���h

		SpriteRender m_weaponRender[enWeapon_num];
	};

	
	WeaponSprits m_weaponSprits[enWeapon_num];



	Vector3 m_weaponIconPos[enWeapon_num] = {
		{720.0f,-330.0f,0.0f},
		{850.0f,38.0f,0.0f},
		{850.0f,-110.0f,0.0f}
	};
	Vector3 m_weaponIconScale[enWeapon_num] = {
		g_vec3One * 0.8f,
		g_vec3One * 0.5f,
		g_vec3One * 0.5f
	};


	FontRender m_TimerFont;				//��������
	SpriteRender m_TimeFlameRender;		//�������Ԃ̘g

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	PlayerUI m_playerUI;			//�v���C���[�̏���UI
	MonsterUI m_monsterUI;			//�����X�^�[�̏���UI

	Vector3 m_playerHpWhiteScale = g_vec3One;		//�v���C���[�̔���HP�o�[
	Vector3 m_oldPlayerHpScale = g_vec3One;	//�O�t���[���̃v���C���[��HP�o�[

	Vector3 m_BossHpWhiteScale = g_vec3One;			//�{�X�̔���HP�o�[
	Vector3 m_oldBossHpScale = g_vec3One;		//�O�t���[���̃{�X��HP�o�[

	Vector3 m_playerMpWhiteScale = g_vec3One;		//�v���C���[�̔���MP�o�[

	float m_oldMainCharaHP = 0.0f;				//�O�t���[���̃��C���L������HP

	float m_gaugeTimer = 0.0f;

	float m_charaIconChangeTimer = 0.0f;
	
	bool m_coolTimeDrawFlag = true;

	float m_wipeSize = 0;

	float m_bossLerpSpeed = 0.0f;
	float m_playerLerpSpeed = 0.0f;







};

