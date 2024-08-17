#pragma once

#include "WeaponStatus.h"

#include "BraveAnimClipAndStateInfo.h"

class WeaponStatus;
class Brave;

using namespace BraveState;

/// <summary>
/// ����̊��N���X
/// </summary>
class WeaponBase:public IGameObject
{
public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~WeaponBase() = default;

	/// <summary>
	/// ����̏��
	/// </summary>
	enum EnWeaponState
	{
		enArmed,			//�������
		enStowed			//���[���
	};

	/// <summary>
	/// �R���{�X�e�[�g
	/// </summary>
	enum EnComboState
	{
		enCombo_None = static_cast<EnComboState>(enBraveState_FirstAttack - 1),//�E�҂̍U���X�e�[�g�ɒl�����킹��
		enCombo_First = enBraveState_FirstAttack,
		enCombo_Second,
		enCombo_Third,
		enCombo_End,

	};

	/// <summary>
	/// �X�L�������H���ł̃X�e�[�g
	/// </summary>
	enum EnSkillProcessState
	{
		enStart,
		enMain,
	};

	/// <summary>
	/// ������
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �U������
	/// </summary>
	virtual void AttackAction() = 0;



	/// <summary>
	/// ���݂̕���̏�Ԃ�ݒ�
	/// </summary>
	/// <param name="setWeaponState"></param>
	void SetCurrentWeaponState(EnWeaponState setWeaponState)
	{
		m_enWeaponState = setWeaponState;
	}

	/// <summary>
	/// ���݂̕���̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	EnWeaponState GetCurrentWeaponState()
	{
		return m_enWeaponState;
	}

	/// <summary>
	/// ���݂̃R���{�X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	EnComboState GetCurrentComboState()
	{
		return m_enComboState;
	}

	/// <summary>
	/// �U���ł̈ړ��t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackActionMove(bool flag)
	{
		m_isAttackActionMove = flag;
	}
	/// <summary>
	/// �U���ňړ����邩
	/// </summary>
	/// <returns></returns>
	bool IsAttackActionMove()
	{
		return m_isAttackActionMove;
	}



	/// <summary>
	/// ���[��Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeStowedState();

	/// <summary>
	/// ������Ԃɐ؂�ւ���
	/// </summary>
	virtual void ChangeArmedState();


	/// <summary>
	/// �R���{�U��������i�߂�
	/// </summary>
	virtual void ProceedComboAttack(){}
	/// <summary>
	/// �R���{�U�����������Z�b�g����
	/// </summary>
	virtual void ResetComboAttack(){}

	/// <summary>
	/// ����A�h��A�N�V�������I��邩
	/// </summary>
	/// <returns>�I���Ȃ�true</returns>
	virtual bool IsEndDefensiveAction() { return false; }

	/// <summary>
	/// ����A�h��A�N�V�����ɓ������Ƃ��̍ŏ��̏���
	/// </summary>
	virtual void EntryDefensiveActionProcess() {}
	/// <summary>
	/// ����A�h��A�N�V�������̍X�V����
	/// </summary>
	virtual void UpdateDefensiveActionProcess() {}
	/// <summary>
	/// ����A�h��A�N�V�����𔲂��o�����̏���
	/// </summary>
	virtual void ExitDefensiveActionProcess() {}


	/// <summary>
	/// �ʏ�U���X�e�[�g�ɓ��������̏���
	/// </summary>
	virtual void EntryNormalAttackProcess(EnComboState comboState) {}
	/// <summary>
	/// �ʏ�U���X�e�[�g�ł̍X�V����
	/// </summary>
	virtual void UpdateNormalAttackProcess(EnComboState comboState) {}
	/// <summary>
	/// �ʏ�U���X�e�[�g�𔲂��o�����̏���
	/// </summary>
	virtual void ExitNormalAttackProcess(EnComboState comboState) {}


	/// <summary>
	/// �X�L���U���X�e�[�g�ɓ��������̏���
	/// </summary>
	virtual void EntrySkillAttackProcess(EnSkillProcessState skillProcessState) {}
	/// <summary>
	/// �X�L���U���X�e�[�g�ł̍X�V����
	/// </summary>
	virtual void UpdateSkillAttackProcess(EnSkillProcessState skillProcessState) {}
	/// <summary>
	/// �X�L���U���X�e�[�g�𔲂��o�����̏���
	/// </summary>
	virtual void ExitSkillAttackProcess(EnSkillProcessState skillProcessState) {}


protected:

	/// <summary>
	/// ���̕�������^�[�Q�b�g�Ɍ������������v�Z����
	/// </summary>
	/// <param name="origin">�^�[�Q�b�g�Ɍ������x�N�g�����v�Z����Ƃ��̋N�_</param>
	/// <param name="nowDirection">���݌����Ă������</param>
	/// <param name="searchRadius">�^�[�Q�b�g�ɂł���͈�(���a)</param>
	/// <param name="maxDot">��r������ρB�i����ȏ㎗�Ă�����V���������ɂȂ�j</param>
	/// <returns></returns>
	Vector3 CalcAutoAimAtTarget(
		Vector3 origin, Vector3 nowDirection,float searchRadius, float maxDot);




protected:

	Brave* m_brave = nullptr;		//�v���C���[�̃C���X�^���X

	WeaponStatus m_status;			//�X�e�[�^�X

	EnWeaponState m_enWeaponState = enStowed;			//����̏�ԁB�f�t�H���g�͎��[���


	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };		//���[��Ԏ��̍��W


	EnComboState m_enComboState = enCombo_None;



	bool m_isAttackActionMove = false;			//�U���ňړ����邩


};

