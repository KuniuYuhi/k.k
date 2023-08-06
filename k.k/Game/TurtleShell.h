#pragma once
#include "AIActor.h"

class TurtleShell:public AIActor
{
public:
	TurtleShell();
	~TurtleShell();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void Move();




	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	bool isAnimationEntable() const
	{
		return true;
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
	//virtual void DamageCollision(CharacterController& characon);
	////�ʏ�U���ɓ����������̏���
	//virtual void HitNormalAttack();
	////�q�[���[�̃X�L���ɓ����������̏���
	//virtual void HitHeroSkillAttack();
	////�E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏���
	//virtual void HitFireBall();
	////�E�B�U�[�h�̃t���C���s���[�ɓ����������̏���
	//virtual void HitFlamePillar();

	bool RotationOnly();

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



	ModelRender m_modelRender;

	CharacterController m_charaCon;

};

