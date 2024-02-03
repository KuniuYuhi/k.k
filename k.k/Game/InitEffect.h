#pragma once
class InitEffect
{
public:
	InitEffect();
	~InitEffect();

	/// <summary>
	/// �e��G�t�F�N�g�̏�����
	/// </summary>
	void InitEFK();

	/// <summary>
	/// �G�t�F�N�g�ԍ�
	/// </summary>
	enum EnEFK
	{
		enEffect_Boss_Summon_Circle,							//�{�X�̏����w
		enEffect_Boss_Summon_FogRemoval,						//�{�X�̖�����
		enEffect_Mob_Summon_Circle,								//���u�����X�^�[�̏����w
		enEffect_Mob_Summon_Right,								//�����X�^�[�̏������ꂽ�u�Ԃ̃��C�g
		enEffect_DarkBall,										//�_�[�N�{�[��
		enEffect_DarkBall_Explosion,							//�_�[�N�{�[���̔���
		enEffect_DarkWall,										//�_�[�N�E�H�[��
		enEffect_DarkSpear,										//�_�[�N�X�s�A
		enEffect_DarkMeteorite,									//�_�[�N���e�I
		enEffect_DarkMeteorite_Wind,							//�_�[�N���e�I�̕�
		enEffect_Meteo,											//���e�I
		enEffect_Meteo_Explosion,								//���e�I�̔���
		enEffect_Meteo_Range,									//���e�I�͈̔�
		enEffect_Mob_Dead,										//���u�����X�^�[�����ꂽ�Ƃ��̃G�t�F�N�g
		enEffect_Invalibation,									//�_���[�W�������̂Ƃ��̃G�t�F�N�g
		enEffect_Boss_Combo_1,									//�{�X�̒ʏ�U���P
		enEffect_Boss_Combo_2,									//�{�X�̒ʏ�U���Q
		enEffect_ComboFinishExplosion,							//�{�X�̒ʏ�U���R�̔���
		enEffect_Boss_KnockBack,								//�{�X�̃m�b�N�o�b�N�U��

		enEffect_Dash,											//�_�b�V��
		enEffect_Hit,											//�q�b�g
		enEffect_SwordStorm_Charge,								//�\�[�h�X�g�[���̃`���[�W
		
		enEffect_BraveLanding,									//�E�ғo�ꎞ�̒��n

		enEffect_SwordShieldCombo12,							//�������̒ʏ�U���P�ƂQ
		enEffect_SwordShieldCombo3,								//�������̒ʏ�U���R
		enEffect_SwordShieldSkillRising,						//�������̃X�L���̏㏸
		enEffect_SwordShieldSkillAttack,						//�������̃X�L���U��
		enEffect_SwordShieldDefendHit,							//�������̏��h��q�b�g
		enEffect_GreatSwordCombo12,								//���茕�̒ʏ�U���P�ƂQ
		enEffect_GreatSwordCombo3,								//���茕�̒ʏ�U���R
		enEffect_GreatSwordSkillRising,							//���茕�̃X�L���̏㏸
		enEffect_GreatSwordSkillAttack,							//���茕�̃X�L���U��
		enEffect_BowArrowCombo,									//�|����̒ʏ�U��
		enEffect_Arrow,											//��̃G�t�F�N�g
		enEffect_BowArrowSkillShot,								//�|����̃X�L���U��




	};


private:
	/// <summary>
	/// �v���C���[�Ɋւ���G�t�F�N�g������
	/// </summary>
	void InitPlayerEffect();
	/// <summary>
	/// �����X�^�[�Ɋւ���G�t�F�N�g������
	/// </summary>
	void InitMonsterEffect();
	/// <summary>
	/// �{�X�����X�^�[�Ɋւ���G�t�F�N�g������
	/// </summary>
	void InitBossMonsterEffect();
	/// <summary>
	/// ���u�����X�^�[�Ɋւ���G�t�F�N�g������
	/// </summary>
	void InitMobMonsterEffect();

};



