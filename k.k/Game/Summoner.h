#pragma once
#include "BossBase.h"
#include "Level3DRender.h"


class IBossStateMachine;
class ISummonerState;

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
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		//enAnimClip_Walk,			// 1 : �����A�j���[�V����
		//enAnimClip_Run,				// 2 : ����A�j���[�V����
		enAnimClip_DarkBall,		// 3 : 
		enAnimClip_DarkWall,		// 4 : 
		enAnimClip_NormalAttack_1,
		enAnimClip_NormalAttack_2,
		enAnimClip_NormalAttack_3,
		//enAnimClip_Attack_3_start,		// 5 : 
		//enAnimClip_Attack_3_main,
		//enAnimClip_Attack_4,		// 6 : 
		//enAnimClip_Attack_DarkMeteorite_start,
		//enAnimClip_Attack_DarkMeteorite_main,
		//enAnimClip_Attack_DarkMeteorite_end,
		//enAnimClip_Summon,
		//enAnimClip_Damage,
		//enAnimClip_Die,
		//enAnimClip_Victory,
		//enAnimClip_Angry,
		enAnimClip_Command,
		enAnimClip_KnockBack,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};
	/// <summary>
	/// �A�j���[�V�����X�e�[�g
	/// </summary>
	enum EnAnimationState {
		enAninationState_Idle,
		//enAninationState_Walk,
		//enAninationState_Run,
		enAnimationState_DarkBall,
		enAnimationState_DarkWall,
		enAnimationState_NormalAttack_1,
		enAnimationState_NormalAttack_2,
		enAnimationState_NormalAttack_3,
		//enAnimationState_Attack_3_start,
		//enAnimationState_Attack_3_main,
		//enAnimationState_Attack_4,
		//enAnimationState_Attack_DarkMeteorite_start,
		//enAnimationState_Attack_DarkMeteorite_main,
		//enAnimationState_Attack_DarkMeteorite_end,
		//enAninationState_Summon,
		//enAnimationState_Damage,
		//enAnimationState_Die,
		//enAnimationState_Victory,
		//enAnimationState_Angry,
		//enAnimationState_Warp,
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
	bool IsStopProcessing() override { return false; }

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEnable() const override
	{
		return false;
	}
	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return m_enAnimationState != enAnimationState_DarkBall ||
			m_enAnimationState != enAnimationState_DarkWall ||
			m_enAnimationState != enAnimationState_KnockBack;
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
			m_enAnimationState != enAnimationState_KnockBack;

	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack) override{}

	//�ʏ�U���ɓ����������̏���
	void HitNormalAttack() override {};

	//�q�[���[�̃X�L���ɓ����������̏���
	void HitSkillAttack() override {};
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




	/////////////////////////////////////////////////////////////////////
	//���̑��̊֐�
	/////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �ړ�����
	/// </summary>
	void ProcessMove();
	/// <summary>
	/// ��]����
	/// </summary>
	void ProcessRotation();

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
	/// ���݂̏�Ԃ��擾
	/// </summary>
	/// <returns></returns>
	ISummonerState* GetNowSummonerState()
	{
		return m_nowBossState;
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



private:
	EnAnimationState				m_enAnimationState = enAninationState_Idle;				//�A�j���[�V�����X�e�[�g
	EnSpecialActionState			m_enSpecialActionState = enSpecialActionState_Normal;	//���ʂȏ�ԃX�e�[�g(�ʏ�A�{�胂�[�h)
	EnWarpStepState					m_enWarpStep = enWarpStep_Up;

	Level3DRender					m_stageLevel;
	AnimationClip					m_animationClip[enAnimClip_Num];						// �A�j���[�V�����N���b�v 

	CharacterController				m_charaCon;												//�L�����N�^�[�R���g���[���[

	IBossStateMachine*				m_stateMachine = nullptr;

	ISummonerState*					m_nowBossState = nullptr;


};

