#include "stdafx.h"
#include "SoundFile.h"

SoundFile::SoundFile()
{
	ResistAllSoundFile();
}

SoundFile::~SoundFile()
{
}

void SoundFile::ResistAllSoundFile()
{
	ResistTitleSoundFile();
	ResistBattleStartSoundFile();
	ResistEntryBossSoundFile();
	ResistBattleSoundFile();
	ResistResultSoundFile();
}

void SoundFile::ResistTitleSoundFile()
{
	//�^�C�g��BGM
	g_soundEngine->ResistWaveFileBank(
		enSoundName_TitleBGM, "Assets/sound/BGM/BGM_Title.wav");
	//���̃L���[��
	g_soundEngine->ResistWaveFileBank(
		enSoundName_StarShine, "Assets/sound/SE/outGame/StarShine.wav");
	//���艹
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
	//�I����
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");
}

void SoundFile::ResistBattleStartSoundFile()
{
	//�o�g���X�^�[�g
	g_soundEngine->ResistWaveFileBank(
		enSoundName_BattleStart, "Assets/sound/SE/inGame/BattleStart2.wav");
}

void SoundFile::ResistEntryBossSoundFile()
{
	//������
	g_soundEngine->ResistWaveFileBank(
		enSoundName_FogRemoval, "Assets/sound/SE/inGame/FogRemoval.wav");
	//�S�S�S�S�S�S�S
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Gogogo, "Assets/sound/SE/inGame/gogogo_2.wav");
}

void SoundFile::ResistBattleSoundFile()
{
	//�o�g��BGM
	g_soundEngine->ResistWaveFileBank(
		enSoundName_BattleBGM, "Assets/sound/BGM/BGM_Battle.wav");

	//�v���C���[�̃T�E���h�t�@�C���ǂݍ���
	ResistPlayerSoundFile();
	//�����X�^�[�̃T�E���h�t�@�C���ǂݍ���
	ResistMonsterSoundFile();
}

void SoundFile::ResistPlayerSoundFile()
{
	//�\�[�h���V�[���h�̃R���{�U���P�C�Q
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldCombo_1_2, 
		"Assets/sound/SE/player/SwordShield/SwordShieldCombo_1_2.wav");
	//�\�[�h���V�[���h�̃R���{�U���R
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldCombo_3,
		"Assets/sound/SE/player/SwordShield/SwordShieldCombo_3.wav");
	//�\�[�h���V�[���h�̃X�L���U��
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldSkillAttack,
		"Assets/sound/SE/player/SwordShield/SwordShieldSkillAttack.wav");

	//�O���C�g�\�[�h�̃R���{�U���P�C�Q
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordCombo_1_2,
		"Assets/sound/SE/player/GreatSword/GreatSwordCombo1_2.wav");
	//�O���C�g�\�[�h�̃W�����v
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordSkillRising,
		"Assets/sound/SE/player/SkillJamp.wav");
	//�O���C�g�\�[�h�̃X�L���U��
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordSkillAttack,
		"Assets/sound/SE/player/GreatSword/GreatSwordSkillAttack.wav");

}

void SoundFile::ResistMonsterSoundFile()
{
	//�{�X�̃T�E���h�t�@�C���ǂݍ���
	ResistBossSoundFile();
	//���u�����X�^�[�̃T�E���h�t�@�C���ǂݍ���
	ResistMobMonsterSoundFile();
}

void SoundFile::ResistBossSoundFile()
{
	//�{�X�̃��[�v
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");
	//�{�X�̓{�胂�[�h
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Angry, "Assets/sound/SE/monster/Angry.wav");
	//�_�[�N�{�[���̔���
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_DarkBall, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//�_�[�N�{�[���̔���
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_DarkBall_Explosion, "Assets/sound/SE/monster/DarkBall_Explosion.wav");
	//���e�I����
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Meteo_Shot, "Assets/sound/SE/monster/Shot_Meteo.wav");
	//���e�I�̔���
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Meteo_Explosion, "Assets/sound/SE/monster/Meteo_Explosion.wav");
	//�����O�̉r��
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Summon_charge, "Assets/sound/SE/monster/Summon_Charge.wav");
	//�����̏u��
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Summon_start, "Assets/sound/SE/monster/Summon.wav");
}

void SoundFile::ResistMobMonsterSoundFile()
{
	//���u�����X�^�[�̃f�X
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Mob_Die, "Assets/sound/SE/monster/Mob_Die.wav");
	//�����X�^�[�̍U�����P
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Attack1, "Assets/sound/SE/monster/Attack1.wav");
	//�X���C���̍U����
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Slime_Attack, "Assets/sound/SE/monster/Slime_Attack.wav");
	//�J�N�^�X�̍U�����P
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Cactus_Attack1, "Assets/sound/SE/monster/Cactus_Attack1.wav");
}

void SoundFile::ResistResultSoundFile()
{
	//�Q�[���N���A
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GameClear, "Assets/sound/SE/inGame/GameClear.wav");
	//�Q�[���I�[�o�[
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GameOver, "Assets/sound/SE/inGame/GameOver.wav");
}
