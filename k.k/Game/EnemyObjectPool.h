#pragma once
#include "IObjectPool.h"

#include "EnemyBase.h"

class EnemyBase;
class Slime;



/// <summary>
/// �G�l�~�[�̃I�u�W�F�N�g�v�[���Ǘ��N���X�B�V���O���g���p�^�[��
/// </summary>
class EnemyObjectPool : public Noncopyable
{
private:
	EnemyObjectPool();
	~EnemyObjectPool();

public:

	static EnemyObjectPool* m_instance;		//�G�l�~�[�I�u�W�F�N�g�v�[���̗B��̃C���X�^���X

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns></returns>
	static EnemyObjectPool* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// 
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	/// <summary>
	/// ����������
	/// </summary>
	void Init();



	/// <summary>
	/// �I�u�W�F�N�g�𐶐����ăL���[�Ɋi�[�B
	/// EnemyBase�N���X���p�����Ă���I�u�W�F�N�g�̂݊i�[�ł��܂��B
	/// �I�u�W�F�N�g�͔�A�N�e�B�u������܂��B
	/// </summary>
	/// <typeparam name="T">�I�u�W�F�N�g�̃N���X</typeparam>
	/// <param name="saveKeyName">�I�u�W�F�N�g��ۑ�����y�A�̃L�[�̖��O</param>
	template <typename T>
	void OnCreateEnemy(const char* saveKeyName)
	{
		//�I�u�W�F�N�g�𐶐�
		EnemyBase* createobj = NewGO<T>(0, saveKeyName);

		//IGameObject���p�����Ă���Ȃ�擾
		IGameObject* targetObj = dynamic_cast<IGameObject*>(createobj);
		////IGameObject���p������Ă��Ȃ��Ȃ�G���[
		if (targetObj == nullptr) std::abort();

		//��A�N�e�B�u������
		targetObj->Deactivate();


		//�I�u�W�F�N�g�v�[���̒��Ɉ����̃I�u�W�F�N�g�̖��O�����邩���ׂ�
		auto it = m_objectPoolQueue.find(saveKeyName);
		if (it != m_objectPoolQueue.end())
		{
			//�L���[�Ƀv�b�V��
			it->second.push(createobj);
		}
	}


	/// <summary>
	/// �L���[����I�u�W�F�N�g���擾����
	/// </summary>
	/// <param name="getKeyName">�擾����I�u�W�F�N�g���i�[����Ă���L�[</param>
	/// <returns></returns>
	
	/// <summary>
	/// �G�l�~�[���Q�b�g����
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="getKeyName"></param>
	/// <returns></returns>
	template <typename T>
	EnemyBase* OnGetEnemy(const char* getKeyName)
	{
		//�L���[����L�[�ɑΉ�����I�u�W�F�N�g���擾
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(getKeyName);

		//�L���[����Ȃ�V�����������Ċi�[
		if (getQueue.empty())
		{
			//�G�l�~�[�𐶐�
			OnCreateEnemy<T>(getKeyName);
		}
		//��ԏォ�����Ă���
		auto* getEnemy = getQueue.front();

		//�Q�b�g�������N���X�������Ă��邩���ׂ�
		getEnemy = dynamic_cast<T*>(getEnemy);

		if (getEnemy == nullptr)
		{
			return nullptr;
		}

		//�A�N�e�B�u������
		getEnemy->Activate();
		//���ɃX�^�[�g�֐��̏��������Ă���Ȃ��蒼��
		if (getEnemy->IsStart())
		{
			getEnemy->ResetStartFlag();
		}

		//�L���[������o��
		getQueue.pop();

		return getEnemy;
	}

	/// <summary>
	/// �I�u�W�F�N�g���L���[�ɕԂ��B�Ԃ��Ƃ��ɔ�A�N�e�B�u������܂��B
	/// </summary>
	/// <param name="keyName">�i�[����L�[�̖��O</param>
	/// <param name="releaseObj">�i�[����I�u�W�F�N�g</param>
	void OnRelease(const char* keyName, EnemyBase* releaseObj)
	{
		//�L���[����L�[�ɑΉ�����I�u�W�F�N�g���擾
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(keyName);
		//
		if (releaseObj == nullptr)
		{
			return;
		}
		//�i�[����O�ɔ�A�N�e�B�u������
		releaseObj->Deactivate();
		//�L���[�Ɋi�[����
		getQueue.push(releaseObj);
	}

	void OnDestroy(const char* keyName)
	{
		//�L���[����L�[�ɑΉ�����I�u�W�F�N�g���擾
		std::queue<EnemyBase*>& getQueue = m_objectPoolQueue.at(keyName);

		if (getQueue.empty())
		{
			return;
		}

		while (!getQueue.empty())
		{
			if (auto* getEnemy = getQueue.front())
			{
				getEnemy->Dead();
			}
			getQueue.pop();
		}
	}

	/// <summary>
	/// �I�u�W�F�N�g�v�[���̒��̃A�N�e�B�u�ȃI�u�W�F�N�g�̐��𒲂ׂ�
	/// </summary>
	/// <returns>�A�N�e�B�u�I�u�W�F�N�g�̐�</returns>
	const int GetActiveObjCount()
	{
		int activeCount = 0;

		for (auto& pool: m_objectPoolQueue)
		{
			std::queue<EnemyBase*> getPool = pool.second;

			while (!getPool.empty())
			{
				EnemyBase* enemy = getPool.front();

				getPool.pop();

				if (enemy->IsActive())
				{
					activeCount++;
				}
			}
		}

		return activeCount;
	}


private:

	int m_initPoolSize = 10;

	std::map<const char*, std::queue<EnemyBase*>> m_objectPoolQueue;	//�I�u�W�F�N�g�v�[���̃}�b�v



};

