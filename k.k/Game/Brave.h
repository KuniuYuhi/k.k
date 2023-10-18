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
	Brave();
	~Brave();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �U������
	/// </summary>
	void ProcessAttack();
	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W</param>
	void Damage(int damage);

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
		return true;
	}

	/// <summary>
	/// �����蔻��\�ȃA�j���[�V������
	/// </summary>
	/// <returns></returns>
	bool isCollisionEntable() const
	{
		return true;
	}

	/// <summary>
	/// ��]�\�ȃA�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isRotationEntable() const
	{
		return true;
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
		enAnimClip_attack4,
		enAnimClip_attack5,

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
		enAnimationState_Attack_4,
		enAnimationState_Attack_5,
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
	void SetNextAnimationState(EnAnimationState nextState);

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
	/// �X�L���̎g�p���Ȃǂ̈ړ��͂��Ȃ�����]�͂������Ƃ��Ɏg��
	/// </summary>
	bool RotationOnly();


	void UpdateWeapons();


private:

	Player* m_player = nullptr;
	IBraveState* m_BraveState = nullptr;

	EnAnimationState m_enAnimationState = enAninationState_Idle;			//�A�j���[�V�����X�e�[�g

	CharacterController m_charaCon;

	Animation	m_animation;				// �A�j���[�V����
	AnimationClip	m_animationClip[enAnimClip_Num];// �A�j���[�V�����N���b�v 

	ModelRender m_modelRender;



	ModelRender Sword;
	int m_swordBoonId = -1;

	ModelRender Shield;
	int m_shieldBoonId = -1;

	int m_playAnimCount = 0;

};

