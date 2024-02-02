#pragma once

#include "MobMonster.h"

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

	/// <summary>
	/// �w�肵���ԍ��̃����X�^�[���擾
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	MobMonster* GetMobMonster(int number)
	{
		return m_mobMonsters[number];
	}

	/// <summary>
	/// �v���C���[�ɋ߂������X�^�[�̐����擾
	/// </summary>
	/// <returns></returns>
	const int& GetMonstersNearPlayerCount() const
	{
		return m_monstersNearPlayerCount;
	}

	/// <summary>
	/// ���t���[���s������
	/// </summary>
	void Execute();

	/// <summary>
	/// �w�肵�������X�^�[���v���C���[�ɋ߂������ׂ�
	/// </summary>
	/// <param name="mobMonster"></param>
	bool SearchMonsterNearPlayer(MobMonster* mobMonster);

	/// <summary>
	/// �S�Ẵ����X�^�[(�{�X�܂�)�̃_���[�W���󂯂��邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAllMonsterDamgeHitFlag(bool flag);

private:

	/// <summary>
	/// �v���C���[�ɋ߂������X�^�[�̐��𒲂ׂ�
	/// </summary>
	void SearchMonstersNearPlayer();

	
	void SortMonsterToPlayerLength(float array[][2], int maxValue);


	

private:
	Boss* m_bossInstance = nullptr;
	Player* m_playerInstance = nullptr;		//�v���C���[�̃C���X�^���X

	std::vector<MobMonster*> m_mobMonsters;			//���u�����X�^�[�̃��X�g

	int m_monstersNearPlayerCount = 0;


	static CharactersInfoManager* m_charactersInfoInstance;

};

