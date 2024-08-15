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
	/// �o�g���̏��s��ݒ肷��
	/// </summary>
	/// <param name="setBattleOutCome"></param>
	void SetBattleOutCome(EnBattleOutCome setBattleOutCome)
	{
		m_battleOutCome = setBattleOutCome;
	}

	/// <summary>
	/// �o�g���̏��s���擾����
	/// </summary>
	/// <returns></returns>
	const EnBattleOutCome& GetBattleOutCome()
	{
		return m_battleOutCome;
	}



private:

	std::unique_ptr<GameSceneContext> m_gameSceneContext = nullptr;		//�Q�[���V�[���R���e�L�X�g
																		//���X�}�[�g�|�C���^�Ő���

	bool m_isSceneChangeable = false;				//�V�[�����؂�ւ��\��


	EnBattleOutCome m_battleOutCome = enBattleOutCome_None;

};

