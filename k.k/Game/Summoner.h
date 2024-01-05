#pragma once
#include "BossBase.h"
#include "Level3DRender.h"

class IBossStateMachine;
class ISummonerState;
class DarkWall;

class Summoner:public BossBase
{
public:
	Summoner();
	~Summoner();

	bool Start();
	void Update();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void Render(RenderContext& rc);

	/////////////////////////////////////////////////////////////////////
	//�\���́A�񋓌^�̐錾
	/////////////////////////////////////////////////////////////////////
	/// <summary>
	/// �U���Ɋւ�����
	/// </summary>
	struct InfoAboutAttack
	{
		const float m_Attack_1Distance = 1200.0f;	//�������U��
		const float m_Attack_2Distance = 200.0f;	//�ߋ����U��
		const float m_Attack_DarkMeteoDistance = 1200.0f;	//�_�[�N���e�I
	};
	/// <summary>
	/// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Walk,
		enAnimClip_DarkBall,
		enAnimClip_DarkWall,
		enAnimClip_DarkSpear_Start,
		enAnimClip_DarkSpear_Main,
		enAnimClip_DarkSpear_End,
		enAnimClip_NormalAttack_1,
		enAnimClip_NormalAttack_2,
		enAnimClip_NormalAttack_3,
		enAnimClip_Attack_DarkMeteorite_start,
		enAnimClip_Attack_DarkMeteorite_main,
		enAnimClip_Summon,
		enAnimClip_CriticalHit,
		enAnimClip_Die,
		enAnimClip_Victory,
		enAnimClip_Angry,
		enAnimClip_Warp,
		enAnimClip_Command,
		enAnimClip_KnockBack,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};
	/// <summary>
	/// �A�j���[�V�����X�e�[�g
	/// </summary>
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Walk,
		enAnimationState_DarkBall,
		enAnimationState_DarkWall,
		enAnimationState_DarkSpear_Start,
		enAnimationState_DarkSpear_Main,
		enAnimationState_DarkSpear_End,
		enAnimationState_NormalAttack_1,
		enAnimationState_NormalAttack_2,
		enAnimationState_NormalAttack_3,
		enAnimationState_Attack_DarkMeteorite_start,
		enAnimationState_Attack_DarkMeteorite_main,
		enAninationState_Summon,
		enAnimationState_CriticalHit,
		enAnimationState_Die,
		enAnimationState_Victory,
		enAnimationState_Angry,
		enAnimationState_Warp,
		enAnimationState_Command,
		enAnimationState_KnockBack,
		enAnimationState_Num,
	};
	/// <summary>
	/// �{�X�̏�ԃX�e�[�g
	/// </summary>
	enum EnSpecialActionState
	{
		enSpecialActionState_Normal,			//�ʏ�B�ړ����Ȃ�
		enSpecialActionState_AngryMode,			//�{�胂�[�h�B���̎������ړ�����
		SpecialActionState_Num
	};
	/// <summary>
	/// ���[�v���鎞�̏������Ԃ̃X�e�b�v
	/// </summary>
	enum EnWarpStepState
	{
		enWarpStep_Up,
		enWarpStep_Warp,
		enWarpStep_Down,
		enWarpStep_End
	};

	/// <summary>
	/// �X�e�[�g�}�V���X�e�[�g
	/// </summary>
	enum EnStateMachineState
	{
		enStateMachineState_Vigilance,	//�x���X�e�[�g�}�V��
		enStateMachineState_Attack		//�U���X�e�[�g�}�V��
	};

	/////////////////////////////////////////////////////////////////////
	//���z�֐��A�������z�֐��̐錾
	/////////////////////////////////////////////////////////////////////
	/// <summary>
	/// �U������
	/// </summary>
	void Attack() override {}
	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing() override;

	/// <summary>
	/// ����̃A�j���[�V�������Đ������B�����蔻��Ɏg��
	/// </summary>
	/// <returns></returns>
	bool isAnimationEnable() const override
	{
		return m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End;
	}
	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return m_enAnimationState != enAnimationState_DarkBall &&
			m_enAnimationState != enAnimationState_DarkWall &&
			m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_NormalAttack_1 &&
			m_enAnimationState != enAnimationState_NormalAttack_2 &&
			m_enAnimationState != enAnimationState_NormalAttack_3 &&
			m_enAnimationState != enAnimationState_CriticalHit &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End;
	}
	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns>true�ōU���\</returns>
	bool IsAttackEnable() const override
	{
		return m_enAnimationState != enAnimationState_DarkBall &&
			m_enAnimationState != enAnimationState_DarkWall &&
			m_enAnimationState != enAnimationState_NormalAttack_1 &&
			m_enAnimationState != enAnimationState_NormalAttack_2 &&
			m_enAnimationState != enAnimationState_NormalAttack_3 &&
			m_enAnimationState != enAnimationState_KnockBack &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_start &&
			m_enAnimationState != enAnimationState_Attack_DarkMeteorite_main &&
			m_enAnimationState != enAnimationState_DarkSpear_Start &&
			m_enAnimationState != enAnimationState_DarkSpear_Main &&
			m_enAnimationState != enAnimationState_DarkSpear_End &&
			m_enAnimationState != enAnimationState_CriticalHit;

	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack) override;

	/// <summary>
	/// �ʏ�U���ɓ����������̏���
	/// </summary>
	void HitNormalAttack() override;

	/// <summary>
	/// �X�L���ɓ����������̏���
	/// </summary>
	void HitSkillAttack() override;

	/// <summary>
	/// �E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	void HitFireBall() override {}
	/// <summary>
	/// �E�B�U�[�h�̃t���C���s���[�ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	void HitFlamePillar(bool damageFlag = false) override {}

	/// <summary>
	/// ��]�݂̂��s����������
	/// </summary>
	/// <returns></returns>
	bool RotationOnly() override
	{
		return false;
	}

	/// <summary>
	/// ���ꂽ�Ƃ��̏���
	/// </summary>
	/// <param name="seFlag">���ꂽ�Ƃ��̌��ʉ����Đ����邩�̃t���O</param>
	void ProcessDead(bool seFlag = true) override;

	/////////////////////////////////////////////////////////////////////
	//���̑��̊֐�
	/////////////////////////////////////////////////////////////////////

	/// <summary>
	/// ���̃X�e�[�g�}�V���𐶐�����
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetNextStateMachine(EnStateMachineState nextStateMachine);

	/// <summary>
	/// �X�^�[�g���̃X�e�[�g�}�V���̍쐬
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetStartStateMachine(EnStateMachineState nextStateMachine);

	/// <summary>
	/// �U�����I�������̈�ʂ�̏���
	/// </summary>
	void ProcessEndAttackState();

	/// <summary>
	/// ���u�����X�^�[���폜
	/// </summary>
	void DeleteMobMonsters();

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();


	/// <summary>
	/// ���݂̃X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	ISummonerState* GetNowSummonerState()
	{
		return m_nowBossState;
	}

	/// <summary>
	/// ���݂̃X�e�[�g�}�V�����擾
	/// </summary>
	/// <returns></returns>
	IBossStateMachine* GetNowStateMachine()
	{
		return m_stateMachine;
	}

	/// <summary>
	/// ��Ԃ̐؂�ւ�
	/// </summary>
	/// <param name="normalOrAngry">false�Ńm�[�}���Atrue�œ{�胂�[�h</param>
	void ChangeSpecialState(bool normalOrAngry)
	{
		if (normalOrAngry == true)m_enSpecialActionState = enSpecialActionState_AngryMode;
		else m_enSpecialActionState = enSpecialActionState_Normal;
	}

	/// <summary>
	/// ���݂̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	const EnSpecialActionState& GetNowSpecialActionState() const
	{
		return m_enSpecialActionState;
	}

	/// <summary>
	/// ���f���h���[�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDrawModelFlag(bool flag)
	{
		m_isDrawModelFlag = flag;
	}
	/// <summary>
	/// ���f���h���[�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsDrawModelFlag() const
	{
		return m_isDrawModelFlag;
	}

	/// <summary>
	/// ��_���[�W�J�E���g��ݒ�
	/// </summary>
	/// <param name="value"></param>
	void SetDamageCount(int value)
	{
		m_damageCount = value;
	}
	/// <summary>
	/// ��_���[�W�J�E���g���擾
	/// </summary>
	/// <returns></returns>
	const int& GetDamageCount() const
	{
		return m_damageCount;
	}

	/// <summary>
	/// �_�[�N�E�H�[���̃{�[��ID�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetDarkWallBoonId() const
	{
		return m_darkWallBoonId;
	}

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel()  override;
	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation()  override;
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState()  override;

	/// <summary>
	/// �ړ�����
	/// </summary>
	void ProcessMove();
	/// <summary>
	/// ��]����
	/// </summary>
	void ProcessRotation();

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	void ProcessHit(int hitDamage);

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̉񕜁B�u���C�N���Ă��Ȃ��Ə������Ȃ�
	/// </summary>
	void RecoverySuperArmor();
	

	/// <summary>
	/// �_�[�N�{�[������
	/// </summary>
	void CreateDarkBall();
	/// <summary>
	/// �_�[�N�E�H�[������
	/// </summary>
	void CreateDarkWall();

	/// <summary>
	/// �m�b�N�o�b�N����
	/// </summary>
	void ProcessKnockBack();

	/// <summary>
	/// �ʏ�U���̓����蔻�萶��
	/// </summary>
	void CreateNormalAttackCollision();

	/// <summary>
	/// �ʏ�U���̓����蔻�萶��
	/// </summary>
	void NormalComboFinnish();

