#pragma once

#include "MobMonster.h"

class Brave;		//�E�҃N���X
class Lich;			//���b�`(�{�X)
class MobMonster;	//���u�����X�^�[
class Player;		//�v���C���[
class Boss;			//�{�X

//todo �擾����̂��|�C���^�ł���K�v������̂�

class CharactersInfoManager:public Noncopyable
{
private:
	CharactersInfoManager();
	~CharactersInfoManager();

public:
	/// <summary>
	/// �C���X�^���X�̍쐬�B
	/// </summary>
	static void CreateInstance()
	{
		m_charactersInfoInstance = new CharactersInfoManager;
	}
	/// <summary>
	/// �C���X�^���X�̔j���B
	/// </summary>
	static void DeleteInstance()
	{


		delete m_charactersInfoInstance;
		m_charactersInfoInstance = nullptr;
	}
	/// <summary>
	/// �C���X�^���X�̎擾�B
	/// �L�����N�^�[���}�l�[�W���[�̊֐����g���Ƃ��͂��̊֐����o�R����
	/// </summary>
	/// <returns></returns>
	static CharactersInfoManager* GetInstance()
	{
		return m_charactersInfoInstance;
	}

	/// <summary>
	/// �E�҃C���X�^���X�̐ݒ�
	/// </summary>
	/// <param name="brave"></param>
	void SetBraveInstance(Brave* brave)
	{
		//���ɃC���X�^���X���������Ă���Ȃ�
		if (m_braveInstance != nullptr)
		{
			std::abort();
		}
		m_braveInstance = brave;
	}
	/// <summary>
	/// �E�҃C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	Brave* GetBraveInstance()
	{
		return m_braveInstance;
	}

	/// <summary>
	/// �v���C���[�C���X�^���X�̐ݒ�
	/// </summary>
	/// <param name="brave"></param>
	void SetPlayerInstance(Player* player)
	{
		//���ɃC���X�^���X���������Ă���Ȃ�
		if (m_playerInstance != nullptr)
		{
			std::abort();
		}
		m_playerInstance = player;
	}
	/// <summary>
	/// �v���C���[�C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	Player* GetPlayerInstance()
	{
		return m_playerInstance;
	}
	/// <summary>
	/// �v���C���[�C���X�^���X�̍폜
	/// </summary>
	void DeletePlayerInstance()
	{
		//DeleteGO(m_playerInstance);
	}

	/// <summary>
	/// ���b�`�C���X�^���X�̐ݒ�
	/// </summary>
	/// <param name="brave"></param>
	void SetLichInstance(Lich* lich)
	{
		//���ɃC���X�^���X���������Ă���Ȃ�
		if (m_lichInstance != nullptr)
		{
			std::abort();
		}
		m_lichInstance = lich;
	}
	/// <summary>
	/// ���b�`�C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	Lich* GetLichInstance()
	{
		return m_lichInstance;
	}

	/// <summary>
	/// �{�X�C���X�^���X�̐ݒ�
	/// </summary>
	/// <param name="boss"></param>
	void SetBossInstance(Boss* boss)
	{
		//���ɃC���X�^���X���������Ă���Ȃ�
		if (m_bossInstance != nullptr)
		{
			std::abort();
		}
		m_bossInstance = boss;
	}
	/// <summary>
	/// �{�X�C���X�^���X�̎擾
	/// </summary>
	/// <returns></returns>
	Boss* GetBossInstance()
	{
		return m_bossInstance;
	}

	/// <summary>
	/// �{�X�C���X�^���X�̍폜
	/// </summary>
	void DeleteBossInstance()
	{
		//DeleteGO(m_bossInstance);
	}


	/// <summary>
	/// ���u�����X�^�[�̃��X�g�ɏ���ǉ�
	/// </summary>
	/// <param name="mobMonster"></param>
	void AddMobMonsterFromList(MobMonster* mobMonster)
	{
		m_mobMonsters.emplace_back(mobMonster);
	}
	/// <summary>
	/// �w�肵�����u�����X�^�[�����X�g����폜
	/// </summary>
	/// <param name="mobMonster"></param>
	void RemoveMobMonsterFormList(MobMonster* mobMonster)
	{
		std::vector<MobMonster*>::iterator it = std::find(
			m_mobMonsters.begin(), // ���u�����X�^�[�̃��X�g�̍ŏ�
			m_mobMonsters.end(),   // ���u�����X�^�[�̃��X�g�̍Ō�
			mobMonster                     // ���������A�N�^�[
		);
		if (it != m_mobMonsters.end()) {
			m_mobMonsters.erase(it);
		}

	}
	/// <summary>
	/// ���u�����X�^�[�̃��X�g�̎擾
	/// </summary>
	/// <returns></returns>
	std::vector<MobMonster*>& GetMobMonsters()
	{
		return m_mobMonsters;
	}


private:
	Brave* m_braveInstance;			//�E�҂̃C���X�^���X
	Lich* m_lichInstance;			//���b�`�̃C���X�^���X
	Boss* m_bossInstance;
	Player* m_playerInstance;		//�v���C���[�̃C���X�^���X

	std::vector<MobMonster*> m_mobMonsters;			//���u�����X�^�[�̃��X�g


	static CharactersInfoManager* m_charactersInfoInstance;

};

