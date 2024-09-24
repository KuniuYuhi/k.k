#pragma once
//#include "Level3DRender.h"
//#include "Level2DRender.h"

class BossStage1;
class GameCamera;
class ResultSeen;
class GameUI;
class Fade;
class EntryBoss;
class BattleStart;
class Pause;
class Brave;
class WaveManager;
class Summoner;

class GameFinishCamera;


class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	
	///////////////////////////////////////////////////////
	//�\���́A�񋓌^�̐錾
	///////////////////////////////////////////////////////

	enum EnGameStep
	{
		enGameStep_FadeOut,
		enGameStep_FadeNone,
		enGameStep_Battle
	};
	
	/// <summary>
	/// �Q�[�����I��������̃J�����������鑊��
	/// </summary>
	enum EnGameEndCameraState
	{
		enClearCameraState_None,
		enClearCameraState_Lich,	//���b�`
		enClearCameraState_Player	//�E��
	};

	


	///////////////////////////////////////////////////////////////////
	//���̑��̊֐�
	///////////////////////////////////////////////////////////////////

	
	/// <summary>
	/// �Q�[��UI����
	/// </summary>
	void CreateGameUI();

	/// <summary>
	/// �v���C���[���S�ł������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerAnnihilationFlag(bool flag)
	{
		m_playerAnnihilationFlag = flag;
	}

	/// <summary>
	/// ���U���g��ʑJ�ڏ���
	/// </summary>
	void GoResult();


	/// <summary>
	/// �{�X�̓o�ꃀ�[�r�[���I��������̃t���O
	/// </summary>
	/// <param name="flag"></param>
	void SetBossMovieFlag(bool flag)
	{
		m_bossMovieEndFlag = flag;
	}
	
	
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

	SkyCube* GetSkyCube()
	{
		return m_skyCube;
	}

	void DeleteGameCamera();

	void DeleteThis();

	/// <summary>
	/// �v���C���[�ƃJ�����̐���
	/// </summary>
	void CreatePlayerAndCamera();

	/// <summary>
	/// �{�X�̐���
	/// </summary>
	void CreateBoss();

	void UpdateGameSystem();

	/// <summary>
	/// �o�g���t�F�[�Y�N���X����
	/// </summary>
	void CreateBattleWave();


	void GameOutComeProcess();

private:
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̏�����
	/// </summary>
	void InitGameObject();
	/// <summary>
	/// ���C�g�̏�����
	/// </summary>
	void InitLighting();

	/// <summary>
	/// �t�F�[�h
	/// </summary>
	/// <returns></returns>
	bool Fadecomplete();
	
	/// <summary>
	/// �X�J�C�L���[�u�̏�����
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// BGM������������������
	/// </summary>
	void CalcMuteBGM();

	//void Render(RenderContext& rc);
	

	EnGameStep					m_enGameStep = enGameStep_FadeOut;

	EnGameEndCameraState		m_clearCameraState = enClearCameraState_None;

	Pause*						m_pause = nullptr;
	Fade*						m_fade = nullptr;

	BossStage1*					m_bossStage1 = nullptr;
	GameCamera*					m_gameCamera = nullptr;
	
	SkyCube*					m_skyCube = nullptr;
	const Vector3				m_skyPos = { 0.0f,-700.0f,0.0f };

	GameUI*						m_gameUI = nullptr;
	EntryBoss*					m_entryBoss = nullptr;
	BattleStart*				m_battleStart = nullptr;
	Brave* m_brave = nullptr;
	WaveManager* m_waveManager = nullptr;
	Summoner* m_summoner = nullptr;

	bool						m_ChaseBossFlag = false;						//�{�X�����邩�̃t���O

	bool						m_playerAnnihilationFlag = false;

	bool						m_bossMovieEndFlag = false;						//�{�X�̓o�ꃀ�[�r�[���I�������

	bool						m_displayResultFlag = false;

	bool						m_muteBGMFlag = false;							//BGM���~���[�g���邩�̃t���O
	float						m_bgmVolume = 0.0f;								//BGM�̉���

	bool						m_battleBGMFlag = false;

};

