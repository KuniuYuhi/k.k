#pragma once
#include "IComponent.h"


class Summoner;
class Brave;

/// <summary>
/// �T���i�[��AI�R���g���[���[�B�A�N�V�����ׂ̍����������L��
/// </summary>
class SummonerAIController :public IComponent
{
public:

	SummonerAIController();
	~SummonerAIController();

	void Start() override;

	void UpdateComponent() override;


	/// <summary>
	/// ���[�v���W���X�e�[�W�̃��x���t�@�C������ǂݍ���
	/// </summary>
	void InitWarpPointsFromStageLevel();

	/// <summary>
	/// ���[�v��̍��W������
	/// </summary>
	void DecisionWarpPoint();
	/// <summary>
	/// ���[�v����
	/// </summary>
	void WarpProcess();

	/// <summary>
	/// �V���b�N�E�F�[�u�̓����蔻��쐬
	/// </summary>
	void CreateShockWaveCollision();

	/// <summary>
	/// �R���{�U���̓����蔻��𐶐�
	/// </summary>
	void CreateComboAttackCollision();
	/// <summary>
	///	�O�R���{�ڂ̓����蔻��̐���
	/// </summary>
	void CreateThirdComboCollision();

	/// <summary>
	/// �_�[�N�{�[���̐���
	/// </summary>
	void CreateDarkBall();

	/// <summary>
	/// �_�[�N���e�I�̗����ʒu������
	/// </summary>
	void DecisionDarkMeteoriteFallPoint();
	/// <summary>
	/// �_�[�N���e�I�𐶐�
	/// </summary>
	void CreateDarkMeteorite();

private:
	

private:
	Summoner* m_summoner = nullptr;		//�T���i�[�̃C���X�^���X

	Brave* m_player = nullptr;

	std::vector<Vector3> m_warpPointsList;		//���[�v����W���܂Ƃ߂����X�g

	CollisionObject* m_shockWaveCollision = nullptr;	//�V���b�N�E�F�[�u�p�R���W����

	CollisionObject* m_comboAttackCollision = nullptr;	//�R���{�U���p�R���W����


	Vector3 m_warpPosition = g_vec3Zero;			//���[�v���W

	Vector3 m_darkMeteoriteFallPoint = g_vec3Zero;		//�_�[�N���e�I�����n�_



	int m_darkBallCreateCount = 0;		//�_�[�N�{�[���������J�E���g

};

