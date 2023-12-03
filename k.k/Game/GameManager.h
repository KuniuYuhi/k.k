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
	//�Q�[���̃V�[��
	enum EnGameSeenState
	{
		enGameSeenState_Fade,
		enGameSeenState_GameStart,
		enGameSeenState_AppearanceBoss,
		enGameSeenState_Game,
		enGameSeenState_Pause,
		enGameSeenState_GameOver,
		enGameSeenState_GameClear
	};

	enum EnOutComeState
	{
		enOutComeState_None,
		enOutComeState_PlayerWin,
		enOutComeState_PlayerLose,
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
	/// �Q�[���V�[���X�e�[�g��ݒ�
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
	/// �{�X�N���X����
	/// </summary>
	void CreateBoss();




	/// <summary>
	/// �������Ԃ̌v�Z
	/// </summary>
	void CalcTimeLimmit();

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


public:
	
	EnGameSeenState m_enGameSeenState = enGameSeenState_GameStart;
	EnOutComeState m_enOutComeState = enOutComeState_None;


	static GameManager* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

private:
	float m_minute = 3.0f;			//��������
	float m_second = 0.0f;

	bool m_playerWinFlag = false;
	bool m_playerLoseFlag = false;

};



