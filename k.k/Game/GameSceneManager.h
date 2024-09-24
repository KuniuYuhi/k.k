#pragma once

/*
*@brief �i�s�󋵂̊Ǘ�
*/

#include "AllGameSceneState.h"

class GameSceneContext;

/// <summary>
/// �V���O���g���p�^�[���ō쐬
/// </summary>
class GameSceneManager:public Noncopyable
{
private:
	GameSceneManager();
	~GameSceneManager();

	/// <summary>
	/// ����
	/// </summary>
	struct Timer
	{
		void SetMinute(float Minute) { minute = Minute; }

		void SetSecond(float Second) { second = Second; }

		void SubMinute(float value) { minute -= value;  }

		void SubSecond(float value) { second -= value; }

		float GetMinute() { return minute; }
		float GetSecond() {	return second; }

		float minute = 0.0f;			//��
		float second = 0.0f;			//�b
	};


public:

	/// <summary>
	/// ���s
	/// </summary>
	enum EnBattleOutCome
	{
		enBattleOutCome_PlayerWin,
		enBattleOutCome_PlayerLose,
		enBattleOutCome_None
	};



	static GameSceneManager* m_instance; //�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B
	
	/// <summary>
	/// �C���X�^���X�𐶐��B�Q�[���V�[����ݒ�
	/// </summary>
	/// <param name="setGameSceneState"></param>
	static void CreateInstanceAndSetGameSceneState(EnGameSceneState setGameSceneState);

	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static GameSceneManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// �Q�[���V�[���R���e�L�X�g�𐶐�
	/// </summary>
	/// <param name="setGameSceneState"></param>
	void CreateGameSceneContext(EnGameSceneState setGameSceneState);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="cangeGameSceneState"></param>
	void ChangeGameSceneState(EnGameSceneState cangeGameSceneState);


	/// <summary>
	/// �V�[���؂�ւ��\���t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsSceneChangeableFlag(bool flag)
	{
		m_isSceneChangeable = flag;
	}

	/// <summary>
	/// �V�[���؂�ւ��\���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsSceneChangeableFlag() const
	{
		return m_isSceneChangeable;
	}

	/// <summary>
	/// ���݂̃Q�[���V�[���X�e�[�g���擾����
	/// </summary>
	/// <returns></returns>
	const EnGameSceneState& GetCurrentGameSceneState();

	/// <summary>
	/// �Q�[���^�C�}�[���X�g�b�v����
	/// </summary>
	void StopGameTimer();


	/// <summary>
	/// �o�g���̏��s��ݒ肷��
	/// </summary>
	/// <param name="setBattleOutCome"></param>
	void SetBattleOutCome(EnBattleOutCome setBattleOutCome)
	{
		m_battleOutCome = setBattleOutCome;
	}
	/// <summary>
	/// �v���C���[�̏���
	/// </summary>
	void SetOutComePlayerWin()
	{
		m_battleOutCome = enBattleOutCome_PlayerWin;
	}
	/// <summary>
	/// �v���C���[�̕���
	/// </summary>
	void SetOutComPlayerLose()
	{
		m_battleOutCome = enBattleOutCome_PlayerLose;
	}

	/// <summary>
	/// �v���C���[�����������H
	/// </summary>
	/// <returns></returns>
	bool IsPlayerWin()
	{
		return m_battleOutCome == enBattleOutCome_PlayerWin;
	}
	/// <summary>
	/// �G�l�~�[���������������H
	/// </summary>
	/// <returns></returns>
	bool IsEnemyWin()
	{
		return m_battleOutCome == enBattleOutCome_PlayerLose;
	}

	/// <summary>
	/// ���s�����������H
	/// </summary>
	/// <returns>�������Ȃ�true</returns>
	bool IsGameOutcome()
	{
		return m_battleOutCome != enBattleOutCome_None;
	}

	/// <summary>
	/// �o�g���̏��s���擾����
	/// </summary>
	/// <returns></returns>
	const EnBattleOutCome& GetBattleOutCome()
	{
		return m_battleOutCome;
	}

	/// <summary>
	/// �{�X���������܂���
	/// </summary>
	void BossDelete()
	{
		m_isBossDelete = true;
	}

	void ResetBossDelete()
	{
		m_isBossDelete = false;
	}

	/// <summary>
	/// �{�X���������ꂽ��
	/// </summary>
	/// <returns></returns>
	bool IsBossDelete()
	{
		return m_isBossDelete;
	}

	/// <summary>
	/// �^�C�}�[���X�^�[�g������
	/// </summary>
	void StartGameTimer();


	float GetGameTimerSecond()
	{
		return m_gameTimer.GetSecond();
	}

	float GetGameTimerMinute()
	{
		return m_gameTimer.GetMinute();
	}


private:

	/// <summary>
	/// �^�C�}�[���X�V
	/// </summary>
	void UpdateGameTimer();

	/// <summary>
	/// �Q�[���^�C�}�[���v�Z
	/// </summary>
	void CalcGameTimer();


private:

	std::unique_ptr<GameSceneContext> m_gameSceneContext = nullptr;		//�Q�[���V�[���R���e�L�X�g
																		//���X�}�[�g�|�C���^�Ő���

	bool m_isSceneChangeable = false;				//�V�[�����؂�ւ��\��


	bool m_isBossDelete = false;					//�{�X�͏������ꂽ���H

	EnBattleOutCome m_battleOutCome = enBattleOutCome_None;

	Timer m_gameTimer;

	bool m_isStartGameTimer = false;

	bool m_isTimeOver = false;
	

};

