#pragma once
#include "AIActor.h"

class Lich;
class ICactusState;

class Cactus :public AIActor
{
public:
	Cactus();
	~Cactus();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Move();

	Vector3 SetDirection();

	bool IsBumpedForest();

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
		return true;
	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

	///// <summary>
	///// ��_���[�W�p�����蔻��
	///// </summary>
	////�E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏���
	void HitFireBall();
	////�E�B�U�[�h�̃t���C���s���[�ɓ����������̏���
	void HitFlamePillar();

	bool RotationOnly();

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
		enAnimationState_Victory
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


	Lich* m_lich = nullptr;
	ICactusState* m_state = nullptr;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	ModelRender m_modelRender;

	CharacterController m_charaCon;

	float m_angleChangeTime = 6.0f;		//�x�N�g�����v�Z����^�C�}�[

	Vector3 m_direction = Vector3::Zero;

	const float m_distanceToPlayer = 300.0f;

};

