#pragma once
#include "Actor.h"

class IWizardState;
class Wizard :public Actor
{
public:
	Wizard();
	~Wizard();

	bool Start();

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();

	void Update();
	void Render(RenderContext& rc);

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Move();

	void Attack();

	void PlayAnimation();

	void ManageState();

	/// <summary>
	/// ����̃A�j���[�V�������Đ���(true)�Ȃ����ւ��\�B
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return m_enAnimationState != enAninationState_Idle &&
			m_enAnimationState != enAninationState_Walk &&
			m_enAnimationState != enAninationState_Run;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�ړ��ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2_start &&
			m_enAnimationState != enAnimationState_Attack_2_main &&
			m_enAnimationState != enAnimationState_Attack_3_start &&
			m_enAnimationState != enAnimationState_Attack_3_main &&
			m_enAnimationState != enAnimationState_Attack_4;
	}

	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();

	void OnProcessAttack_1StateTransition();

	void OnProcessAttack_2StateTransition();

	void OnProcessAttack_3StateTransition();

	void OnProcessAttack_4StateTransition();


	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,			// 1 : �����A�j���[�V����
		enAnimClip_Run,				// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2_start,		// 4 : 
		enAnimClip_Attack_2_main,
		enAnimClip_Attack_3_start,		// 5 : 
		enAnimClip_Attack_3_main,
		enAnimClip_Attack_4,		// 6 : 
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};

	

private:
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2_start,
		enAnimationState_Attack_2_main,
		enAnimationState_Attack_3_start,
		enAnimationState_Attack_3_main,
		enAnimationState_Attack_4
	};

	//�U���p�^�[���X�e�[�g
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_2_start,
		enAttackPattern_2_main,
		enAttackPattern_3_start,
		enAttackPattern_3_main,
		enAttackPattern_4,
		enAttackPattern_End
	};

public:
	//�A�j���[�V�����X�e�[�g��ݒ肷��
	void SetAnimationState()
	{
		//m_enAnimationState= enAninationState_Idle;
	}

private:
	
	
	void SetNextAnimationState(EnAnimationState nextState);


	

	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;

	IWizardState* m_animationState = nullptr;
};

