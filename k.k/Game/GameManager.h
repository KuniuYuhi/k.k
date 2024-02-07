#pragma once

/*
*@brief �i�s�󋵂̊Ǘ�
*/



class GameManager :public Noncopyable
{
public:
	//////////////////////////////////////////////////////////////
	//�\���́A�񋓌^�̐錾
	//////////////////////////////////////////////////////////////
	
	/// <summary>
	/// �Q�[���̃V�[��
	/// </summary>
	enum EnGameSeenState
	{
		enGameSeenState_Fade,
		enGameSeenState_GameStart,
		enGameSeenState_AppearanceBoss,
		enGameSeenState_Game,
		enGameSeenState_Pause,
		enGameSeenState_GameOver,
		enGameSeenState_GameClear,
		enGameSeenState_GameEnd
	};
	/// <summary>
	/// ���s�X�e�[�g
	/// </summary>
	enum EnOutComeState
	{
		enOutComeState_None,
		enOutComeState_PlayerWin,
		enOutComeState_PlayerLose,
	};
	/// <summary>
	/// �t�F�[�Y�X�e�[�g�B���[�v����
	/// </summary>
	enum EnPhaseState
	{
		EnPhaseState_Phase1,		//�t�F�[�Y1
		EnPhaseState_Phase2,		//�t�F�[�Y2
		EnPhaseState_Phase3,		//�t�F�[�Y3
		EnPhaseState_BreakTime,		//�x�e
	};
	/// <summary>
	/// �t�F�[�Y���ł̏����̃X�e�b�v
	/// </summary>
	enum EnPhaseStep
	{
		enPhaseStep_PhaseTime,
		enPhaseStep_ShortBreak,
	};
	/// <summary>
	/// �����X�^�[�𐶐����邩�폜���邩
	/// </summary>
	enum EnDeleteAndCreateMonstersState
	{
		enMonsters_None,
		enMonsters_Create,
		enMonsters_Delete,
	};

private:
	GameManager(EnGameSeenState startGameSeenState);
	~GameManager();
public:

