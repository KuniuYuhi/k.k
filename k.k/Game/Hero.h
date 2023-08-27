#pragma once
#include "Actor.h"
class IHeroState;
class Player;

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

	/// <summary>
	/// �X�L���g�p���̓����蔻��̍쐬
	/// </summary>
	void CreateSkillCollision();

	void Damage(int attack);

	void PowerUpTimer();

	bool CalcDash();

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
			m_enAnimationState != enAninationState_Walk /*&&
			m_enAnimationState != enAninationState_Dash*/;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����Ȃ�ړ��ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Attack_Skill_Charge&&
			m_enAnimationState != enAnimationState_PowerUp &&
			m_enAnimationState != enAnimationState_Damage&&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ����̂Ƃ������蔻������Ȃ�
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_PowerUp &&
			m_enAnimationState != enAnimationState_Die;
	}

	bool isRotationEntable() const
	{
		return m_enAnimationState != enAninationState_Dash &&
			m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_Skill_Charge;
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
	/// �_�b�V���̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDashStateTransition();
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
	/// �A�^�b�N�X�L���`���[�W�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_Skill_ChargeStateTransition();
	/// <summary>
	/// �A�^�b�N�X�L�����C���̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_Skill_MainStateTransition();
	/// <summary>
	/// �|���ꂽ�Ƃ��̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDieStateTransition();
	/// <summary>
	/// �_���[�W���󂯂����̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessDamageStateTransition();
	/// <summary>
	/// �p���[�A�b�v�̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessPowerUpStateTransition();
	/// <summary>
	/// �����̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessVictoryStateTransition();

public:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,	// 1 : �����A�j���[�V����
		enAnimClip_Dash,		// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,// 3 : 
		enAnimClip_Attack_2,// 4 : 
		enAnimClip_Attack_3,// 5 : 
		enAnimClip_Attack_Skill_Charge,
		enAnimClip_Attack_Skill_Main,// 6 : 
		enAnimClip_PowerUp,
		enAnimClip_Victory,
		enAnimClip_Die,
		enAnimClip_Damage,
		enAnimClip_Num,		// 7 :�A�j���[�V�����N���b�v�̐�
	};

private:
	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAninationState_Dash,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Attack_Skill_Charge,
		enAnimationState_Attack_Skill_Main,
		enAnimationState_PowerUp,
		enAnimationState_Victory,
		enAnimationState_Die,
		enAnimationState_Damage
	};

	//�U���p�^�[���X�e�[�g
	enum EnAttackPattern {
		enAttackPattern_None,
		enAttackPattern_1,
		enAttackPattern_1to2,
		enAttackPattern_2,
		enAttackPattern_2to3,
		enAttackPattern_3,
		enAttackPattern_Skill_Charge,
		enAttackPattern_Skill_Main,
		enAttackPattern_Skill_PowerUp,
		enAttackPattern_End
	};

	/// <summary>
	/// �����X�e�[�g��ݒ肷��
	/// </summary>
	void SetVictoryAnimationState()
	{
		SetNextAnimationState(enAnimationState_Victory);
	}

private:
	
	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	bool RotationOnly();



	ModelRender m_modelRender;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnAttackPattern m_enAttackPatternState = enAttackPattern_None;	//���݂̍U���^�C�~���O��m�邽�߂̃X�e�[�g

	Player* m_player = nullptr;

	IHeroState* m_state = nullptr;					//�X�e�[�g�N���X

	const int m_skillMp = 30;	//�X�L�������ɕK�v��MP
	const int m_skillPowerUpMp = 80;		//�p���[�A�b�v�ɕK�v��

	bool m_powerUpTimeFlag = false;
	const float m_powerUpTime = 15.0f;				//�p���[�A�b�v���鎞��
	float m_powerUpTimer = 0.0f;					//�p���[�A�b�v�̎��Ԃ��v��^�C�}�[
	const int m_powerUpPower = 10;					//�p���[�A�b�v�������̏オ��U����

	int m_swordBoonId = -1;		//���̃{�[��ID�擾�p�ϐ�
	int m_skillBoonId = -1;		//�X�L���g�p���̃{�[���擾�p�ϐ�

	bool m_createSkillCollisionFlag = false;		//�X�L���g�p���ɓ����蔻��𐶐����邩�̃t���O
	float m_ChargeTimer = 0.0f;
	const float m_MaxChargeTime = 3.0f;				//�X�L���P�̗��߂̍ő厞��


	float m_dashTimer = 1.0f;

};

