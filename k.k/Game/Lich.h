#pragma once
#include "AIActor.h"
#include "Level3DRender.h"

class ILichState;
class LichAction;
class DarkMeteorite;
class Summon;
class DarkBall;
class DarkWall;

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
		const float m_Attack_1Distance = 1200.0f;	//�������U��
		const float m_Attack_2Distance = 200.0f;	//�ߋ����U��
		const float m_Attack_DarkMeteoDistance = 1200.0f;	//�_�[�N���e�I
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
	/// �����蔻��̏��������邩
	/// </summary>
	bool IsCollisionDetection();

	/// <summary>
	/// �ʏ�U���ɓ����������̏���
	/// </summary>
	void HitNormalAttack();
	
	/// <summary>
	/// �q�[���[�̃X�L���ɓ����������̏���
	/// </summary>
	void HitHeroSkillAttack();

	/// <summary>
	/// �E�B�U�[�h�̃t���C���s���[�ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	void HitFlamePillar(bool damageFlag = false);

	/// <summary>
	/// �_�[�N�E�H�[���̐���
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// �_�[�N�{�[���̐���
	/// </summary>
	/// <param name="AddBallFlag">�_�[�N�{�[���𕡐��������邩�̃t���O</param>
	void CreateDarkBall(bool AddBallFlag = false);

	/// <summary>
	/// �_�[�N�{�[���̒ǉ��̐���
	/// </summary>
	/// <param name="darkBall">���݂̉�]����Y���̉��Z����l</param>
	/// <param name="degY">�_�[�N�{�[���̃I�u�W�F�N�g��</param>
	void AddCreateDarkBall(DarkBall* darkBall, const char* name,float degY);

	/// <summary>
	/// �_�[�N���e�I�̐���
	/// </summary>
	/// <param name="lastMeteoFlag">�_�[�N���e�I�̖{�̂������̃t���O</param>
	void CreateDarkMeteorite(bool lastMeteoFlag = false);

	/// <summary>
	/// �_�[�N���e�I�̍폜�B(�_�[�N���e�I�������Ƀv���C���[������邩������Ȃ�����)
	/// </summary>
	void DeleteDarkMeteo();

	/// <summary>
	/// �����N���X�̐���
	/// </summary>
	void CreateSummon();

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

	void CreateDamageFont(int damage);

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Angry &&
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
			m_enAnimationState != enAninationState_Summon &&
			m_enAnimationState != enAnimationState_Warp;
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
		enAnimClip_Angry,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �A�^�b�N�P(�ߋ���)�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_1StateTransition();
	/// <summary>
	/// �A�^�b�N2(������)�X�e�[�g�J�ڏ��������s
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
	/// �_�[�N���e�I(�X�^�[�g)�J�ڏ��������s
	/// </summary>
	void OnProcessDarkMeteorite_StartStateTransition();
	/// <summary>
	/// �_�[�N���e�I(���C��)�J�ڏ��������s
	/// </summary>
	void OnProcessDarkMeteorite_MainStateTransition();
	/// <summary>
	/// �_�[�N���e�I(�G���h)�J�ڏ��������s
	/// </summary>
	void OnProcessDarkMeteorite_EndStateTransition();
	/// <summary>
	/// �����J�ڏ��������s
	/// </summary>
	void OnProcessSummonStateTransition();
	/// <summary>
	/// �����J�ڏ��������s
	/// </summary>
	void OnProcessVictoryStateTransition();
	/// <summary>
	/// �{�胂�[�h�J�ڏ��������s
	/// </summary>
	void OnProcessAngryStateTransition();
	/// <summary>
	/// ���[�v�J�ڏ��������s
	/// </summary>
	void OnProcessWarpStateTransition();

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
		enAnimationState_Victory,
		enAnimationState_Angry,
		enAnimationState_Warp
	};

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	//
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,			//�ʏ�B�ړ����Ȃ�
		enSpecialActionState_AngryMode,			//�{�胂�[�h�B���̎������ړ�����
		SpecialActionState_Num
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
	/// ���[�v��̍��W�����߂�
	/// </summary>
	void DecideWarpPosition();

	/// <summary>
	/// ���[�v��̍��W�Ɉړ�����
	/// </summary>
	void MoveWarpPosition();


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

	/// <summary>
	/// ��������Ă��郂���X�^�[�̐����擾
	/// </summary>
	/// <returns></returns>
	const int& GetAIActors() const
	{
		return m_monsters.size();
	}

	std::vector<AIActor*>& GetMonsters()
	{
		return m_monsters;
	}

	/// <summary>
	/// �{�胂�[�h���Ԃ̌v�Z
	/// </summary>
	/// <returns></returns>
	bool CalcAngryTime();
	/// <summary>
	/// �{�胂�[�h�J�E���g�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetAngryModeCount() const
	{
		return m_angryModeCount;
	}

	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	/// <summary>
	/// �^�C���A�b�v�ŏI��������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetTimeUpEndFlag() const
	{
		return m_timeUpEndFlag;
	}

	/// <summary>
	/// ���S�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_dieFlag = flag;
	}
	/// <summary>
	/// ���S�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetDieFlag() const
	{
		return m_dieFlag;
	}

