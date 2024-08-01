#pragma once


/// <summary>
/// �I�u�W�F�N�g�v�[���̊��N���X
/// </summary>
/// 
/// 
/// ������G�l�~�[�̃I�u�W�F�N�g�v�[���N���X�Ƃ��č��΂悭�ˁH

//�������������Ă����Ĕ�A�N�e�B�u�����Ă���


class IObjectPool
{
public:

	virtual ~IObjectPool() = default;

	///// <summary>
	///// �I�u�W�F�N�g�v�[�����X�g�Ƀy�A�^�̕ϐ���}��
	///// </summary>
	///// <param name="insertPair"></param>
	//void Insert(std::pair<std::string, T*> insertPair)
	//{
	//	m_objectMap.insert(insertPair);
	//}


	/// <summary>
	/// �I�u�W�F�N�g�𐶐����ăL���[�Ɋi�[�B�I�u�W�F�N�g�͔�A�N�e�B�u������܂��B
	/// </summary>
	/// <typeparam name="T">�I�u�W�F�N�g�̃N���X</typeparam>
	/// <param name="saveKeyName">�I�u�W�F�N�g��ۑ�����y�A�̃L�[�̖��O</param>
	template<typename T>
	void OnCreate(const char* saveKeyName)
	{
		//�I�u�W�F�N�g�𐶐�
		auto* createobj = NewGO<T>(0, saveKeyName);
		
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
	IGameObject* OnGet(const char* getKeyName)
	{
		std::queue<IGameObject*>& getQueue = m_objectPoolQueue.at(getKeyName);

		if (getQueue.empty())
		{
			OnCreate<IGameObject>(getKeyName);
		}

	}


protected:

	int m_initPoolSize = 10;

	std::map<const char*, std::queue<IGameObject*>> m_objectPoolQueue;	//�I�u�W�F�N�g�v�[���̃}�b�v

	

};

