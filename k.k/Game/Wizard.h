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

	void Damage(int attack);

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
			m_enAnimationState != enAnimationState_Attack_4&&
			m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����̂Ƃ������蔻������Ȃ�
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	bool isRotationEntable() const
	{
		return true;
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

	void OnProcessDieStateTransition();

	void OnProcessDamageStateTransition();

	void OnProcessVictoryStateTransition();


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
		enAnimClip_Die,
		enAnimClip_Damage,
		enAnimClip_Victory_start,
		enAnimClip_Victory_main,
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
		enAnimationState_Attack_4,
		enAnimationState_Die,
		enAnimationState_Damage,
		enAnimationState_Victory_start,
		enAnimationState_Victory_main,
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

	
public:
	
	/// <summary>
	/// �����X�e�[�g��ݒ肷��
	/// </summary>
	void SetVictoryAnimationState()
	{
		SetNextAnimationState(enAnimationState_Victory_start);
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

