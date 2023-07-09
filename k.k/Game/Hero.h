#pragma once
#include "Actor.h"
class IHeroState;
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

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();

	/// <summary>
	/// �U�����̓����蔻��̍쐬
	/// </summary>
	void CreateCollision();


	bool GetAtkCollsionCreateFlag() const
	{
		return m_createAttackCollisionFlag;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ����ւ��\�B?
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
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 /*&&
			m_enAnimationState != enAnimationState_Attack_4*/;
	}

	/// <summary>
	/// ���f�������_�[�̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �A�^�b�N�P�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// �A�^�b�N2�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// �A�^�b�N3�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_3StateTransition();
	/// <summary>
	/// �A�^�b�N4�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_4StateTransition();

public:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,	// 1 : �����A�j���[�V����
		enAnimClip_Run,		// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,// 3 : 
		enAnimClip_Attack_2,// 4 : 
		enAnimClip_Attack_3,// 5 : 
		enAnimClip_Attack_4,// 6 : 
		enAnimClip_Num,		// 7 :�A�j���[�V�����N���b�v�̐�
	};

private:
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Attack_4
	};

	//�U���p�^�[���X�e�[�g
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_2,
		enAttackPattern_3,
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

	
	
	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);


	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;

	IHeroState* m_state = nullptr;					//�X�e�[�g�N���X


	int m_swordBoonId = -1;		//���̃{�[��ID�擾�p�ϐ�
	bool m_createAttackCollisionFlag = false;		//�U�����ɓ����蔻��𐶐����邩�̃t���O

};

