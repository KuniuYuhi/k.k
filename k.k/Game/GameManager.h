#pragma once

/*
*@brief �i�s�󋵂̊Ǘ�
*/



class GameManager :public Noncopyable
{
private:
	GameManager();
	~GameManager();
public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new GameManager;
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


public:
	//�Q�[���̃V�[��
	enum EnSeenState
	{
		enSeen_Tittle,
		enSeen_Battle,
		enSeen_GameClear,
		enSeen_GameOver,
		enSeen_Result,
		enSeen_Feed
	};


	static GameManager* m_instance;		//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B



};



