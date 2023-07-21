#pragma once
#include "AIActor.h"
class ILichState;
class Lich:public AIActor
{
public:
	Lich();
	~Lich();

	bool Start();
	void Update();

	void Render(RenderContext& rc);

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation();

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	void Damage();

	/// <summary>
	/// ��_���[�W�p�����蔻��
	/// </summary>
	void DamageCollision();

	/// <summary>
	/// �A�^�b�N�P�̍U������
	/// </summary>
	bool Attack();
	/// <summary>
	/// �A�^�b�N�Q�̍U������
	/// </summary>
	/// <returns></returns>
	bool Attack2();

	/// <summary>
	/// ���̍s��������
	/// </summary>
	void DecideNextAction();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1&&
			m_enAnimationState != enAnimationState_Attack_2;
	}

	/// <summary>
	/// ��]�\���ǂ���
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return /*m_enAnimationState != enAnimationState_Attack_1 &&*/
			m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// ���f�������_�[�̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}


	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,			// 1 : �����A�j���[�V����
		enAnimClip_Run,				// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2,		// 4 : 
		enAnimClip_Attack_3_start,		// 5 : 
		enAnimClip_Attack_3_main,
		enAnimClip_Attack_4,		// 6 : 
		enAnimClip_Damage,
		enAnimClip_Die,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �A�^�b�N�P�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// �A�^�b�N2�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// Die�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDieStateTransition();

private:

	bool RotationOnly();

	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3_start,
		enAnimationState_Attack_3_main,
		enAnimationState_Attack_4,
		enAnimationState_Damage,
		enAnimationState_Die
	};

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);


	ILichState* m_state = nullptr;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	FontRender m_hpFont;

	bool m_attackRangeFlag = false;				//�U���͈͂ɂ��邩�̃t���O

	const float m_attackIntervalTime = 3.0f;
	const float m_damageIntervalTime = 0.5f;

	const float m_Attack_1Distance = 600.0f;

	const float m_Attack_2Distance = 200.0f;

	bool m_dieFlag = false;

};

