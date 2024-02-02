#pragma once

#include "MobMonster.h"

using namespace MobMonsterInfo;

class ITurtleShellState;
class IMobStateMachine;

class TurtleShell:public MobMonster
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// �����蔻�萶��
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	bool IsStopProcessing();

	/// <summary>
	///  �h����I��邩
	/// </summary>
	/// <returns>true�ŏI���</returns>
	bool IsDifenceEnd();

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
		return //m_enAnimationState != enAnimationState_Difence &&
			//m_enAnimationState != enAnimationState_DifenceDamage &&
			m_enAnimationState != enAnimationState_Hit &&
			m_enAnimationState != enAnimationState_Die;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool isRotationEnable() const override
	{
		return m_enAnimationState != enAninationState_Idle;
	}

	/// <summary>
	/// �U������
	/// </summary>
	/// <returns></returns>
	bool IsAttackEnable() const override
	{
		return /*m_enAnimationState != enAnimationState_Difence &&*/
			m_enAnimationState != enAnimationState_Attack &&
			m_enAnimationState != enAnimationState_Skill;
	}

	//�ʏ�U���ɓ����������̏���
	void HitNormalAttack() override;

	//�X�L���ɓ����������̏���
	void HitSkillAttack() override;

	/// <summary>
	/// �X�L���g�p�\���^�C�}�[
	/// </summary>
	/// <returns>�g�p�\�Ȃ�true</returns>
	bool IsSkillUsable() override;

	/*/// <summary>
	/// �G����������
	/// </summary>
	/// <returns>��������true</returns>
	bool IsFoundPlayerFlag() override;*/

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	void Damage(int attack);

	bool RotationOnly();

	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ��������s
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �A�^�b�N�P�X�e�[�g�J�ڏ��������s
	/// </summary>
	void OnProcessAttack_1StateTransition();
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
	
	ITurtleShellState* m_state = nullptr;

	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClip[enAnimationClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

	EnStateMachineState m_enStateMachineState = enStateMachineState_Patrol;

	CollisionObject* m_headCollision = nullptr;
	
	ModelRender m_modelRender;

	
	int m_attackBoonId = -1;					//�U���Ŏg���{�[��ID

	bool m_createAttackCollisionFlag = false;


	bool m_damagedFlag = false;			//���g���U�����󂯂����̃t���O
	bool m_difenceEnableFlag = false;			//�h�䂵�Ă��邩�̃t���O
	const float m_difenceTime = 3.0f;
	float m_difenceTimer = 0.0f;

};

