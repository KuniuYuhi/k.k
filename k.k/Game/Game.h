#pragma once
//#include "Level3DRender.h"
//#include "Level2DRender.h"

class Player;
class Lich;
class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;
class EntryBoss;
class BattleStart;

class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	
	/// <summary>
	/// �Q�[���̏��s�����܂�����
	/// </summary>
	/// <returns></returns>
	bool IsOutcome();
	/// <summary>
	/// �������̏���
	/// </summary>
	void ProcessWin();
	/// <summary>
	/// �����̏���
	/// </summary>
	void ProcessLose();
	/// <summary>
	/// �^�C���A�b�v�ŕ��������̏���
	/// </summary>
	void ProcessLoseTimeUp();

	/// <summary>
	/// �{�X�����ꂽ���̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDeathBossFlag(bool flag)
	{
		m_DeathBossFlag = flag;
	}

	/// <summary>
	/// �v���C���[���S�ł������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerAnnihilationFlag(bool flag)
	{
		m_playerAnnihilationFlag = flag;
	}

	/// <summary>
	/// �{�X�o�ꃀ�[�r�[�̃X�L�b�v����
	/// </summary>
	/// <returns></returns>
	bool IsBossMovieSkipTime();

	/// <summary>
	/// ���s�X�e�[�g
	/// </summary>
	enum EnOutCome
	{
		enOutCome_Player_Win,
		enOutCome_Player_Lose,
		enOutCome_None
	};
	/// <summary>
	/// ���U���g��ʑJ�ڏ���
	/// </summary>
	/// <param name="outcome">���s�X�e�[�g</param>
	void GoResult(EnOutCome outcome);
	/// <summary>
	/// ���s�X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="outCome"></param>
	void SetEnOutCome(EnOutCome outCome)
	{
		m_enOutCome = outCome;
	}
	/// <summary>
	/// ���s�X�e�[�g�̎擾
	/// </summary>
	/// <returns></returns>
	const EnOutCome& GetEnOutCome() const
	{
		return m_enOutCome;
	}

	enum EnGameState
	{
		enGameState_Fade,
		enGameState_GameStart,
		enGameState_AppearanceBoss,
		enGameState_Game,
		enGameState_Pause,
		enGameState_GameOver,
		enGameState_GameOver_TimeUp,
		enGameState_GameClear
	};

	/// <summary>
	/// ���̃Q�[���X�e�[�g��ݒ�
	/// </summary>
	/// <param name="nextgamestate"></param>
	void SetNextGameState(EnGameState nextgamestate)
	{
		m_enGameState = nextgamestate;
	}
	/// <summary>
	/// �Q�[���X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	EnGameState GetNowGameState()
	{
		return m_enGameState;
	}

	/// <summary>
	/// �^�C���A�b�v��
	/// </summary>
	/// <returns>�^�C���A�b�v�Ȃ�true��Ԃ�</returns>
	bool IsTimeUp()
	{
		return m_enGameState == enGameState_GameOver_TimeUp;
	}
	/// <summary>
	/// ���s�����܂�����
	/// </summary>
	/// <returns></returns>
	bool IsWinnerDecision()
	{
		return m_enGameState == enGameState_GameOver_TimeUp ||
			m_enGameState == enGameState_GameOver ||
			m_enGameState == enGameState_GameClear;
	}

	/// <summary>
	/// �{�X�̓o�ꃀ�[�r�[���I��������̃t���O
	/// </summary>
	/// <param name="flag"></param>
	void SetBossMovieFlag(bool flag)
	{
		m_bossMovieEndFlag = flag;
	}
	/// <summary>
	/// �{�X�����񂾂��̃t���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetDeathBossFlag()
	{
		return m_DeathBossFlag;
	}

	/// <summary>
	/// �Q�[�����I��������̃J�����������鑊��
	/// </summary>
	enum EnGameEndCameraState
	{
		enClearCameraState_None,
		enClearCameraState_Lich,	//���b�`
		enClearCameraState_Player	//�E��
	};
	/// <summary>
	/// �Q�[���G���h�J������ݒ�
	/// </summary>
	/// <param name="nextClearCameraState"></param>
	void SetClearCameraState(EnGameEndCameraState nextClearCameraState)
	{
		m_clearCameraState = nextClearCameraState;
	}
	/// <summary>
	/// �Q�[���G���h�J�������擾
	/// </summary>
	/// <returns></returns>
	EnGameEndCameraState GetClearCameraState()
	{
		return m_clearCameraState;
	}

	/// <summary>
	/// �������Ԃ̕��̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetMinute() const
	{
		return m_minute;
	}
	/// <summary>
	/// �������Ԃ̕b�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetSecond() const
	{
		return m_second;
	}

	/// <summary>
	/// �Q�[���̌��݂̃X�e�[�g�ƌ����������X�e�[�g������������
	/// </summary>
	/// <param name="searchGameState">�����������Q�[���X�e�[�g</param>
	/// <returns>true�œ����Afalse�ňႤ</returns>
	bool IsMatchGameState(EnGameState searchGameState)
	{
		if (GetNowGameState() == searchGameState)
		{
			return true;
		}
		return false;
	}


