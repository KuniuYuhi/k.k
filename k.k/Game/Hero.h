#pragma once
#include "Actor.h"

class Hero :public Actor
{
public:
	Hero();
	~Hero();

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
		return m_enAnimationState == enIdle &&
			m_enAnimationState == enWalk &&
			m_enAnimationState == enRun;
	}

private:
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enIdle,
		enWalk,
		enRun
	};

	void OnProcessCommonStateTransition();
	void OnProcessRunStateTransition();



	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,	// 1 : �����A�j���[�V����
		enAnimClip_Run,		// 2 : ����A�j���[�V����
		enAnimClip_Num,		// 3 :�A�j���[�V�����N���b�v�̐�
	};

	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enIdle;	//�A�j���[�V�����X�e�[�g
	
	
	

};

