#pragma once
#include "AIActor.h"

class BossBase:public AIActor
{
public:
	BossBase();
	virtual ~BossBase();



	/////////////////////////////////////////////////////////////////////////////
	// ���z�֐��A�������z�֐��̐錾
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack() override{}
	/// <summary>
	/// �������~�߂邩
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing() override { return false; }

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const override = 0;

	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const override = 0;

	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const override = 0;

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) override = 0;



	/////////////////////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	/////////////////////////////////////////////////////////////////////////////



protected:



};

