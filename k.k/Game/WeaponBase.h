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
	/// ���g���폜����
	/// </summary>
	virtual void DeleteThis() = 0;

	WeaponStatus GetCommonStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ����̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	int GetWeaponCurrentPower()
	{
		return m_status.GetCurrentPower();
	}
	/// <summary>
	/// ����̃X�L���̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	int GetWeaponSkillPower()
	{
		return m_status.GetSkillPower();
	}

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
	/// �U���ňړ����邩�t���O���擾
	/// </summary>
	/// <returns></returns>
	bool IsAttackActionMove()
	{
		return m_isAttackActionMove;
	}

	void SetDefensiveActionMove(bool flag)
	{
		m_isDefensiveActionMove = flag;
	}

	bool IsDefensiveActionMove()
	{
		return m_isDefensiveActionMove;
	}

	/// <summary>
	/// ���̃{�^����ҋ@���Ă����Ԃ��B
	/// </summary>
	/// <returns>�ł���Ȃ�true</returns>
	bool IsStandbyPeriod()
	{
		return m_isStandbyPeriod;
	}
	/// <summary>
	/// ���̍s���̑ҋ@��Ԃ��t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetStandbyPeriodFlag(bool flag)
	{
		m_isStandbyPeriod = flag;
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
	/// ����A�h��q�b�g�ɓ������Ƃ��̍ŏ��̏���
	/// </summary>
	virtual void EntryDefensiveHitProcess() {}
	/// <summary>
	/// ����A�h��q�b�g���̍X�V����
	/// </summary>
	virtual void UpdateDefensiveHitProcess() {}
	/// <summary>
	/// ����A�h��q�b�g�𔲂��o�����̏���
	/// </summary>
	virtual void ExitDefensiveHitProcess() {}


	/// <summary>
	/// ����A�h��A�N�V�������s���邩
	/// </summary>
	/// <returns>�s����Ȃ�true</returns>
	virtual bool CanDefensiveAction() { return false; }
	/// <summary>
	/// �X�L���U�����s���邩
	/// </summary>
	/// <returns>�s����Ȃ�true</returns>
	virtual bool CanSkillAttack() { return false; }


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


	/// <summary>
	/// �U���̏u�Ԃ̏���
	/// </summary>
	/// <param name="startOrEnd">start��true</param>
	virtual void AttackImpactProcess(bool startOrEnd);


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

	/// <summary>
	/// �R���{�X�e�[�g��ԍ�(int)�ɕϊ�����
	/// </summary>
	/// <param name="comboState">�R���{�X�e�[�g</param>
	/// <returns>�ԍ�</returns>
	int ConvertComboStateToNumber(EnComboState comboState);


protected:

	Brave* m_brave = nullptr;		//�v���C���[�̃C���X�^���X

	WeaponStatus m_status;			//�X�e�[�^�X

	EnWeaponState m_enWeaponState = enStowed;			//����̏�ԁB�f�t�H���g�͎��[���


	Vector3 m_stowedPosition = { 0.0f,-200.0f,0.0f };		//���[��Ԏ��̍��W


	EnComboState m_enComboState = enCombo_None;


	//�U���n�t���O
	bool m_isAttackActionMove = false;			//�U���ňړ����邩
	bool m_isStandbyPeriod = false;	//�ʏ�U�����͑ҋ@��ԃt���O

	//����A�h��n�t���O
	bool m_isDefensiveActionMove = false;		//����ňړ����邩

	bool m_isImpossibleancelAction = false;          //�A�N�V�����̃L�����Z���s�\���t���O�B�s���㏑���Ɏg��
													//��{�I�ɓ����蔻�肪�o�Ă���Ƃ���U����������ł��낤�u��
													//�̓L�����Z���A�N�V�����ł��Ȃ�


	

};

