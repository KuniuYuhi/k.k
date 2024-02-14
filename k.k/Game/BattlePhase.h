#pragma once

class MobMonster;

class BattlePhase:public IGameObject
{
public:
	BattlePhase();
	~BattlePhase();

	bool Start() override;
	void Update() override;

private:
	
	/// <summary>
	/// �����X�^�[�̏������鏈��
	/// </summary>
	void ProcessSummonMonsters();

	/// <summary>
	/// ���u�����X�^�[�̐������鐔�����߂�
	/// </summary>
	/// <returns></returns>
	void SetMobMonsterSpawnCount();

	/// <summary>
	/// ���u�����X�^�[�𐶐�
	/// </summary>
	void CreateMobMonsters();

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

	/// <summary>
	/// �D�悷�郂�u�����X�^�[�̔ԍ����擾�B�ǂ̃��u�����X�^�[��I�΂�Ȃ����Ƃ�����
	/// </summary>
	/// <returns></returns>
	const int GetMobMonsterPriority();

	/// <summary>
	/// ���u�����X�^�[�𐶐�����
	/// </summary>
	/// <param name="priorityNumber">�������郂���X�^�[�̔ԍ��B�D��x�B0�ȏ�Ȃ炱�̔ԍ��̃����X�^�[�����������Ȃ�</param>
	/// <returns></returns>
	MobMonster* GenerateMobMonster(int priorityMobMonsterNumber = -1);


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

	std::vector<Vector3> m_createPositions;

	float m_incantationTimer = 0.0f;

	int m_mobMonsterSpawnCount = 0;

};

