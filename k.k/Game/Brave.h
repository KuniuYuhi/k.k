#pragma once
#include "Actor.h"

class Player;
class IBraveState;

/// <summary>
/// �E�҃N���X
/// </summary>
class Brave:public Actor
{
public:

	/// <summary>
	/// �A�N�V�������鎞�Ɏg���t���O���܂Ƃ߂Ă���
	/// </summary>
	struct InfoAboutActionFlag
	{
		bool isActionFlag = false;		//�A�N�V�����t���O
		bool dashAttackFlag = false;	//�O�i�U���t���O
		bool nextComboFlag = false;		//���̃R���{�U�������邩�̃t���O
		bool isComboReceptionFlag = false;	//�R���{��t�\�t���O
	};

	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void ProcessRotation();
	/// <summary>
	/// �U������
	/// </summary>
	void ProcessAttack();
	/// <summary>
	/// �h�䏈��
	/// </summary>
	void ProcessDefend();
	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	void Damage(int damage);
	/// <summary>
	/// �s���s�\���ǂ����̔���\
	/// </summary>
	/// <returns>true�ŕs�\�Afalse�ŉ\</returns>
	const bool& IsInaction() const;

	/// <summary>
	/// �U�����ɑO�i���鎞�Ɏg��
	/// </summary>
	/// <param name="Speed">�O�i���鑬��</param>
	void CalcAttackDirection(float Speed);

	void ChangeWeapon();



	/// <summary>
	/// �L�����N�^�[���`�F���W�\��
	/// </summary>
	/// <returns></returns>
	bool isAnimationSwappable() const
	{
		return true;
	}

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Skill_start &&
			m_enAnimationState != enAnimationState_Skill_Main &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// �����蔻��\�ȃA�j���[�V������
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Defend;
	}

	/// <summary>
	/// ��]�\�ȃA�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return m_enAnimationState == enAnimationState_Defend;
	}

	/// <summary>
	/// �����X�e�[�g��ݒ肷��
	/// </summary>
	void SetVictoryAnimationState()
	{

	}
	/// <summary>
	/// �A�C�h���X�e�[�g��ݒ肷��
	/// </summary>
	void SetIdleAnimationState()
	{

	}

	

	/// <summary>
	/// ���킪�����̎��̃X�L���̏���
	/// </summary>
	/// <param name="UpOrDownFlag">true��Up�AfalseDown</param>
	void ProcessSwordShieldSkill(bool UpOrDownFlag);

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Sprint,
		enAnimClip_DashForward,
		enAnimClip_KnockBack,
		enAnimClip_Hit,
		enAnimClip_Defend,
		enAnimClip_DefendHit,
		enAnimClip_Die,
		enAnimClip_ChangeSwordShield,
		enAnimClip_Win_Start,
		enAnimClip_Win_Main,
		enAnimClip_attack1,
		enAnimClip_attack2,
		enAnimClip_attack3,
		enAnimClip_Skill_Start,
		enAnimClip_Skill_Main,

		enAnimClip_Num
	};

	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAninationState_Sprint,
		enAninationState_DashForward,
		enAnimationState_KnockBack,
		enAnimationState_Hit,
		enAnimationState_Defend,
		enAnimationState_DefendHit,
		enAnimationState_Die,
		enAnimationState_ChangeSwordShield,
		enAnimationState_Win_Start,
		enAnimationState_Win_Main,
		enAnimationState_Attack_1,
		enAnimationState_Attack_2,
		enAnimationState_Attack_3,
		enAnimationState_Skill_start,
		enAnimationState_Skill_Main,
	};

	//�U���p�^�[���X�e�[�g�B�������R���{�̃^�C�~���O��}�邽��
	enum EnAttackPattern {
		enAttackPattern_None = static_cast<EnAttackPattern>(enAnimationState_Attack_1 - 1),
		enAttackPattern_1 = enAnimationState_Attack_1,
		enAttackPattern_2,
		enAttackPattern_3,
		enAttackPattern_End
	};

	/// <summary>
	/// ���f�������_�[�̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(int nextState);

	/// <summary>
	/// �X�e�[�g���ʂ̏�ԑJ�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ʏ�U���X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessNormalAttackStateTransition();
	/// <summary>
	/// �X�L���̃X�^�[�g�X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessSkillStartStateTransition();
	/// <summary>
	/// �X�L���̃��C���X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessSkillMainStateTransition();
	/// <summary>
	/// �U�����󂯂����̃X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessHitStateTransition();
	/// <summary>
	/// ���ꂽ���̃X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessDieStateTransition();

	/// <summary>
	/// �A�N�V�����t���O�\���̂̑S�Ẵt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAllInfoAboutActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = false;
		m_infoAboutActionFlag.dashAttackFlag = false;
		m_infoAboutActionFlag.nextComboFlag = false;
		m_infoAboutActionFlag.isComboReceptionFlag = false;
	}
	/// <summary>
	/// �A�N�V�����t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = flag;
	}
	/// <summary>
	/// �A�N�V�����t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsActionFlag() const
	{
		return m_infoAboutActionFlag.isActionFlag;
	}
	/// <summary>
	/// �O�i����U���t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDashAttackFlag(bool flag)
	{
		m_infoAboutActionFlag.dashAttackFlag = flag;
	}
	/// <summary>
	/// �O�i����U���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetDashAttackFlag() const
	{
		return m_infoAboutActionFlag.dashAttackFlag;
	}
	/// <summary>
	/// ���̃R���{�U�������邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetNextComboFlagFlag(bool flag)
	{
		m_infoAboutActionFlag.nextComboFlag = flag;
	}
	/// <summary>
	/// ���̃R���{�U�������邩�̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetNextComboFlagFlag() const
	{
		return m_infoAboutActionFlag.nextComboFlag;
	}
	/// <summary>
	/// �R���{��t�\�t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsComboReceptionFlagFlag(bool flag)
	{
		m_infoAboutActionFlag.isComboReceptionFlag = flag;
	}
	/// <summary>
	/// �R���{��t�\�t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsComboReceptionFlagFlag() const
	{
		return m_infoAboutActionFlag.isComboReceptionFlag;
	}


	/// <summary>
	/// �O�i����U���̃X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	float GetNormalAttackSpeed()
	{
		return m_normalAttackSpeed;
	}

