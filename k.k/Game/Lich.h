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
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_end &&
			m_enAnimationState != enAninationState_Summon;
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
		enAnimClip_Attack_DarkMeteorite_end,
		enAnimClip_Summon,
		enAnimClip_Damage,
		enAnimClip_Die,
		enAnimClip_Victory,
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
	/// <summary>
	/// 
	/// </summary>
	void OnProcessDarkMeteorite_EndStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void OnProcessSummonStateTransition();
	/// <summary>
	/// �����J�ڏ��������s
	/// </summary>
	void OnProcessVictoryStateTransition();

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
		enAnimationState_Attack_DarkMeteorite_end,
		enAninationState_Summon,
		enAnimationState_Damage,
		enAnimationState_Die,
		enAnimationState_Victory
	};

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	//
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,
		enSpecialActionState_Warp,
		enSpecialActionState_CenterWarp,
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

	/// <summary>
	/// �^�[�Q�b�g�����ԉ����Ƃ���ɍ��W���ړ�������
	/// </summary>
	void Warp(EnSpecialActionState SpecialActionState);


	/// <summary>
	/// ���G��ԃt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetInvincibleFlag(bool flag)
	{
		m_invincibleFlag = flag;
	}
	/// <summary>
	/// ���G��ԃt���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetInvincibleFlag()
	{
		return m_invincibleFlag;
	}

	/// <summary>
	/// �q�b�g�J�E���g�ƒ~�σ_���[�W��ݒ�
	/// </summary>
	/// <param name="count">�J�E���g</param>
	/// <param name="damage">�󂯂��_���[�W</param>
	void SetHitCountAndDamage(int count, int damage)
	{
		AddHitCount(count);
		AddAccumulationDamage(damage);
	}

	/// <summary>
	/// �q�b�g�J�E���g��ݒ�
	/// </summary>
	/// <param name="count"></param>
	void AddHitCount(int count)
	{
		m_hitCount += count;
	}

	/// <summary>
	/// �q�b�g�J�E���g���擾
	/// </summary>
	/// <returns></returns>
	const int& GetHitCount() const
	{
		return m_hitCount;
	}

	/// <summary>
	/// �~�σ_���[�W��ݒ�
	/// </summary>
	/// <param name="count"></param>
	void AddAccumulationDamage(int damage)
	{
		m_accumulationDamage += damage;
	}

	/// <summary>
	/// �~�σ_���[�W���擾
	/// </summary>
	/// <returns></returns>
	const int& GetAccumulationDamage() const
	{
		return m_accumulationDamage;
	}


	/// <summary>
	/// AIActor(�����X�^�[)�̏������X�g�ɒǉ�
	/// </summary>
	/// <param name="monster"></param>
	void AddAIActorFromList(AIActor* monster)
	{
		m_monsters.emplace_back(monster);
	}
	/// <summary>
	/// AIActor(�����X�^�[)�̏������X�g����폜����
	/// </summary>
	/// <param name="monster"></param>
	void RemoveAIActorFromList(AIActor* monster)
	{
		std::vector<AIActor*>::iterator it = std::find(
			m_monsters.begin(), // �A�N�^�[�̃��X�g�̍ŏ�
			m_monsters.end(),   // �A�N�^�[�̃��X�g�̍Ō�
			monster                     // ���������A�N�^�[
		);
		if (it != m_monsters.end()) {
			m_monsters.erase(it);
		}
	}


private:

	bool RotationOnly();


	Level3DRender m_stageLevel;
	std::vector<Vector3> m_WarpPosition;
	
	LichAction* m_lichAction = nullptr;
	ILichState* m_state = nullptr;
	DarkMeteorite* m_darkMeteorite = nullptr;

	std::vector<AIActor*> m_monsters;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnSpecialActionState m_enSpecialActionState = enSpecialActionState_Normal;

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	FontRender m_hpFont;

	//��_���[�W���ɃJ�E���g�𑝂₷
	int m_hitCount = 0;
	//��_���[�W���Ɏ󂯂��_���[�W�����₷
	int m_accumulationDamage = 0;

	const float m_distanceToPlayer = 300.0f;

	int m_darkWallBoonId = -1;					//�_�[�N�E�H�[���Ŏg���{�[��ID

	bool m_attackRangeFlag = false;				//�U���͈͂ɂ��邩�̃t���O

	bool m_CreateDarkWallFlag = false;			//�_�[�N�E�H�[�������t���O

	const float m_attackIntervalTime = 4.0f;
	const float m_damageIntervalTime = 0.5f;
	
	InfoAboutAttack m_InfoAboutAttack;


	bool m_halfHpFlag = false;

	bool m_dieFlag = false;
	bool m_winFlag = false;

	bool m_invincibleFlag = false;


	const float m_RisingLimit = 200.0f;

	bool m_createDarkMeteoriteFlag = false;

};

