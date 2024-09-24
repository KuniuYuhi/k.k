#pragma once
#include "WaveStatus.h"


class WaveStatus;
class EmnemyBase;
class Summoner;

/// <summary>
/// �E�F�[�u�󋵂��Ǘ�����N���X
/// </summary>
class WaveManager:public IGameObject
{
public:

	//���@�w
	//�G�l�~�[����
	//�E�F�[�u�i�s
	//�I�����̕]��
	//�u���C�N�^�C��


	enum EnWaveManageState
	{
		enWaveState_FirstSummonMobEnemys,		//�ŏ��̃G�l�~�[�̐���
		enWaveState_ProgressWaveTime,			//�E�F�[�u���Ԃ̐i�s�B���[�v����Ƃ��͂�������
		enWaveState_EvaluateWaveResults,		//�E�F�[�u�̕]��
		enWaveState_ShortBreakTime,				//���x�e
		enWaveState_SettingCreatePos,			//�����ʒu�̃Z�b�e�B���O
		enWaveState_SummonMobEnemys,			//���u�G�l�~�[����
		enWaveState_BreakTimeWave,				//�x�e�E�F�[�u
	};

	EnWaveManageState m_currentWaveManageState = enWaveState_FirstSummonMobEnemys;
	EnWaveManageState m_nextWaveManageState = enWaveState_FirstSummonMobEnemys;

	//
	// ���Ԋu���ƂɓG�𐶐�����B
	// �G�̓E�F�[�u�؂�ւ����ɍ폜���Ȃ�
	// �ő吔�𒴂��Ȃ��悤�ɐ���
	//

	WaveManager();
	~WaveManager();


	bool Start() override;

	void Update() override;


	const int& GetMobEnemySize()
	{
		return m_mobEnemyList.size();
	}

	const float& GetCurrentWaveTimer()
	{
		return m_currentWaveTimer;
	}

	const float& GetCurrentTimeLimit()
	{
		return m_currentTimeLimit;
	}
	


private:


	////////////////////////////////////////////////////////////////////////
	///�X�e�[�g�֐�
	////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// �ŏ��̃G�l�~�[�̐���
	/// </summary>
	void ProcessFirstSummonMobEnemysStateTransition();
	/// <summary>
	/// �E�F�[�u���Ԃ̐i�s����
	/// </summary>
	void ProcessProgressWaveTimeStateTransition();
	/// <summary>
	/// �E�F�[�u�̕]��
	/// </summary>
	void ProcessEvaluateWaveResultsStateTransition();
	/// <summary>
	/// �E�F�[�u��̏��x�e
	/// </summary>
	void ProcessShortBreakTimeStateTransition();
	/// <summary>
	/// �����ʒu�̃Z�b�e�B���O
	/// </summary>
	void ProcessSettingCreatePosStateTransition();
	/// <summary>
	/// ���u�G�l�~�[����
	/// </summary>
	void ProcessSummonMobEnemysStateTransition();
	/// <summary>
	/// �x�e�E�F�[�u
	/// </summary>
	void ProcessBreakTimeWaveStateTransition();

	////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �E�F�[�u�^�C�}�[�̌v�Z
	/// </summary>
	/// <param name="nextWaveState">���̃X�e�[�g</param>
	void CalcWaveTimer(EnWaveManageState nextWaveState);

	/// <summary>
	/// �E�F�[�u�I�����̕]��
	/// </summary>
	void EvaluateWaveResults();


	/// <summary>
	/// �E�F�[�u�̐i�s���Ǘ�
	/// </summary>
	void ManageWaveProgression();


	/// <summary>
	/// ���u�G�l�~�[����������
	/// </summary>
	void SummonMobEnemys();

	/// <summary>
	/// �������@�w�̐���
	/// </summary>
	/// <param name="castAmount"></param>
	void CastSummmonCircle(int castAmount);

	/// <summary>
	/// �������郂���X�^�[�̍��W�̐ݒ�
	/// </summary>
	/// <param name="radius">��������͈͂̉~�̔��a</param>
	void SetSummonRandomPosition(const int radius,int amount);

	/// <summary>
	/// ���̍��W�Ƌ������󂢂Ă��邩
	/// </summary>
	/// <param name="count">���ׂ����z��̔ԍ�</param>
	/// <param name="distance">���̍��W�Ƃ̊Ԋu</param>
	/// <returns></returns>
	bool IsWithInDistances(int count, float distance);


	bool IsChangeSummonState();

private:

	Summoner* m_summoner = nullptr;

	WaveStatus m_waveStatus;		//�E�F�[�u�̃X�e�[�^�X

	std::vector<EmnemyBase*> m_mobEnemyList = {nullptr};		//���u�G�l�~�[���X�g

	std::vector<Vector3> m_createPositions;

	float m_currentWaveTimer = 0.0f;
	float m_currentTimeLimit = 0.0f;

	int m_currentWaveNumber = 0;


	float m_changeSummonTimer = 0.0f;

	bool m_firstSummonSircleSet = false;

};

