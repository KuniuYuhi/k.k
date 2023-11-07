#pragma once

#include "IWeapon.h"

class Arrow;

class Bow :public IWeapon
{
public:
	Bow();
	~Bow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// ���̓����蔻�肪�q�b�g������
	/// </summary>
	/// <returns>�q�b�g������true�A�q�b�g���Ȃ�������false</returns>
	bool IsHitCollision();

	/// <summary>
	/// �X�L���U������
	/// </summary>
	void ProcessSkillAttack();

	/// <summary>
	/// ��̃��[���h���W���擾
	/// </summary>
	/// <returns></returns>
	const Matrix GetArrowMatrix() const
	{
		return m_arrowMatrix;
	}

	/// <summary>
	/// �|�̃E�F�|���X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	const EnWeaponState& GetBowEnWeaponState() const
	{
		return m_enWeaponState;
	}

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;
	/// <summary>
	/// �������U������
	/// </summary>
	void ProcessLongRangeAttack() override;


	/// <summary>
	/// ��̕ێ��t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetStockArrowFlag(bool flag)
	{
		m_stockArrowFlag = flag;
	}
	/// <summary>
	/// ��̕ێ��t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetStockArrowFlag() const
	{
		return m_stockArrowFlag;
	}

private:
	Arrow* m_arrow = nullptr;

	ModelRender m_modelBow;		//�|���f��

	Vector3 m_bowPos = g_vec3Zero;

	Matrix m_bowMatrix = g_matIdentity;
	Matrix m_arrowMatrix = g_matIdentity;

	//������������鎞�̃{�[��ID
	int m_armedBowBoonId = -1;
	int m_armedArrowBoonId = -1;
	//��������܂������̃{�[��ID
	int m_stowedBowBoonId = -1;
	int m_stowedArrowBoonId = -1;

	bool m_stockArrowFlag = false;

};

