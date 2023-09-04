#pragma once

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

	void InitPlayerUI();

	void InitMonsterUI();

	/// <summary>
	/// �v���C���[UI�̏���
	/// </summary>
	void PlayerUIUpdate();
	/// <summary>
	/// ���C���L�����̃X�e�[�^�X�̍X�V
	/// </summary>
	void UpdateMainStatus();
	/// <summary>
	/// �T�u�L�����̃X�e�[�^�X�̍X�V
	/// </summary>
	void UpdateSubStatus();
	/// <summary>
	/// �L�����A�C�R���̐؂�ւ�
	/// </summary>
	bool ChangeCharacterIcon();
	/// <summary>
	/// 
	/// </summary>
	void UpdateCharaIcon();
	/// <summary>
	/// HP��MP�Ȃǂ̃Q�[�W�̃X�P�[�����v�Z����
	/// </summary>
	/// <param name="Maxvalue">�ő�l</param>
	/// <param name="value">�v�Z�������l</param>
	/// <returns>�T�C�Y</returns>
	Vector3 CalcGaugeScale(float Maxvalue, float value);

	void CalcChangeCharaIconCoolTime();


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
	/// 
	/// </summary>
	/// <param name="player"></param>
	void GetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="lich"></param>
	void GetLich(Lich* lich)
	{
		m_lich = lich;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="game"></param>
	void GetGame(Game* game)
	{
		m_game = game;
	}

	/// <summary>
	/// �X�v���C�g�̍��W�A�T�C�Y�A��]��ݒ�
	/// </summary>
	/// <param name="spriterender">�X�v���C�g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

private:

	struct MonsterUI
	{
		FontRender m_hpFont;
		FontRender m_AccumulationDamageFont;

		SpriteRender m_IconRender;
		SpriteRender m_HpFlameRender;
		SpriteRender m_HpFrontRender;
		SpriteRender m_HpBackRender;
	};

	struct PlayerUI
	{
		FontRender m_hpFont;
		FontRender m_mpFont;

		SpriteRender m_MainIconRender;
		SpriteRender m_SubIconRender;

		SpriteRender m_MainIconBaseRender;
		SpriteRender m_SubIconBaseRender;

		SpriteRender m_MainStatusBarRender;
		SpriteRender m_SubStatusBarRender;

		SpriteRender m_MainHpFrontRender;
		SpriteRender m_MainHpBackRender;
		SpriteRender m_MainMpFrontRender;
		SpriteRender m_MainMpBackRender;

		SpriteRender m_SubHpFrontRender;
		SpriteRender m_SubHpBackRender;
		SpriteRender m_SubMpFrontRender;
		SpriteRender m_SubMpBackRender;

		SpriteRender m_SkillCenterRender;
		SpriteRender m_Skill_1FlameRender;
		SpriteRender m_Skill_2FlameRender;
		SpriteRender m_Skill_1FlameInsideRender;
		SpriteRender m_Skill_2FlameInsideRender;

		SpriteRender m_SkillButtonXRender;
		SpriteRender m_SkillButtonYRender;

		SpriteRender m_SkillPowerUpRender;
		SpriteRender m_SkillRotarySlashRender;

		SpriteRender m_SkillFireBallRender;
		SpriteRender m_SkillFlamePillarRender;

		SpriteRender m_ChangeCharacterIconRender;
		SpriteRender m_ChangeCharacterIconBlackRender;
		FontRender m_ChangeCharacterCoolTimeFont;

		SpriteRender m_TimeFlameRender;

	};

	FontRender m_TimerFont;				//��������

	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	PlayerUI m_playerUI;			//�v���C���[�̏���UI
	MonsterUI m_monsterUI;			//�����X�^�[�̏���UI


	float m_oldMainCharaHP = 0.0f;				//�O�t���[���̃��C���L������HP

	float m_gaugeTimer = 0.0f;

	float m_charaIconChangeTimer = 0.0f;
	
	bool m_coolTimeDrawFlag = true;

	float m_wipeSize = 0;

};

