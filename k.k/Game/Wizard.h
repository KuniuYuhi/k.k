#pragma once
#include "Actor.h"

class Player;
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

	void CreateCollision();

	void CreateFlamePillar();

	bool CreateFireBall();

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

	void OnProcessAttack_2MainStateTransition();

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
		enAttackPattern_1to4,
		enAttackPattern_3_start,
		enAttackPattern_3_main,
		enAttackPattern_4,
		enAttackPattern_End
	};

	//�X�L���p�^�[���X�e�[�g
	enum EnSkillPattern {
		enSkillPattern_FlamePillar,
		enSkillPattern_FireBall
	};

public:
	//�A�j���[�V�����X�e�[�g��ݒ肷��
	void SetAnimationState()
	{
		//m_enAnimationState= enAninationState_Idle;
	}

private:
	
	bool IsComboStateSame();

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g�����߂�
	/// </summary>
	/// <param name="nextState">���̃A�j���[�V�����X�e�[�g</param>
	void SetNextAnimationState(EnAnimationState nextState);

	bool RotationOnly();
	
	Player* m_player = nullptr;

	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;

	EnSkillPattern m_enSkillPatternState = enSkillPattern_FlamePillar;

	IWizardState* m_animationState = nullptr;

	Quaternion m_wandRotation = Quaternion::Identity;


	bool m_createAttackCollisionFlag = false;		//�U�����ɓ����蔻��𐶐����邩�̃t���O

	int m_magicWandBoonId = -1;


	const int m_flamePillar_skillMp = 50;
	const int m_fireBall_SkillMp = 10;


	bool m_createFireBallFlag = false;
	
	const float m_createFireBallTime = 0.7f;
	float m_fireBallTimer = 0.0f;

};