private:

	bool RotationOnly();

	enum EnWarpStep
	{
		enWarpStep_Up,
		enWarpStep_Warp,
		enWarpStep_Down,
		enWarpStep_End
	};

	void OnProcessenWarpStepUp();
	void OnProcessenWarpStepWarp();
	void OnProcessenWarpSteDown();
	void OnProcessenWarpStepEnd();

	std::vector<Vector3>			m_warpPositions;										//���[�v��̍��W�̃��X�g
	std::vector<AIActor*>			m_monsters;												//�������������X�^�[�̏����i�[���郊�X�g

	EnAnimationState				m_enAnimationState = enAninationState_Idle;				//�A�j���[�V�����X�e�[�g
	EnSpecialActionState			m_enSpecialActionState = enSpecialActionState_Normal;	//���ʂȏ�ԃX�e�[�g(�ʏ�A�{�胂�[�h)
	EnWarpStep						m_enWarpStep = enWarpStep_Up;							//���[�v�����X�e�[�g
	
	Game*							m_game = nullptr;
	LichAction*						m_lichAction = nullptr;
	ILichState*						m_state = nullptr;
	DarkMeteorite*					m_darkMeteorite = nullptr;
	Summon*							m_summon = nullptr;
	DarkBall*						m_darkBall_light = nullptr;
	DarkBall*						m_darkBall_left = nullptr;
	DarkWall*						m_darkWall = nullptr;

	Level3DRender					m_stageLevel;
	Animation						m_animation;											// �A�j���[�V����
	AnimationClip					m_animationClip[enAnimClip_Num];						// �A�j���[�V�����N���b�v 
	
	ModelRender						m_modelRender;											//���f�������_�[
	CharacterController				m_charaCon;												//�L�����N�^�[�R���g���[���[

	Vector3							m_warpPosition = g_vec3Zero;							//���[�v�̍��W
	Vector3							m_toPlayerDir = g_vec3Zero;								//�v���C���[�Ɍ������x�N�g��

	
	int								m_hitCount = 0;											//��_���[�W���ɃJ�E���g�𑝂₷
	int								m_accumulationDamage = 0;								//��_���[�W���Ɏ󂯂��_���[�W�����₷

	const float						m_distanceToPlayer = 300.0f;							//�v���C���[�Ƃ̋���

	int								m_darkWallBoonId = -1;									//�_�[�N�E�H�[���Ŏg���{�[��ID

	bool							m_attackRangeFlag = false;								//�U���͈͂ɂ��邩�̃t���O

	float							m_attackIntervalTime = 4.0f;
	const float						m_damageIntervalTime = 0.5f;							//�_���[�W���󂯂�Ԋu
	
	InfoAboutAttack					m_InfoAboutAttack;										//�U���Ɋւ�����

	bool							m_halfHpFlag = false;									//�U�������U���I�ɂȂ�

	bool							m_dieFlag = false;										//���񂾃t���O
	
	bool							m_invincibleFlag = false;								//���G��ԃt���O
	
	bool							m_timeUpEndFlag = false;								//�^�C���A�b�v�ŃQ�[�����I��������̃t���O

	bool							m_createDarkMeteoriteFlag = false;						//�_�[�N���e�I�����t���O

	bool							m_firstSummonFlag = true;								//���߂Ă̏������̃t���O

	const float						m_angryLimitTime = 20.0f;
	float							m_angryLimitTimer = 0.0f;
	int								m_angryModeCount = 0;									//�{�胂�[�h�ɂȂ�w�W�ƂȂ�J�E���g


};

