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
		enEffect_DarkMeteorite,									//�_�[�N���e�I
		enEffect_DarkMeteorite_Wind,							//�_�[�N���e�I�̕�
		enEffect_Meteo,											//���e�I
		enEffect_Meteo_Explosion,								//���e�I�̔���

	};
};



