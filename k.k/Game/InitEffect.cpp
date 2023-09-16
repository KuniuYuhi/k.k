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
	//���u�����X�^�[�̂���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Mob_Dead, u"Assets/effect/sprite/Mob_Dead.efk");
	//�_���[�W�����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Invalibation, u"Assets/effect/sprite/Invalibation.efk");
	//�_�b�V���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Dash, u"Assets/effect/sprite/Dash.efk");
	//�q�b�g�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_Hit, u"Assets/effect/sprite/hit.efk");
	//�t�@�C���[�{�[���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FireBall, u"Assets/effect/sprite/FireBall.efk");
	//�t�@�C���[�{�[���̔����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FireBall_Explosion, u"Assets/effect/sprite/FireBall_Explosion.efk");
	//�t���C���s���[�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FlamePillar, u"Assets/effect/sprite/FlamePillar.efk");
	//�t���C���s���[�̖��@�w�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(EnEFK::enEffect_FlamePillar_Circle, u"Assets/effect/sprite/FlamePillar_Circle.efk");


}
