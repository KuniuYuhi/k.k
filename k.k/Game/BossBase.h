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
	virtual bool isAnimationEnable() const override = 0;

	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEnable() const override = 0;

	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEnable() const override = 0;

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

	/// <summary>
	/// ���f�������_�[�̎擾
	/// </summary>
	/// <returns></returns>
	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̑����̌v�Z
	/// </summary>
	/// <param name="addOrSub">true�ŉ��Z�Afalse�Ō��Z</param>
	/// <param name="value"></param>
	void CalcSuperArmor(bool addOrSub,float value);
	/// <summary>
	/// �X�[�p�[�A�[�}�[�u���C�N�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetBreakSuperArmorFlag(bool flag)
	{
		m_BreakSuperArmorFlag = flag;
	}
	/// <summary>
	/// �X�[�p�[�A�[�}�[�u���C�N�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetBreakSuperArmorFlag() const
	{
		return m_BreakSuperArmorFlag;
	}

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

	/// <summary>
	/// �X�[�p�[�A�[�}�[�̉��Z
	/// </summary>
	/// <param name="addValue"></param>
	void AddSuperArmorPoint(float addValue);
	/// <summary>
	/// �X�[�p�[�A�[�}�[�̌��Z
	/// </summary>
	/// <param name="addValue"></param>
	void SubSuperArmorPoint(float subValue);

	/// <summary>
	/// ���ނ��ǂ���
	/// </summary>
	/// <returns>true�ŋ���</returns>
	bool IsFlinch();

protected:
	ModelRender m_modelRender;

	float m_superArmorPoint = 0.0f;		//�X�[�p�[�A�[�}�[�̃|�C���g
	float m_maxSuperArmorPoint = 0.0f;	//�X�[�p�[�A�[�}�[�̍ő�l

	bool m_BreakSuperArmorFlag = false; //�X�[�p�[�A�[�}�[�����ꂽ���̃t���O





};