private:

	EnAnimationState				m_enAnimationState = enAninationState_Idle;				//�A�j���[�V�����X�e�[�g
	EnSpecialActionState			m_enSpecialActionState = enSpecialActionState_Normal;	//���ʂȏ�ԃX�e�[�g(�ʏ�A�{�胂�[�h)
	EnWarpStepState					m_enWarpStep = enWarpStep_Up;

	EnStateMachineState	m_stateMachineState = enStateMachineState_Vigilance;

	Level3DRender					m_stageLevel;
	AnimationClip					m_animationClip[enAnimClip_Num];						// �A�j���[�V�����N���b�v 

	DarkWall* m_darkWall = nullptr;

	CharacterController				m_charaCon;												//�L�����N�^�[�R���g���[���[

	std::unique_ptr<IBossStateMachine> m_SummonerstateMachine = nullptr;


	IBossStateMachine*				m_stateMachine = nullptr;

	ISummonerState*					m_nowBossState = nullptr;

	bool							m_isDrawModelFlag = true;

	int								m_damageCount = 0;				//��_���[�W�������ɉ��Z����J�E���g

	int m_darkWallBoonId = -1;

	bool m_oldBreakSuperArmorFlag = false;	//�O�t���[���̃X�[�p�[�A�[�}�[�u���C�N�t���O

};