	/// <summary>
	/// ����������
	/// </summary>
	void Init();


	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance(EnGameSeenState startGameSeenState)
	{
		m_instance = new GameManager(startGameSeenState);
	}
	/// <summary>
	/// �C���X�^���X�̔j���B
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static GameManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �Q�[���V�[���X�e�[�g��ݒ�
	/// </summary>
	/// <param name="setGameSeenState"></param>
	void SetGameSeenState(EnGameSeenState setGameSeenState)
	{
		m_enGameSeenState = setGameSeenState;
	}
	/// <summary>
	/// �Q�[���V�[���X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const EnGameSeenState& GetGameSeenState() const
	{
		return m_enGameSeenState;
	}

	/// <summary>
	/// ���s���ʃX�e�[�g��ݒ�
	/// </summary>
	/// <param name="outComeState"></param>
	void SetOutComeState(EnOutComeState outComeState)
	{
		m_enOutComeState = outComeState;
	}
	/// <summary>
	/// ���s���ʃX�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const EnOutComeState& GetOutComeState() const
	{
		return m_enOutComeState;
	}

	/// <summary>
	/// ���݂̃t�F�[�Y���擾
	/// </summary>
	/// <returns></returns>
	const EnPhaseState& GetNowPhaseState() const
	{
		return m_enPhaseState;
	}



	/// <summary>
	/// ���t���[���̍X�V����
	/// </summary>
	void Execute();

	/// <summary>
	/// �o�g���X�^�[�g�N���X�̍쐬
	/// </summary>
	void CreateBattleStartClass();

	/// <summary>
	/// �v���C���[�N���X����
	/// </summary>
	void CreatePlayerClass();
	/// <summary>
	/// �v���C���[�N���X�폜
	/// </summary>
	void DeletePlayerClass();

	/// <summary>
	/// �{�X�N���X����
	/// </summary>
	void CreateBoss();
	/// <summary>
	/// �{�X�N���X�폜
	/// </summary>
	void DeleteBossClass();

	/// <summary>
	/// �Q�[���̌��݂̃X�e�[�g�ƌ����������X�e�[�g������������
	/// </summary>
	/// <param name="searchGameState">�����������Q�[���X�e�[�g</param>
	/// <returns>true�œ����Afalse�ňႤ</returns>
	/*bool IsMatchGameState(EnGameState searchGameState)
	{
		if (GetNowGameState() == searchGameState)
		{
			return true;
		}
		return false;
	}*/

	/// <summary>
	/// �t�F�[�Y�^�C�}�[�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetPhaseTimer() const
	{
		return m_phaseTimer;
	}
	/// <summary>
	/// ���݂̃t�F�[�Y�̃^�C�}�[�̍ő�l���擾
	/// </summary>
	/// <returns></returns>
	const float& GetNowPhaseTimerLimmit() const
	{
		return m_nowPhaseTimerLimmit;
	}

	/// <summary>
	/// �b�̎擾
	/// </summary>
	/// <returns></returns>
	const int GetMinute() const
	{
		return m_minute;
	}
	/// <summary>
	/// �b�̎擾
	/// </summary>
	/// <returns></returns>
	const int GetSecond() const
	{
		return m_second;
	}

	/// <summary>
	/// �v���C���[�̏����t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerWinFlag(bool flag)
	{
		m_playerWinFlag = flag;
	}
	const bool& GetPlayerWinFlag() const
	{
		return m_playerWinFlag;
	}
	/// <summary>
	/// �v���C���[�̔s�k�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerLoseFlag(bool flag)
	{
		m_playerLoseFlag = flag;
	}
	const bool& GetPlayerLoseFlag() const
	{
		return m_playerLoseFlag;
	}

	/// <summary>
	/// �Q�[�����I�������̏������I��������̃t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetGameFinishProcessEndFlag(bool flag)
	{
		m_gameFinishProcessEndFlag = flag;
	}
	/// <summary>
	/// �Q�[�����I�������̏������I��������̃t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetGameFinishProcessEndFlag() const
	{
		return m_gameFinishProcessEndFlag;
	}

	/// <summary>
	/// �{�X���폜���Ă��悢���̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetBossDeleteOkFlag(bool flag)
	{
		m_bossDeleteOkFlag = flag;
	}
	/// <summary>
	/// �{�X���폜���Ă��悢���̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetBossDeleteOkFlag() const
	{
		return m_bossDeleteOkFlag;
	}

	/// <summary>
	/// �����X�^�[�𐶐����邩�폜���邩�̃X�e�[�g��ݒ�
	/// </summary>
	/// <param name="state"></param>
	void SetDAndCMonstersState(EnDeleteAndCreateMonstersState state)
	{
		m_enDAndCMonstersState = state;
	}
	/// <summary>
	/// �����X�^�[�𐶐����邩�폜���邩�̃X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const EnDeleteAndCreateMonstersState& GetDAndCMonstersState() const
	{
		return m_enDAndCMonstersState;
	}

private:

	/// <summary>
	/// �t�F�[�Y�̏���
	/// </summary>
	void ProcessPhase();

	/// <summary>
	/// �t�F�[�Y1,2,3�̏���
	/// </summary>
	void OnProcessPhaseTransition();

	/// <summary>
	/// �t�F�[�Y�ƃt�F�[�Y�̊Ԃ̏��x�e
	/// </summary>
	void ProcessShortBreak();

	/// <summary>
	/// �x�e���Ԃ̏���
	/// </summary>
	void OnProcessBreakTimeTransition();

	/// <summary>
	/// �������Ԃ̌v�Z
	/// </summary>
	void CalcTimeLimmit();
	/// <summary>
	/// ���s����������
	/// </summary>
	void IsOutComeDecided();

	/// <summary>
	/// �t�F�[�Y�̐������Ԃ̌v�Z
	/// </summary>
	void CalcPhaseTime();

public:

	static GameManager* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

private:


	EnGameSeenState m_enGameSeenState = enGameSeenState_GameStart;
	EnOutComeState m_enOutComeState = enOutComeState_None;
	EnPhaseState m_enPhaseState = EnPhaseState_Phase1;

	EnPhaseStep m_enPhaseStep = enPhaseStep_PhaseTime;

	EnDeleteAndCreateMonstersState m_enDAndCMonstersState = enMonsters_Create;

	float m_minute = 3.0f;			//��������
	float m_second = 0.0f;

	bool m_playerWinFlag = false;
	bool m_playerLoseFlag = false;

	float m_phaseTimer = 0.0f;
	float m_nowPhaseTimerLimmit = 0.0f;


	bool m_gameFinishProcessEndFlag = false;	//�Q�[�����I�������̏���(���U���g�܂܂Ȃ�)���I��������̃t���O
	bool m_bossDeleteOkFlag = false;			//�{�X�������폜���Ă��ǂ���Ԃ��̃t���O


};



