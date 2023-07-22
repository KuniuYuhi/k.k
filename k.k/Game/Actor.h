#pragma once
#include "Status.h"

//class Player;

class Actor:public IGameObject,public Noncopyable
{
public:

	Actor();
	virtual ~Actor();

	/// <summary>
	/// ���f���̍��W�A��]�A�T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="modelRender"></param>
	void SetTransFormModel(ModelRender& modelRender);

	/// <summary>
	/// ���W�A��]�A�T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	void SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale=Vector3::One);

	/// <summary>
	/// �A�j���[�V�����X�e�[�g��ݒ肷��
	/// </summary>
	virtual void SetAnimationState() = 0;

	/// <summary>
	/// �L�����R���̍��W�ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetCharaConPosition(Vector3 position)
	{
		//m_charaCon.SetPosition(position);
	}

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="attack">����̍U����</param>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// �L�����N�^�[���`�F���W�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationSwappable() const = 0;

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;


	//�G�̃_���[�W����p�R���{�X�e�[�g
	enum EnComboState
	{
		enNowCombo_None,
		enDamageCombo_None,
		enNowCombo_1,
		enNowCombo_2,
		enNowCombo_3
	};

	/// <summary>
	/// ���݂̃R���{�X�e�[�g�ƃ_���[�W���󂯂����̃R���{�X�e�[�g���Ⴄ��
	/// </summary>
	/// <returns></returns>
	bool IsComboStateSame();

	/// <summary>
	/// ���݂̃R���{��Ԃ�ݒ�
	/// </summary>
	/// <param name="nowcombostate"></param>
	void SetNowComboState(EnComboState nowcombostate)
	{
		m_enNowComboState = nowcombostate;
	}
	/// <summary>
	/// �_���[�W���󂯂����̃R���{��Ԃ�ݒ�B
	/// �_���[�W���󂯂��L�������ݒ肷��B
	/// �܂��̓A�j���[�V�������I�������ݒ肷��B
	/// </summary>
	/// <param name="oldcombostate"></param>
	void SetDamagedComboState(EnComboState damagedcombostate)
	{
		m_enDamagedComboState = damagedcombostate;
	}

	/// <summary>
	///	���݂̃R���{��Ԃ��擾
	/// </summary>
	/// <returns></returns>
	EnComboState GetNowComboState() const
	{
		return m_enNowComboState;
	}
	/// <summary>
	/// �_���[�W���󂯂����̃R���{��Ԃ��擾
	/// </summary>
	/// <returns></returns>
	EnComboState GetDamagedComboState() const
	{
		return m_enDamagedComboState;
	}

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// �g�嗦�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// �X�L���̍U���͂̎擾
	/// </summary>
	/// <returns></returns>
	const int GetSkillAttackPower() const
	{
		return m_skillAttackPower;
	}

	/// <summary>
	/// ���g�����ꂽ���̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_dieFlag = flag;
	}
	/// <summary>
	/// ���g�����ꂽ���̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool GetDieFlag() const
	{
		return m_dieFlag;
	}

	/// <summary>
	/// ���g������ăL�����N�^�[��؂�ւ��邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDieToChangeFlag(bool flag)
	{
		m_dieToChangeFlag = flag;
	}
	/// <summary>
	/// ���g������ăL�����N�^�[��؂�ւ��邩�̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool GetDieToChangeFlag() const
	{
		return m_dieToChangeFlag;
	}

protected:

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() = 0;

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// �X�L���̎g�p���Ȃǂ̈ړ��͂��Ȃ�����]�͂������Ƃ��Ɏg��
	/// </summary>
	virtual bool RotationOnly() = 0;

	/// <summary>
	/// MP�̉�
	/// m_recoveryMpFlag��true�̎��ɏ��������s
	/// </summary>
	void RecoveryMP();

	/// <summary>
	/// MP���񕜂��邩�̃t���O��ݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetRecoveryMpFlag(bool flag)
	{
		m_recoveryMpFlag = flag;
	}

	/// <summary>
	/// �ړ����̉�]
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation();

	

protected:

	Status m_status;
	//Player* m_player = nullptr;

	
	EnComboState m_enNowComboState = enNowCombo_None;		//���݂̃R���{
	EnComboState m_enDamagedComboState = enDamageCombo_None;		//�_���[�W���󂯂����̃R���{

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//��]�̂ݎg�p���鎞�Ɏg��

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	int m_skillAttackPower = 0;

	bool m_dashFlag = false;		//�_�b�V�����邩�̃t���O

	//enum�ɂ��邩��
	bool m_dieFlag = false;			//���ꂽ��true�ɂ���
	bool m_dieToChangeFlag = false;		//����Ă���L�����N�^�[�؂�ւ��Ɉڂ邽�߂̃t���O

	bool m_recoveryMpFlag = false;	//�X�L����ł��I��������Ƃ�true�ɂ���B�łO��false

	bool m_createAttackCollisionFlag = false;		//�U�����ɓ����蔻��𐶐����邩�̃t���O
};

