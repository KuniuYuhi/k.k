#pragma once
#include "MobMonster.h"

using namespace MobMonsterInfo;

class Lich;
class ICactusState;
class IMobStateMachine;


class Cactus :public MobMonster
{
public:
	Cactus();
	~Cactus();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	/// <summary>
	/// �����蔻�萶��
	/// </summary>
	void CreateCollision();

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
	bool isAnimationEnable() const override
	{
		return m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return true;//m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	///  �U������
	/// </summary>
	/// <returns></returns>
	bool IsAttackEnable() const override
	{
		return m_enAnimationState != enAnimationState_Attack &&
			m_enAnimationState != enAnimationState_Skill;
	}

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

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

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	void SetNextAnimationState(EnAnimationState nextState);

	/// <summary>
	/// ���̃X�e�[�g�}�V�����쐬����
	/// </summary>
	/// <param name="nextStateMachine"></param>
	void SetNextStateMachine(EnStateMachineState nextStateMachine) override;

	/// <summary>
	/// �����蔻�萶���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetCreateAttackCollisionFlag() const
	{
		return m_createAttackCollisionFlag;
	}

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

	ICactusState*					m_state = nullptr;

	Animation						m_animation;										//�A�j���[�V����
	AnimationClip					m_animationClip[enAnimationClip_Num];					//�A�j���[�V�����N���b�v 

	EnAnimationState				m_enAnimationState = enAninationState_Idle;			//�A�j���[�V�����X�e�[�g

	EnStateMachineState m_enStateMachineState = enStateMachineState_Patrol;

	ModelRender						m_modelRender;

	CollisionObject*				m_headCollision = nullptr;

	int								m_attackBoonId = -1;								//�U���Ŏg���{�[��ID

	bool							m_createAttackCollisionFlag = false;
};