private:

	/// <summary>
	/// �t�F�[�h
	/// </summary>
	/// <returns></returns>
	bool Fadecomplete();
	/// <summary>
	/// �{�X�̐���
	/// </summary>
	void CreateBoss();
	/// <summary>
	/// �X�J�C�L���[�u�̏�����
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// �������Ԃ̌v�Z
	/// </summary>
	/// <returns></returns>
	bool CalcTimeLimit();
	/// <summary>
	/// BGM������������������
	/// </summary>
	void CalcMuteBGM();

	/// <summary>
	/// �X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();
	//�Q�[���̗���
	void OnProcessGameStartTransition();
	void OnProcessAppearanceBossTransition();
	void OnProcessGameTransition();
	void OnProcessGameOverTransition();
	void OnProcessGameClearTransition();

	enum EnGameStep
	{
		enGameStep_FadeOut,
		enGameStep_FadeNone,
		enGameStep_Battle
	};
	

	void OnProcessGame_FadeOutTransition();
	void OnProcessGame_FadeNoneTransition();
	void OnProcessGame_BattleTransition();

	enum EnFadeState
	{
		enFadeState_None,
		enFadeState_StartToBoss,
		enFadeState_BossToPlayer,

	};


	EnFadeState					m_enFadeState = enFadeState_None;

	EnGameStep					m_enGameStep = enGameStep_FadeOut;

	EnGameEndCameraState		m_clearCameraState = enClearCameraState_None;

	Fade*						m_fade = nullptr;
	Player*						m_player = nullptr;
	Lich*						m_lich = nullptr;
	BossStage1*					m_bossStage1 = nullptr;
	GameCamera*					m_gameCamera = nullptr;
	ResultSeen*					m_result = nullptr;
	SkyCube*					m_skyCube = nullptr;
	const Vector3				m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI*						m_gameUI = nullptr;
	EntryBoss*					m_entryBoss = nullptr;
	BattleStart*				m_battleStart = nullptr;

	ModelRender					model;


	//Level2DRender level2DSp;

	Vector3						m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion					m_rotation = Quaternion::Identity;
	Vector3						m_scale = Vector3::One;

	EnGameState					m_enGameState = enGameState_GameStart;

	EnOutCome					m_enOutCome = enOutCome_None;

	float						m_TimeLimit = 300.0f;							//�������ԎO��
	float						m_minute = 3.0f;								//��
	float						m_second = 0.0f;								//�b


	bool						m_DeathBossFlag = false;						//�{�X�����ꂽ�̃t���O
	bool						m_ChaseBossFlag = false;						//�{�X�����邩�̃t���O

	bool						m_playerAnnihilationFlag = false;

	bool						m_bossMovieEndFlag = false;						//�{�X�̓o�ꃀ�[�r�[���I�������

	bool						m_displayResultFlag = false;
	
	const float					m_fadeTime = 3.0f;
	const float					m_cameraZoomOutTime = 2.0f;
	float						m_cameraZoomOutTimer = 0.0f;

	const float					m_bossMovieSkipTime = 1.0f;
	float						m_bossMovieSkipTimer = 0.0f;

	bool						m_muteBGMFlag = false;							//BGM���~���[�g���邩�̃t���O
	float						m_bgmVolume = 0.0f;								//BGM�̉���

	bool						m_battleBGMFlag = false;

};

