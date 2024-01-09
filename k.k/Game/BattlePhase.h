#pragma once

namespace {
	const int SUMMON_MONSTERS_CONUT = 5;
}

class BattlePhase:public IGameObject
{
public:
	BattlePhase();
	~BattlePhase();

	bool Start();
	void Update();

private:
	
	/// <summary>
	/// �����X�^�[�̏������鏈��
	/// </summary>
	void ProcessSummonMonsters();

	/// <summary>
	/// ���u�����X�^�[�𐶐�
	/// </summary>
	/// <param name="createCount">�������鐔</param>
	void CreateMobMonsters(const int createCount);

	/// <summary>
	/// ���u�����X�^�[���폜
	/// </summary>
	void DeleteMobMonsters();

	/// <summary>
	/// �������郂���X�^�[�̍��W�̐ݒ�
	/// </summary>
	/// <param name="radius">��������͈͂̉~�̔��a</param>
	void SetSummonRandomPosition(const int radius);

	/// <summary>
	/// ���̍��W�Ƌ������󂢂Ă��邩
	/// </summary>
	/// <param name="count">���ׂ����z��̔ԍ�</param>
	/// <param name="distance">���̍��W�Ƃ̊Ԋu</param>
	/// <returns></returns>
	bool IsWithInDistances(int count,float distance);

	/// <summary>
	/// ���݂̃t�F�[�Y�ɂ���ē�Փx��ύX
	/// </summary>
	/// <param name="summonMonstersCount">�������郂���X�^�[�̐�</param>
	void AdjustDifficultyByPhase(const int summonMonstersCount);
	/// <summary>
	/// ��Փx�F�ȒP�̏���
	/// </summary>
	void processEasyDifficulty(const int summonMonstersCount);
	/// <summary>
	/// ��Փx�F���ʂ̏���
	/// </summary>
	void processNormalDifficulty(const int summonMonstersCount);
	/// <summary>
	/// ��Փx�F����̏���
	/// </summary>
	void processHardDifficulty(const int summonMonstersCount);

	/// <summary>
	/// �r������(�����̏���)
	/// </summary>
	/// <returns></returns>
	bool IsIncantation();

	/// <summary>
	/// ���@�w�̐���
	/// </summary>
	/// <param name="monsterCount">���@�w�𐶐����鐔(�����X�^�[�̐��Ɠ���)�B</param>
	void CreateMagicCircleEffect(const int monsterCount);

	/// <summary>
	/// �������ꂽ�u�Ԃ̌��̏���
	/// </summary>
	/// <param name="createPos"></param>
	void CreateSummonLightEffect(Vector3 createPos);

private:
	/// <summary>
	/// �����X�^�[�����̃X�e�b�v
	/// </summary>
	enum EnSummonMonsterStep
	{
		enSummonMonsterStep_SettingMagicCircle,
		enSummonMonsterStep_SummonMonsters,
	};
	EnSummonMonsterStep m_enSummonMonsterStep = enSummonMonsterStep_SettingMagicCircle;

	ModelRender model[SUMMON_MONSTERS_CONUT];

	Vector3 m_createPos[SUMMON_MONSTERS_CONUT];

	float m_incantationTimer = 0.0f;

	

};

