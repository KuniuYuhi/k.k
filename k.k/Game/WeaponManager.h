#pragma once

/// <summary>
/// ������Ǘ�����}�l�[�W���[�N���X�B
/// �V���O���g���p�^�[��
/// </summary>
class WeaponManager:public Noncopyable
{
private:
	WeaponManager();
	~WeaponManager();

public:

	static WeaponManager* m_instance;//�B��̃C���X�^���X�̃A�h���X���L�^����ϐ��B

	/// <summary>
	/// �C���X�^���X�𐶐�
	/// </summary>
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X���擾�B
	/// </summary>
	/// <returns></returns>
	static WeaponManager* GetInstance()
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


private:



};

