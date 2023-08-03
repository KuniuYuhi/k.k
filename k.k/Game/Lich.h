#pragma once
#include "AIActor.h"
#include "Level3DRender.h"

class ILichState;
class LichAction;
class DarkMeteorite;

class Lich:public AIActor
{
public:
	Lich();
	~Lich();

	/// <summary>
	/// �U���Ɋւ�����
	/// </summary>
	struct InfoAboutAttack
	{
		const float m_Attack_1Distance = 600.0f;	//�������U��
		const float m_Attack_2Distance = 200.0f;	//�ߋ����U��
	};

	bool Start();
	void Update();

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Render(RenderContext& rc);

	void SetStageLevelPosition();

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

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	/// <param name="attack">���炤�U����</param>
	void Damage(int attack);

	/// <summary>
	/// ��_���[�W�p�����蔻��
	/// </summary>
	void DamageCollision(CharacterController& characon)override;

	/// <summary>
	/// �_�[�N�E�H�[���̐���
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// �_�[�N���e�I�̐���
	/// </summary>
	void CreateDarkMeteorite();

	bool IsRisingDarkMeteorite();

	/// <summary>
	/// ���̍s��������
	/// </summary>
	void DecideNextAction();

	/// <summary>
	/// ���ނ��ǂ���
	/// </summary>
	/// <returns></returns>
	bool Isflinch();

	bool IsDistanceToPlayer();

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// �U���A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2&&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main;
	}

	/// <summary>
	/// ��]�\���ǂ���
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAninationState_Idle&&
			m_enAnimationState!=enAnimationState_Attack_DarkMeteorite_start&&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main;
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
	/// �_�[�N�E�H�[���̃{�[��Id���擾
	/// </summary>
	/// <returns></returns>
	const int& GetDarkWallBoonId() const
	{
		return m_darkWallBoonId;
	}

	const InfoAboutAttack& GetInfoAboutAttack()
	{
		return m_InfoAboutAttack;
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
		enAnimClip_Attack_DarkMeteorite_start,
		enAnimClip_Attack_DarkMeteorite_main,
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
	/// <summary>
	/// ��_���[�W�J�ڏ��������s
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_StartStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_MainStateTransition();

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
		enAnimationState_Attack_DarkMeteorite_start,
		enAnimationState_Attack_DarkMeteorite_main,
		enAnimationState_Damage,
		enAnimationState_Die
	};

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	/// <summary>
	/// �^�[�Q�b�g�����ԉ����Ƃ���ɍ��W���ړ�������
	/// </summary>
	void Warp();

	//
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,
		enSpecialActionState_Warp,
		SpecialActionState
	};

	void SetSpecialActionState(EnSpecialActionState SpecialActionState)
	{
		m_enSpecialActionState = SpecialActionState;
	}

	const EnSpecialActionState& GetSpecialActionState() const
	{
		return m_enSpecialActionState;
	}


private:

	bool RotationOnly();


	Level3DRender m_stageLevel;
	std::vector<Vector3> m_WarpPosition;
	
	LichAction* m_lichAction = nullptr;
	ILichState* m_state = nullptr;
	DarkMeteorite* m_darkMeteorite = nullptr;



	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnSpecialActionState m_enSpecialActionState = enSpecialActionState_Normal;

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	FontRender m_hpFont;

	const float m_distanceToPlayer = 300.0f;

	int m_darkWallBoonId = -1;					//�_�[�N�E�H�[���Ŏg���{�[��ID

	bool m_attackRangeFlag = false;				//�U���͈͂ɂ��邩�̃t���O

	bool m_CreateDarkWallFlag = false;			//�_�[�N�E�H�[�������t���O

	const float m_attackIntervalTime = 4.0f;
	const float m_damageIntervalTime = 0.5f;
	
	InfoAboutAttack m_InfoAboutAttack;

	bool m_halfHpFlag = false;

	bool m_dieFlag = false;


	const float m_RisingLimit = 200.0f;

	bool m_createDarkMeteoriteFlag = false;

};

