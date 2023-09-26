#pragma once
#include "MobMonster.h"

class Lich;
class ITurtleShellState;

class TurtleShell:public MobMonster
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Attack();

	bool Difence();

	/// <summary>
	/// ���̍s�������߂�
	/// </summary>
	void DecideNextAction();

	void CreateCollision();

	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	/// <summary>
	/// �h��^�C�}�[�̏���
	/// </summary>
	bool IsDifenceTime();

	/// <summary>
	/// ���f�������_�[�̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{

		return //m_enAnimationState != enAnimationState_Difence &&
			m_enAnimationState != enAnimationState_DifenceDamage &&
			m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAnimationState_Difence &&
			m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return /*m_enAnimationState != enAnimationState_Difence &&*/
			m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2;
	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

	bool RotationOnly();

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,			// 1 : �����A�j���[�V����
		enAnimClip_Run,				// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2,
		enAnimClip_Difence,
		enAnimClip_DefenceDamage,
		enAnimClip_Damage,
		enAnimClip_Die,
		enAnimClip_Victory,
		enAnimClip_Appear,
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
	/// �A�^�b�N�Q�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_2StateTransition();
	/// <summary>
	/// �h��X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDifenceStateTransition();
	/// <summary>
	/// �h���_���[�W�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDifenceDamageStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// �f�X�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// �����X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessVictoryStateTransition();
	/// <summary>
	/// �������ꂽ���̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAppearStateTransition();

	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Run,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Difence,
		enAnimationState_DifenceDamage,
		enAnimationState_Damage,
		enAnimationState_Die,
		enAnimationState_Victory,
		enAnimationState_Appear
	};

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	enum EnDefenceState
	{
		enDefenceState_None,
		enDefenceState_damaged,
		enDefenceState_Defence,
		enDefenceState_DefenceDamaged
	};

private:
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


	//Lich* m_lich = nullptr;
	ITurtleShellState* m_state = nullptr;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	//float m_angleChangeTime = 0.0f;		//�x�N�g�����v�Z����^�C�}�[�B�������Ɍ��߂�

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	//Vector3 m_direction = Vector3::Zero;

	//const float m_attackIntervalTime = 2.0f;	//�U��������̃C���^�[�o��

	//const float m_distanceToPlayer = 400.0f;
	//const float m_attackRange = 60.0f;
	//const float m_stayDistance = 50.0f;

	int m_attackBoonId = -1;					//�U���Ŏg���{�[��ID

	bool m_createAttackCollisionFlag = false;


	bool m_damagedFlag = false;			//���g���U�����󂯂����̃t���O
	bool m_difenceFlag = false;			//�h�䂵�Ă��邩�̃t���O
	const float m_difenceTime = 3.0f;
	float m_difenceTimer = 0.0f;

	EnDefenceState m_defenceState = enDefenceState_None;

};

