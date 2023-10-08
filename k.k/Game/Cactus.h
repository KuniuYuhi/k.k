#pragma once
#include "MobMonster.h"

class Lich;
class ICactusState;

class Cactus :public MobMonster
{
public:
	Cactus();
	~Cactus();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void Attack();

	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	void CreateCollision();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
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
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return m_enAnimationState != enAnimationState_Damage &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return true;//m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsAttackEntable() const
	{
		return m_enAnimationState != enAnimationState_Attack_1 &&
			m_enAnimationState != enAnimationState_Attack_2;
	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Walk,			// 1 : �����A�j���[�V����
		enAnimClip_Run,				// 2 : ����A�j���[�V����
		enAnimClip_Attack_1,		// 3 : 
		enAnimClip_Attack_2,
		enAnimClip_Plant,
		enAnimClip_PlantToBattle,
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
	/// �v�����g�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessPlantStateTransition();
	/// <summary>
	/// �v�����g����o�g���X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessPlantToBattleStateTransition();
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
		enAnimationState_Plant,
		enAnimationState_PlantToBattle,
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

	/// <summary>
	/// �U�����̉����Đ�
	/// </summary>
	void PlayAttackSound();


	enum EnAttackName
	{
		enAttackName_1,
		enAttackName_2
	};
	EnAttackName m_enAttackName;


	ICactusState*					m_state = nullptr;

	Animation						m_animation;										//�A�j���[�V����
	AnimationClip					m_animationClip[enAnimClip_Num];					//�A�j���[�V�����N���b�v 

	EnAnimationState				m_enAnimationState = enAninationState_Idle;			//�A�j���[�V�����X�e�[�g

	ModelRender						m_modelRender;

	CharacterController				m_charaCon;

	int								m_attackBoonId = -1;								//�U���Ŏg���{�[��ID

	bool							m_createAttackCollisionFlag = false;
};

