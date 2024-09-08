#pragma once
#include "BossEnemyBase.h"


#include "SummonerInfo.h"
#include "CommonEnemyStatus.h"
#include "SummonerSkillStatus.h"

#include "SummonerSMContext.h"
#include "SummonerStateContext.h"



class SummonerSkillStatus;

class SummonerSMContext;
class SummonerStateContext;
class SummonerAIController;

class Brave;

using namespace SummonerStates;
using namespace SummonerAnimationClips;
using namespace SummonerSkillType;

/// <summary>
/// �{�X�G�l�~�[�F�T���i�[�N���X
/// </summary>
class Summoner :public BossEnemyBase
{
public:
	~Summoner();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	Vector3 GetForwardYZero()
	{
		m_forward.y = 0.0f;
		m_forward.Normalize();
		return m_forward;
	}

	/// <summary>
	/// �V���b�N�E�F�[�u�̃_���[�W����ݒ肷��
	/// </summary>
	void SetShockWaveDamageInfo();

	/// <summary>
	/// �_���[�W����ݒ肷��
	/// </summary>
	/// <param name="skillType">�Z�̃^�C�v</param>
	void SettingDamageInfo(EnSkillSType skillType);

	/// <summary>
	/// �ڋ߂ł��鋗���ɓ��B����
	/// </summary>
	void ActiveArrivedApproachDistance()
	{
		m_isArrivedApproachDistance = true;
	}
	/// <summary>
	/// �ڋ߂ł��鋗���ɓ��B���Ă��Ȃ�
	/// </summary>
	void DeactiveArrivedApproachDistance()
	{
		m_isArrivedApproachDistance = false;
	}
	/// <summary>
	/// �ڋ߂ł��鋗�������Z�b�g����
	/// </summary>
	void ResetApproachDistanceValue()
	{
		m_status.ResetCurrentApproachDistance();
	}

	/// <summary>
	/// �ړ��X�g�b�v�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetStopMoveFlag(bool flag)
	{
		m_isStopMove = flag;
	}
	/// <summary>
	/// ��̃{�[��ID���擾
	/// </summary>
	/// <returns></returns>
	const int GetStaffBoonId()
	{
		return m_staffBoonId;
	}
	/// <summary>
	/// �E��̃{�[��ID���擾
	/// </summary>
	/// <returns></returns>
	const int GetRightHandBoonId()
	{
		return m_rightHandBoonId;
	}
	/// <summary>
	/// ����̃{�[��ID���擾
	/// </summary>
	/// <returns></returns>
	const int GetLeftHandBoonId()
	{
		return m_leftHandBoonId;
	}


	/// <summary>
	/// �X�e�[�g�؂�ւ�
	/// </summary>
	/// <param name="changeState"></param>
	void ChangeState(EnSummonerState changeState);
	/// <summary>
	/// �X�e�[�g�}�V����؂�ւ��邩�`�F�b�N
	/// </summary>
	bool CheckAndTransitionStateMachine();


	/// <summary>
	/// ���ʃX�e�[�g�̏���
	/// </summary>
	void ProcessCommonTranstion();

	/// <summary>
	/// ���g����v���C���[�Ɍ������������擾
	/// </summary>
	float GetDistanceToPlayerPositionValue();


	/// <summary>
	/// �R���{�U���̏���
	/// </summary>
	void UpdateComboAttackProcess();
	/// <summary>
	/// �U�����Ƀ^�[�Q�b�g�̂ق��ɉ�]����
	/// </summary>
	void UpdateAttackTurnToTargetProcess();
	/// <summary>
	/// �V���b�N�E�F�[�u�U���̏���
	/// </summary>
	void UpdateShockWaveProcess();

	/// <summary>
	/// ���S��̏���
	/// </summary>
	void AfterDieProcess();

	/// <summary>
	/// �O�����玩�g���폜
	/// </summary>
	void DieFlomOutside();


private:

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	void ProcessHit(DamageInfo damageInfo) override;

	/// <summary>
	/// �A�j���[�V�����N���b�v��ǂݍ���
	/// </summary>
	void LoadAnimationClip();

	/// <summary>
	/// �����蔻��̏��������Ȃ�����
	/// </summary>
	/// <returns></returns>
	bool IgnoreCollision() override;

	/// <summary>
	/// ���g���I�u�W�F�N�g�v�[���ɖ߂�
	/// </summary>
	void ReleaseThis() override;

	/// <summary>
	/// ����ɒǉ�����R���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void AddMoreComponent() override;

	/// <summary>
	/// �S�ẴR���|�[�l���g��������
	/// </summary>
	void InitComponents();


	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();


private:

	SummonerSkillStatus m_skillStatus;
	

	std::unique_ptr<SummonerSMContext> m_StateMachineCotext = nullptr;	//�X�e�[�g�}�V���Ǘ�

	std::unique_ptr<SummonerStateContext> m_stateContext = nullptr;		//�X�e�[�g�Ǘ�

	SummonerAIController* m_aiController = nullptr;


	AnimationClip m_animationClip[enSummonerAnimClip_Num];

	Vector3 m_currentRotDirection = g_vec3Zero;



	bool m_isSettingComponents = false;

	bool m_isAttackMove = false;



	int m_staffBoonId = -1;					//��̃{�[��ID
	int m_rightHandBoonId = -1;				//����̃{�[��ID
	int m_leftHandBoonId = -1;				//�E��̃{�[��ID

};

