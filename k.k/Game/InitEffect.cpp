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
	//�v���C���[�Ɋւ���G�t�F�N�g��������
	InitPlayerEffect();
	//�����X�^�[�Ɋւ���G�t�F�N�g��������
	InitMonsterEffect();
}

void InitEffect::InitPlayerEffect()
{
	//�E�҂̒��n�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BraveLanding,
		u"Assets/effect/sprite/BraveLanding.efk"
	);
	//�_�b�V���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Dash, 
		u"Assets/effect/sprite/Dash.efk"
	);
	//	�|�̃`���[�W�G�t�F�N�g�P��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ArrowCharge1, 
		u"Assets/effect/sprite/SwordStorm_Charge.efk"
	);
	//	�|�̃`���[�W�G�t�F�N�g�Q��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ArrowCharge2,
		u"Assets/effect/sprite/Charge2.efk"
	);

	//�������̒ʏ�U���P�A�Q�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldCombo12,
		u"Assets/effect/sprite/SwordShieldComboSlash.efk"
	);
	//�������̒ʏ�U���R�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldCombo3,
		u"Assets/effect/sprite/SwordShieldComboThrust.efk"
	);
	//�������̏��o���A�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldDefendBarrier,
		u"Assets/effect/sprite/ShieldBarrier.efk"
	);
	//�������̃X�L���㏸�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldSkillRising,
		u"Assets/effect/sprite/SwordShieldSkillRising.efk"
	);
	//�������̃X�L���U���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_SwordShieldSkillAttack,
		u"Assets/effect/sprite/SwordShieldSkillAttack.efk"
	);


	//���茕�̒ʏ�U���P�A�Q�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo1,
		u"Assets/effect/sprite/GreatSwordComboSlash.efk"
	);
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo2,
		u"Assets/effect/sprite/GreatSwordComboSlashSlow.efk"
	);
	//���茕�̒ʏ�U���R�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordCombo3,
		u"Assets/effect/sprite/GreatSwordComboThrust.efk"
	);
	//���茕�̃X�L���㏸�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordSkillRising,
		u"Assets/effect/sprite/GreatSwordSkillRising.efk"
	);
	//���茕�̃X�L���U���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_GreatSwordSkillAttack,
		u"Assets/effect/sprite/GreatSwordSkillAttack.efk"
	);

	//�|����̒ʏ�U���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BowArrowCombo,
		u"Assets/effect/sprite/BowArrowCombo.efk"
	);
	//��̍U���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Arrow,
		u"Assets/effect/sprite/Arrow.efk"
	);
	//�|����̃X�L���U���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_BowArrowSkillShot,
		u"Assets/effect/sprite/BowArrowSkillShot.efk"
	);
	


	//�U���̓A�b�v�J�n�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_AttackUpStart,
		u"Assets/effect/sprite/AttackUpStart.efk"
	);

	//�U���̓A�b�v�G�t�F�N�g�V���{���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_AttackEffectSynbol,
		u"Assets/effect/sprite/AttackEffectSymbol.efk"
	);

	//�񕜊J�n�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_RecoveryStart,
		u"Assets/effect/sprite/RecoveryEffect.efk"
	);

	//�񕜃G�t�F�N�g�V���{���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_RecoveryEffectSynbol,
		u"Assets/effect/sprite/RecoveryEffectSymbol.efk"
	);
}

void InitEffect::InitMonsterEffect()
{
	//�����X�^�[�̂���G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Dead, 
		u"Assets/effect/sprite/Mob_Dead.efk"
	);

	//�q�b�g�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Hit,
		u"Assets/effect/sprite/hit.efk"
	);

	//���u�����X�^�[�Ɋւ���G�t�F�N�g��������
	InitMobMonsterEffect();
	//�{�X�Ɋւ���G�t�F�N�g��������
	InitBossMonsterEffect();
}

void InitEffect::InitBossMonsterEffect()
{
	//�{�X�̖��@�w�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Summon_Circle, 
		u"Assets/effect/sprite/Boss_Summon_circle.efk"
	);
	//�_�[�N�{�[���̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkBall, 
		u"Assets/effect/sprite/DarkBall2.efk"
	);
	//�_�[�N�{�[���̔����̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkBall_Explosion, 
		u"Assets/effect/sprite/DarkBall_Explosion.efk"
	);
	//�_�[�N���e�I�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkMeteorite, 
		u"Assets/effect/sprite/DarkMeteorite.efk"
	);
	//�_�[�N���e�I�̕��̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkMeteorite_Wind, 
		u"Assets/effect/sprite/DarkMeteorite_wind.efk"
	);
	//���e�I�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo, 
		u"Assets/effect/sprite/Meteo.efk"
	);
	//���e�I�̔����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo_Explosion, 
		u"Assets/effect/sprite/Meteo_Explosion.efk"
	);
	//���e�I�͈̔̓G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Meteo_Range, 
		u"Assets/effect/sprite/Range.efk"
	);
	//�_�[�N�X�s�A�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_DarkSpear,
		u"Assets/effect/sprite/DarkSpear.efk"
	);
	//�ʏ�U���P�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Combo_1,
		u"Assets/effect/sprite/Summoner_Slash.efk"
	);
	//�ʏ�U��2�̃G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Combo_2,
		u"Assets/effect/sprite/Boss_Combo_2.efk"
	);
	//�ʏ�U���R�̔����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_ComboFinishExplosion,
		u"Assets/effect/sprite/ComboFinishExplosion.efk"
	);
	//�m�b�N�o�b�N�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_KnockBack,
		u"Assets/effect/sprite/Boss_KnockBack.efk"
	);

	//�_���[�W�����G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Invalibation, 
		u"Assets/effect/sprite/Invalibation.efk"
	);

	//���[�v���@�w�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_WarpSircle,
		u"Assets/effect/sprite/Summoner_WarpSircle.efk"
	);

	//���[�v�̕��G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_WarpWind,
		u"Assets/effect/sprite/Summoner_Warp.efk"
	);
}

void InitEffect::InitMobMonsterEffect()
{
	//���u�����X�^�[�̖��@�w�̃��C�g�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Boss_Summon_FogRemoval, 
		u"Assets/effect/sprite/Boss_Summon_wind.efk"
	);
	//���u�����X�^�[�̖��@�w�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Summon_Circle, 
		u"Assets/effect/sprite/Summon_circle.efk"
	);
	//���u�����X�^�[�̖��@�w�̃��C�g�̃G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_Mob_Summon_Right, 
		u"Assets/effect/sprite/Summon_right.efk"
	);

	//���u�G�l�~�[�̃{�[���G�t�F�N�g�ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(
		enEffect_EyeBall,
		u"Assets/effect/sprite/miniBall.efk"
	);
	
}
