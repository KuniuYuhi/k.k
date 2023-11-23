#pragma once
#include "MonsterBase.h"

class BossBase:public MonsterBase
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

	//�ʏ�U���ɓ����������̏���
	virtual void HitNormalAttack() override;

	//�q�[���[�̃X�L���ɓ����������̏���
	virtual void HitSkillAttack() override;
	/// <summary>
	/// �E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	virtual void HitFireBall() override {}
	/// <summary>
	/// �E�B�U�[�h�̃t���C���s���[�ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	virtual void HitFlamePillar(bool damageFlag = false) override {}
	
	/// <summary>
	/// ��]�݂̂��s����������
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() override = 0;

	/////////////////////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	/////////////////////////////////////////////////////////////////////////////



protected:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() override = 0;

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() override = 0;

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	virtual void ManageState() override = 0;


};

