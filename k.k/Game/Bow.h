#pragma once

#include "IWeapon.h"

class Bow :public IWeapon
{
public:
	Bow();
	~Bow();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// ���̓����蔻�肪�q�b�g������
	/// </summary>
	/// <returns>�q�b�g������true�A�q�b�g���Ȃ�������false</returns>
	bool IsHitCollision();

	/// <summary>
	/// �X�L���U������
	/// </summary>
	void ProcessSkillAttack();


private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// �����蔻��̐���
	/// </summary>
	void InitCollision() override;
	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;

private:
	ModelRender m_modelBow;		//�|���f��
	ModelRender m_modelArrow;		//��f��

	CollisionObject* m_arrowCollision = nullptr;	//��̓����蔻��

	Vector3 m_bowPos = g_vec3Zero;
	Vector3 m_arrowPos = g_vec3Zero;

	Matrix m_bowMatrix = g_matIdentity;
	Matrix m_arrowMatrix = g_matIdentity;

	//������������鎞�̃{�[��ID
	int m_armedBowBoonId = -1;
	int m_armedArrowBoonId = -1;
	//��������܂������̃{�[��ID
	int m_stowedBowBoonId = -1;
	int m_stowedArrowBoonId = -1;



};

