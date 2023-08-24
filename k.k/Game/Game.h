#pragma once
#include "Level3DRender.h"
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
	void Render(RenderContext& rc);
	/// <summary>
	/// �t�F�[�h
	/// </summary>
	/// <returns></returns>
	bool Fadecomplete();

	void CreateBoss();

	void InitSkyCube();

	void GoResult();

	void Spotmove();

	const Vector3 GetPosition() const
	{
		return m_position;
	}

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

	bool IsBossMovieSkipTime();

	enum EnGameState
	{
		enGameState_Fade,
		enGameState_GameStart,
		enGameState_AppearanceBoss,
		enGameState_Game,
		enGameState_Pause,
		enGameState_GameOver,
		enGameState_GameClear
	};

	/// <summary>
	/// �X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageState();

	void OnProcessGameStartTransition();
	void OnProcessAppearanceBossTransition();
	void OnProcessGameTransition();
	void OnProcessGameOverTransition();
	void OnProcessGameClearTransition();

	/// <summary>
	/// ���̃Q�[���X�e�[�g��ݒ�
	/// </summary>
	/// <param name="nextgamestate"></param>
	void SetNextGameState(EnGameState nextgamestate)
	{
		m_enGameState = nextgamestate;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	EnGameState GetNowGameState()
	{
		return m_enGameState;
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

	enum EnClearCameraState
	{
		enClearCameraState_None,
		enClearCameraState_Lich,
		enClearCameraState_Player
	};

	void SetClearCameraState(EnClearCameraState nextClearCameraState)
	{
		m_clearCameraState = nextClearCameraState;
	}

	EnClearCameraState GetClearCameraState()
	{
		return m_clearCameraState;
	}

private:

	enum EnFadeState
	{
		enFadeState_None,
		enFadeState_StartToBoss,
		enFadeState_BossToPlayer,

	};
	EnFadeState m_enFadeState = enFadeState_None;

	EnClearCameraState m_clearCameraState = enClearCameraState_None;

	Fade* m_fade = nullptr;
	Player* m_player = nullptr;
	Lich* m_lich = nullptr;
	BossStage1* m_bossStage1 = nullptr;
	GameCamera* m_gameCamera = nullptr;
	ResultSeen* m_result = nullptr;
	SkyCube* m_skyCube = nullptr;
	const Vector3 m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI* m_gameUI = nullptr;
	EntryBoss* m_entryBoss = nullptr;
	BattleStart* m_battleStart = nullptr;

	ModelRender model;


	//Level2DRender level2DSp;

	//spotLight
	Vector3 spPosition = Vector3::Zero;
	Vector3 spDirection = Vector3::Zero;

	Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	EnGameState m_enGameState = enGameState_GameStart;

	bool m_gameStartCreateFlag = false;			//�o�g���X�^�[�g�N���X�����t���O

	bool m_DeathBossFlag = false;				//�{�X�����ꂽ�̃t���O
	bool m_ChaseBossFlag = false;				//�{�X�����邩�̃t���O

	bool m_createResultFlag = false;

	bool m_playerAnnihilationFlag = false;

	bool m_bossCreateFlag = false;

	bool m_bossMovieEndFlag = false;			//�{�X�̓o�ꃀ�[�r�[���I�������

	bool m_displayResultFlag = false;
	
	const float m_fadeTime = 3.0f;
	const float m_cameraZoomOutTime = 2.0f;
	float m_cameraZoomOutTimer = 0.0f;

	const float m_bossMovieSkipTime = 1.0f;
	float m_bossMovieSkipTimer = 0.0f;

};

