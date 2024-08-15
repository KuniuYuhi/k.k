#pragma once
#include "MyWeapon.h"

class Game;
class Player;
class Boss;

class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();

	struct WeaponSprits
	{
		SpriteRender* m_weaponRender = nullptr;
		SpriteRender* m_weaponEndranceRender = nullptr;
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �v���C���[�C���X�^���X�̎擾
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player)
	{
		m_player = player;
	}

	/// <summary>
	/// �{�X�C���X�^���X�̎擾
	/// </summary>
	/// <param name="boss"></param>
	void SetBoss(Boss* boss)
	{
		m_boss = boss;
	}

	/// <summary>
	/// �Q�[���C���X�^���X�̎擾
	/// </summary>
	/// <param name="game"></param>
	void SetGame(Game* game)
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
	/// �t�F�[�Y��UI�̏�����
	/// </summary>
	void InitPhaseUI();

	/// <summary>
	/// �v���C���[UI�̏���
	/// </summary>
	void PlayerUIUpdate();
	/// <summary>
	/// ���C���L�����̃X�e�[�^�X�̍X�V����
	/// </summary>
	void UpdateMainStatus();
	
	/// <summary>
	/// ����̍X�V����
	/// </summary>
	void UpdateWeapon();
	/// <summary>
	/// �Q�[�W�̃X�P�[�����v�Z����
	/// </summary>
	/// <param name="Maxvalue">�ő�l</param>
	/// <param name="value">�v�Z�������l</param>
	/// <returns>�T�C�Y�BX�̒l���ϊ�������Ԃ��Ԃ��Ă���</returns>
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
	/// �{�X��HP�̏���
	/// </summary>
	void ProcessBossHP();

	/// <summary>
	/// �{�X�̃X�[�p�[�A�[�}�[�̏���
	/// </summary>
	void ProcessBossSuperArmor();

	/// <summary>
	/// �t�F�[�Y�̏���
	/// </summary>
	void ProcessPhase();
	/// <summary>
	/// ���݂̃t�F�[�Y�̃����X�^�[�̐��̃e�L�X�g��ݒ�
	/// </summary>
	void CalcOffsetForNowPhaseMonsters(int monsters);

	/// <summary>
	/// ����؂�ւ�����
	/// </summary>
	void ChangeWeaponAction(WeaponSprits& changeWeaponSprite);

	/// <summary>
	/// �ϋv�l�̃t�H���g�̏���
	/// </summary>
	void ProcessWeaponEndranceFont();

	/// <summary>
	/// �����̌����ɂ���ăI�t�Z�b�g���v�Z����
	/// </summary>
	/// <param name="num">�I�t�Z�b�g��������</param>
	/// <param name="xOffset">X�����ɂ��炷��</param>
	/// <param name="yOffset">Y�����ɂ��炷��</param>
	Vector2 CalcNumberCount(float num, float xOffset, float yOffset);

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

		SpriteRender m_hpFlameRender;			//HP�̃t���[��
		SpriteRender m_hpFrontRender;			//�ϓ�����HP�o�[
		SpriteRender m_hpWhiteRender;			//�x��Č��炷����HP�o�[
		SpriteRender m_hpBackRender;			//HP�o�[�̗���

		SpriteRender m_superArmor_FlameRender;		//�X�[�p�[�A�[�}�[�̃t���[��
		SpriteRender m_superArmor_FrontBarRender;	//�X�[�p�[�A�[�}�[�̕\�̕ϓ�����̃o�[
		SpriteRender m_superArmor_BackBarRender;	//�X�[�p�[�A�[�}�[�̌��̃o�[

	};

	//�v���C���[��UI
	struct PlayerUI
	{
		FontRender m_hpFont;						//HP�̒l
		
		SpriteRender m_hpFlameRender;			//�X�e�[�^�X�o�[

		SpriteRender m_hpFrontRender;			//�ϓ�����HP�o�[
		SpriteRender m_hpBackRender;			//HP�o�[�̗���
		SpriteRender m_hpWhiteRender;			//�x��Č��炷����HP�o�[

		SpriteRender m_mainWeaponFlameRender;		//���C������̃t���[��

		SpriteRender m_subWeaponFlameRender;		//�T�u����P�̃t���[��
		SpriteRender m_subWeaponCommandRender;		//�T�u����P�̃R�}���h

		SpriteRender m_sub2WeaponFlameRender;		//�T�u����Q�̃t���[��
		SpriteRender m_sub2WeaponCommandRender;		//�T�u����Q�̃R�}���h

		SpriteRender m_weaponRender[enMaxWeapons_num];
		SpriteRender m_weaponEndranceRender[enMaxWeapons_num];
		FontRender m_weaponEndranceFont;

	};

	WeaponSprits m_weaponSprits[enMaxWeapons_num];

	Vector3 m_weaponIconPos[enMaxWeapons_num] = {
		{720.0f,-330.0f,0.0f},
		{850.0f,38.0f,0.0f},
		{850.0f,-110.0f,0.0f}
	};
	Vector3 m_weaponIconScale[enMaxWeapons_num] = {
		g_vec3One * 0.8f,
		g_vec3One * 0.5f,
		g_vec3One * 0.5f
	};


	FontRender m_timerFont;				//��������
	SpriteRender m_timeFlameRender;		//�������Ԃ̘g

	

	struct phaseUI
	{
		SpriteRender m_phaseFlameRender;	//�t�F�[�Y�̃t���[��
		FontRender m_nowPhaseFont;			//���݂̃t�F�[�Y��
		FontRender m_nowPhaseMonstersFont;	//���݂̃t�F�[�Y�̃����X�^�[�̐�
		SpriteRender m_phaseTimeFlameRender;
		SpriteRender m_phaseTimeBarRender;

	};



	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Boss* m_boss = nullptr;

	PlayerUI m_playerUI;			//�v���C���[��UI
	MonsterUI m_monsterUI;			//�����X�^�[��UI
	phaseUI m_phaseUI;				//�t�F�[�Y��UI

	Vector3 m_playerWhiteHpScale = g_vec3One;		//�v���C���[�̔���HP�o�[

	Vector3 m_bossHpWhiteScale = g_vec3One;			//�{�X�̔���HP�o�[

	float m_gaugeTimer = 0.0f;

	float m_wipeSize = 0;

	

	float m_nowPlayerWhiteHp = 0.0f;
	float m_nowBossWhiteHp = 0.0f;

	int m_oldPhaseMonstersNum = 0;



};

