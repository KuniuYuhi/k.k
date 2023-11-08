#pragma once
#include "Actor.h"
#include "IWeapon.h"

class Player;
class IBraveState;
class IWeapon;

class SwordShield;

/// <summary>
/// �E�҃N���X
/// </summary>
class Brave:public Actor
{
public:

	enum EnWepons
	{
		enWeapon_Main,
		enWeapon_Sub,
		enWeapon_num
	};

	/// <summary>
	/// �A�N�V�������鎞�Ɏg���t���O���܂Ƃ߂Ă���
	/// </summary>
	struct InfoAboutActionFlag
	{
		bool isActionFlag = false;		//�A�N�V�����t���O
		bool moveforwardFlag = false;	//�O�i�U���t���O
		bool nextComboFlag = false;		//���̃R���{�U�������邩�̃t���O
		bool isComboReceptionFlag = false;	//�R���{��t�\�t���O
		bool isCollisionPossibleFlag = false;	//�����蔻�肪�L�����̔���t���O�B
		bool attackHitFlag = false;		//�U��������Ƀq�b�g�������̃t���O
	};

	/// <summary>
	/// �g�p���镐��̍\����
	/// </summary>
	struct UseWeapon
	{
		IWeapon* weapon = nullptr;	//����I�u�W�F�N�g�̕ϐ�
		int weaponAnimationStartIndexNo = 0;	//����̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ�
	};

	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	//////////////////////////////////////////////////////////////
	// ���z�֐��A�������z�֐�
	//////////////////////////////////////////////////////////////

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	void Damage(int damage) override;
	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const override
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2 &&
			m_enAnimationState != enAnimationState_Attack_3 &&
			m_enAnimationState != enAnimationState_Skill_start &&
			m_enAnimationState != enAnimationState_Skill_Main &&
			m_enAnimationState != enAnimationState_Defend &&
			m_enAnimationState != enAnimationState_DefendHit &&
			m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Die &&
			m_enAnimationState != enAnimationState_ChangeSwordShield;
	}
	/// <summary>
	/// �����蔻��\�ȃA�j���[�V������
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const override;
	/// <summary>
	/// ��]�\�ȃA�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const override;

	/// <summary>
	/// �������̏���
	/// </summary>
	void ProcessWin() override;

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState() override;
	/// <summary>
	/// �X�L���̎g�p���Ȃǂ̈ړ��͂��Ȃ�����]�͂������Ƃ��Ɏg��
	/// </summary>
	bool RotationOnly() override;

	//////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	//////////////////////////////////////////////////////////////

