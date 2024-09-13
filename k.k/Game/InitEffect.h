#pragma once

#include "EffectNumbers.h"

using namespace EffectNumbers;

class InitEffect
{
public:
	InitEffect();
	~InitEffect();

	/// <summary>
	/// �e��G�t�F�N�g�̏�����
	/// </summary>
	void InitEFK();



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



