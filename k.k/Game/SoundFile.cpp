#include "stdafx.h"
#include "SoundFile.h"

SoundFile::SoundFile()
{
	SetSoundFile();
}

SoundFile::~SoundFile()
{
}

void SoundFile::SetSoundFile()
{
	//�^�C�g��BGM
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/sound/BGM/BGM_Title.wav");
	//�o�g��BGM
	g_soundEngine->ResistWaveFileBank(enSoundName_BattleBGM, "Assets/sound/BGM/BGM_Battle.wav");

	//���̃L���[��
	g_soundEngine->ResistWaveFileBank(enSoundName_StarShine, "Assets/sound/SE/outGame/StarShine.wav");

	//�Q�[���N���A
	g_soundEngine->ResistWaveFileBank(enSoundName_GameClear, "Assets/sound/SE/inGame/GameClear.wav");
	//�Q�[���I�[�o�[
	g_soundEngine->ResistWaveFileBank(enSoundName_GameOver, "Assets/sound/SE/inGame/GameOver.wav");

	//�{�X�o��
	//������
	g_soundEngine->ResistWaveFileBank(enSoundName_FogRemoval, "Assets/sound/SE/inGame/FogRemoval.wav");
	//�S�S�S�S�S�S�S
	g_soundEngine->ResistWaveFileBank(enSoundName_Gogogo, "Assets/sound/SE/inGame/gogogo.wav");

	//�o�g���X�^�[�g
	g_soundEngine->ResistWaveFileBank(enSoundName_BattleStart, "Assets/sound/SE/inGame/BattleStart.wav");


	//���艹
	g_soundEngine->ResistWaveFileBank(enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
	//�I����
	g_soundEngine->ResistWaveFileBank(enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");

	//�_�b�V��
	g_soundEngine->ResistWaveFileBank(enSoundName_Dash, "Assets/sound/SE/player/Dash.wav");
	//�\�[�h�q�b�g
	g_soundEngine->ResistWaveFileBank(enSoundName_Sword_Hit, "Assets/sound/SE/player/Sword_Slash.wav");
	//��q�b�g
	g_soundEngine->ResistWaveFileBank(enSoundName_Wand_Hit, "Assets/sound/SE/player/Wand_Attack1.wav");
	//�N���e�B�J���q�b�g
	g_soundEngine->ResistWaveFileBank(enSoundName_CriticalHit, "Assets/sound/SE/player/CriticalHit.wav");
	//�\�[�h�X�g�[���`���[�W
	g_soundEngine->ResistWaveFileBank(enSoundName_SwordStorm_Charge, "Assets/sound/SE/player/SwordStorm_Charge.wav");
	//�\�[�h�X�g�[��
	g_soundEngine->ResistWaveFileBank(enSoundName_SwordStorm_Storm, "Assets/sound/SE/player/SwordStorm_Storm.wav");
	//�t���C���s���[
	g_soundEngine->ResistWaveFileBank(enSoundName_FlamePillar, "Assets/sound/SE/player/FlamePillar.wav");
	//�t�@�C���[�{�[������
	g_soundEngine->ResistWaveFileBank(enSoundName_FireBall_Start, "Assets/sound/SE/player/FireBall_Start.wav");




	//���u�����X�^�[�̃f�X
	g_soundEngine->ResistWaveFileBank(enSoundName_Mob_Die, "Assets/sound/SE/monster/Mob_Die.wav");
	//�����X�^�[�̍U�����P
	g_soundEngine->ResistWaveFileBank(enSoundName_Attack1, "Assets/sound/SE/monster/Attack1.wav");
	//�{�X�̃��[�v
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");
	//�_�[�N�{�[���̔���
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_DarkBall, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//
	//g_soundEngine->ResistWaveFileBank(enSoundName_Boss_DarkBall_Explosion, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//���e�I�̔���
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Meteo_Explosion, "Assets/sound/SE/monster/Meteo_Explosion.wav");
	//�����O�̉r��
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Summon_charge, "Assets/sound/SE/monster/Summon_Charge.wav");
	//�����̏u��
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Summon_start, "Assets/sound/SE/monster/Summon.wav");





}