private:
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	
	/// <summary>
	/// �R���{�U���̃R���{�̏���
	/// </summary>
	void ProcessComboAttack();


	/// <summary>
	/// �X�L���̎g�p���Ȃǂ̈ړ��͂��Ȃ�����]�͂������Ƃ��Ɏg��
	/// </summary>
	bool RotationOnly();


	void UpdateWeapons();


private:

	Player* m_player = nullptr;
	IBraveState* m_BraveState = nullptr;

	EnAnimationState m_enAnimationState = enAninationState_Idle;			//�A�j���[�V�����X�e�[�g
	EnAttackPattern m_attackPatternState = enAttackPattern_None;
	CharacterController m_charaCon;

	Animation	m_animation;				// �A�j���[�V����
	AnimationClip	m_animationClip[enAnimClip_Num];// �A�j���[�V�����N���b�v 

	ModelRender m_modelRender;

	InfoAboutActionFlag m_infoAboutActionFlag;

	int m_charaCenterBoonId = -1;

	//bool m_dashAttackFlag = false;		//�U�����ɑO�i���邩�̃t���O

	//bool m_isActionFlag = false;			//�U����h��Ȃǂ̃A�N�V�������N���������̃t���O
	//bool m_nextComboFlag = false;		//���̃R���{�U�������邩�̃t���O

	//bool m_isComboReceptionFlag = false;	//�R���{��t�\�t���O

	float m_mulYPos = 0.0f;

	const float m_normalAttackSpeed = 160.0f;

	//�Ȃ��Ȃ�
	ModelRender Sword;
	int m_swordBoonId = -1;

	ModelRender Shield;
	int m_shieldBoonId = -1;

	int m_playAnimCount = 0;

};

