#pragma once
class InitEffect
{
public:
	InitEffect();
	~InitEffect();

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
		enEffect_Dash,											//�_�b�V��
		enEffect_Hit,											//�q�b�g
		enEffect_FireBall,										//�t�@�C���[�{�[��
		enEffect_FireBall_Explosion,							//�t�@�C���[�{�[���̔���
		enEffect_FlamePillar,									//�t���C���s���[
		enEffect_FlamePillar_Circle,							//�t���C���s���[�̖��@�w
		enEffect_SwordStorm,									//�\�[�h�X�g�[��
		enEffect_SwordStorm_Charge,								//�\�[�h�X�g�[���̃`���[�W
		

	};
};