public:
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
	/// �s���s�\���ǂ����̔���\
	/// </summary>
	/// <returns>true�ŕs�\�Afalse�ŉ\</returns>
	const bool& IsInaction() const;

	/// <summary>
	/// �U�����ɑO�i���鎞�Ɏg��
	/// </summary>
	/// <param name="Speed">�O�i���鑬��</param>
	void MoveForward(float Speed);

	/// <summary>
	///	����̐؂�ւ�����
	/// </summary>
	void ChangeWeapon();
	/// <summary>
	/// ���킪�����̎��̃X�L���̏���
	/// </summary>
	/// <param name="UpOrDownFlag">true��Up�AfalseDown</param>
	void ProcessSwordShieldSkill(bool UpOrDownFlag);

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Sprint,
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
	/// �X�e�[�g���ʂ̕���؂�ւ�����
	/// </summary>
	void ProcessCommonWeaponChangeStateTransition();
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
	/// �h��A����̃X�e�[�g�̏�ԑJ�ڏ���
	/// </summary>
	void ProcessDefendStateTransition();

	/// <summary>
	/// �A�N�V�����t���O�\���̂̑S�Ẵt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAllInfoAboutActionFlag(bool flag)
	{
		m_infoAboutActionFlag.isActionFlag = flag;
		m_infoAboutActionFlag.moveforwardFlag = flag;
		m_infoAboutActionFlag.nextComboFlag = flag;
		m_infoAboutActionFlag.isComboReceptionFlag = flag;
		m_infoAboutActionFlag.isCollisionPossibleFlag = flag;
		m_infoAboutActionFlag.attackHitFlag = flag;
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
	/// �O�i����t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetMoveforwardFlag(bool flag)
	{
		m_infoAboutActionFlag.moveforwardFlag = flag;
	}
	/// <summary>
	/// �O�i����U���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetMoveForwardFlag() const
	{
		return m_infoAboutActionFlag.moveforwardFlag;
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
	/// �����蔻��L�����t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsCollisionPossibleFlag(bool flag)
	{
		m_infoAboutActionFlag.isCollisionPossibleFlag = flag;
	}
	/// <summary>
	/// �����蔻��L�����t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetIsCollisionPossibleFlag() const
	{
		return m_infoAboutActionFlag.isCollisionPossibleFlag;
	}
	/// <summary>
	/// �U�����q�b�g�������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackHitFlag(bool flag)
	{
		m_infoAboutActionFlag.attackHitFlag = flag;
	}
	/// <summary>
	/// �U�����q�b�g�������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetAttackHitFlag() const
	{
		return m_infoAboutActionFlag.attackHitFlag;
	}

	/// <summary>
	/// �O�i����U���̃X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	float GetNormalAttackSpeed()
	{
		return m_normalAttackSpeed;
	}
	/// <summary>
	/// �������Ƃ��̃X�s�[�h�̎擾
	/// </summary>
	/// <returns></returns>
	const float& GetAvoidSpeed() const
	{
		return m_avoidSpeed;
	}
	
	/// <summary>
	/// ����̎擾
	/// </summary>
	/// <param name="subOrMain">�T�u�����C���̃X�e�[�g</param>
	/// <returns></returns>
	IWeapon* GetWeapon(EnWepons subOrMain) const
	{
		return m_useWeapon[subOrMain].weapon;
	}
	/// <summary>
	/// ���C������̃A�j���[�V�����N���b�v�̍ŏ��̔ԍ����擾
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentMainWeaponAnimationStartIndexNo() const
	{
		return m_currentAnimationStartIndexNo;
	}
	/// <summary>
	/// ���C������̖h��^�C�v���擾
	/// </summary>
	/// <returns></returns>
	const int& GetMainWeaponDefendTipe() const
	{
		return m_useWeapon[enWeapon_Main].weapon->GetEnDefendTipe();
	}

	/// <summary>
	/// �U���̒��S���W��ݒ�
	/// </summary>
	/// <param name="attackPosition"></param>
	void SetAttackPosition(Vector3 attackPosition);
	/// <summary>
	/// �m�b�N�o�b�N���鎞�̐ݒ���܂Ƃ߂�����
	/// </summary>
	/// <param name="attackPosition">�U���̒��S���W</param>
	/// <param name="power">�m�b�N�o�b�N�p���[</param>
	/// <param name="flag">�m�b�N�o�b�N���邩�̃t���O</param>
	void SetKnockBackInfo(
		Vector3 attackPosition, float power, bool flag)
	{
		//�U���̒��S���W��ݒ�
		SetAttackPosition(attackPosition);
		//�m�b�N�o�b�N�p���[��ݒ�
		SetKnockBackPower(power);
		//�m�b�N�o�b�N�U���t���O���Z�b�g
		SetKnockBackAttackFalg(flag);
	}

private:
	/// <summary>
	/// �R���{�U���̃R���{�̏���
	/// </summary>
	void ProcessComboAttack();
	/// <summary>
	/// ���C������ƃT�u��������ւ���
	/// </summary>
	void ReverseWeapon();
	/// <summary>
	/// UseWeapon�\���̂̒��g�����ւ���
	/// </summary>
	void ChangeUseWeapon();

	/// <summary>
	/// �h�䒆�Ƀq�b�g������
	/// </summary>
	/// <returns>�q�b�g�Ȃ�true�A�q�b�g���Ă��Ȃ��Ȃ�false</returns>
	bool IsDefendHit();

	/// <summary>
	/// �O�������ݒ�ł��������
	/// </summary>
	/// <returns>true�ŉ\�Afalse�ŕs�\</returns>
	const bool& IsSetForwardCondition() const
	{
		if (isAnimationEntable() == true)
		{
			return true;
		}


		//��]�̂݉\�ȃA�j���[�V�����ł͂Ȃ��Ȃ�
		return 
			GetMoveForwardFlag() != true;
	}

private:
	/// <summary>
	/// ���킻�ꂼ��̃A�j���[�V�����N���b�v�O���[�v
	/// </summary>
	enum AnimationClipGroup {
		AnimationClipGroup_OneHandedSword,	// �Ў茕�𑕔����̃A�j���[�V�����N���b�v�O���[�v
		AnimationClipGroup_TwoHandedSword,	// ���茕�𑕔����̃A�j���[�V�����N���b�v�O���[�v
		AnimationClipGroup_Bow,				// �|�𑕔����̃A�j���[�V�����N���b�v�O���[�v
		AnimationClipGroup_Num,
	};
	//�Ў茕�̍ŏ��̃A�j���[�V�����N���b�v�̔ԍ�
	const int OneHandSwordAnimationStartIndexNo = AnimationClipGroup_OneHandedSword;
	//���茕�̍ŏ��̃A�j���[�V�����N���b�v�̔ԍ�
	const int TwoHandSwordAnimationStartIndexNo = enAnimClip_Num * AnimationClipGroup_TwoHandedSword;
	//�|�̍ŏ��̃A�j���[�V�����N���b�v�̔ԍ�
	const int BowAnimationStartIndexNo = enAnimClip_Num * AnimationClipGroup_Bow;

	//���݂̕���̃A�j���[�V�����̍ŏ��̔ԍ�
	int m_currentAnimationStartIndexNo = OneHandSwordAnimationStartIndexNo;

	UseWeapon					m_useWeapon[enWeapon_num];	//�g������

	IWeapon*					m_weapon[enWeapon_num];	//����̐�
	IWeapon*					m_mainWeapon = nullptr;	//���C������
	IWeapon*					m_subWeapon = nullptr;		//�T�u����

	Player*						m_player = nullptr;
	IBraveState*				m_BraveState = nullptr;
	SwordShield*				m_swordShield = nullptr;

	EnAnimationState			m_enAnimationState = enAninationState_Idle;			//�A�j���[�V�����X�e�[�g
	EnAttackPattern				m_attackPatternState = enAttackPattern_None;
	CharacterController			m_charaCon;
	
	AnimationClip				m_animationClip[enAnimClip_Num * AnimationClipGroup_Num];// �A�j���[�V�����N���b�v 
	
	ModelRender					m_modelRender;

	InfoAboutActionFlag			m_infoAboutActionFlag;

	int							m_charaCenterBoonId = -1;

	float						m_mulYPos = 0.0f;

	const float					m_normalAttackSpeed = 160.0f;

	const float					m_avoidSpeed = 230.0f;

	

};

