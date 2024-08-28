#pragma once

class MobEnemyBase;

/// <summary>
/// �G�l�~�[�̏����Ǘ�����}�l�[�W���[
/// </summary>
class EnemyManager:public Noncopyable
{
public:

	EnemyManager();
	~EnemyManager();

	static EnemyManager* m_instance;

	/// <summary>
	/// �C���X�^���X�𐶐�
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void DeleteInstance();

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static EnemyManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ���u�G�l�~�[���߂Â��鋗�����Ǘ�
	/// </summary>
	void ControlEnemyDistances(Vector3 targetPosition);


	/// <summary>
	/// ���u�G�l�~�[�����X�g�ɒǉ�
	/// </summary>
	/// <param name="addMobEnemy"></param>
	void AddMobEnemyToList(MobEnemyBase* addMobEnemy)
	{
		m_mobEnemyList.emplace_back(addMobEnemy);
	}
	/// <summary>
	/// ���u�G�l�~�[���X�g���N���A
	/// </summary>
	void CrearMobEnemyList()
	{
		m_mobEnemyList.clear();
	}



	std::vector<MobEnemyBase*> GetMobEnemyList()
	{
		return m_mobEnemyList;
	}


	/// <summary>
	/// ���u�G�l�~�[�����X�g����폜
	/// </summary>
	/// <param name="removeMobEnemy"></param>
	void RemoveMobEnemyToList(MobEnemyBase* removeMobEnemy)
	{
		std::vector<MobEnemyBase*>::iterator it = std::find(
			m_mobEnemyList.begin(), // ���u�����X�^�[�̃��X�g�̍ŏ�
			m_mobEnemyList.end(),   // ���u�����X�^�[�̃��X�g�̍Ō�
			removeMobEnemy                     // ���������A�N�^�[
		);
		if (it != m_mobEnemyList.end()) {
			m_mobEnemyList.erase(it);
		}
	}

	/// <summary>
	/// �G�l�~�[�̃��X�g���^�[�Q�b�g�Ɍ����������ɋ߂����ɓ���ւ���
	/// </summary>
	/// <param name="targetPosition"></param>
	void SortEnemiesByDistanceToTarget(Vector3 targetPosition);

private:

	







private:

	//�{�X�G�l�~�[���

	std::vector<MobEnemyBase*> m_mobEnemyList;		//���u�G�l�~�[���X�g


	int m_approachMobEnemyAmount = 5;				//�ڋ߂ł��郂�u�G�l�~�[�̐�

	
};

