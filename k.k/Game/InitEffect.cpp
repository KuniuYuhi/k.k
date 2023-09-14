#include "stdafx.h"
#include "InitEffect.h"

InitEffect::InitEffect()
{
}

InitEffect::~InitEffect()
{
}

void InitEffect::InitEFK()
{
	//�{�X�̖��@�w�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Boss_Summon_Circle, u"Assets/effect/sprite/Boss_Summon_circle.efk");
	//���u�����X�^�[�̖��@�w�̃��C�g�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Boss_Summon_FogRemoval, u"Assets/effect/sprite/Boss_Summon_wind.efk");
	//���u�����X�^�[�̖��@�w�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Summon_Circle, u"Assets/effect/sprite/Summon_circle.efk");
	//���u�����X�^�[�̖��@�w�̃��C�g�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Summon_Right, u"Assets/effect/sprite/Summon_right.efk");
	//�_�[�N�{�[���̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkBall, u"Assets/effect/sprite/DarkBall2.efk");
	//�_�[�N�{�[���̔����̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkBall_Explosion, u"Assets/effect/sprite/DarkBall_Explosion.efk");
	//�_�[�N���e�I�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkMeteorite, u"Assets/effect/sprite/DarkMeteorite.efk");
	//�_�[�N���e�I�̕��̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_DarkMeteorite_Wind, u"Assets/effect/sprite/DarkMeteorite_wind.efk");
	//���e�I�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Meteo, u"Assets/effect/sprite/Meteo.efk");
	//���e�I�̔����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Meteo_Explosion, u"Assets/effect/sprite/Meteo_Explosion.efk");




}
