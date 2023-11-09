#pragma once

#include "WeaponBase.h"

class Arrow;

class Bow :public WeaponBase
{
public:
	Bow();
	~Bow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;

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

	/// <summary>
	/// �U�����q�b�g�������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackHitFlag(bool flag);
	/// <summary>
	/// �U�����q�b�g�������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetAttackHitFlag() const;
	
	/// <summary>
	/// ��_���[�W�ȂǂŁA����ɖ߂��͂��������ϐ��������I�Ƀ��Z�b�g����
	/// </summary>
	void ResetVariable() override
	{
		m_RotationDelectionFlag = false;
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
	void ProcessLongRangeAttack();
	/// <summary>
	/// �X�L�������������̏���
	/// </summary>
	void SkillShot();

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

	/// <summary>
	/// ��̐�������
	/// </summary>
	void CreateArrow();

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

	float m_ChargeTimer = 0.0f;		//�X�L���̃`���[�W�^�C�}�[
};

