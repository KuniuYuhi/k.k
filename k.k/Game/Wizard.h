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

	void Move();

	void PlayAnimation();

	void ManageState();

	/// <summary>
	/// ����̃A�j���[�V�������Đ���(true)�Ȃ����ւ��\�B
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return m_enAnimationState == enAninationState_Idle &&
			m_enAnimationState == enAninationState_Walk &&
			m_enAnimationState == enAninationState_Run;
	}

	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,	// 1 : �����A�j���[�V����
		enAnimClip_Run,		// 2 : ����A�j���[�V����
		enAnimClip_Num,		// 3 :�A�j���[�V�����N���b�v�̐�
	};

	

private:
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run
	};

public:
	//�A�j���[�V�����X�e�[�g��ݒ肷��
	void SetAnimationState()
	{
		m_enAnimationState= enAninationState_Idle;
	}

private:
	
	void OnProcessRunStateTransition();
	void SetNextAnimationState(EnAnimationState nextState);


	

	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	IWizardState* m_animationState = nullptr;
};

