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
	//タイトルBGM
	g_soundEngine->ResistWaveFileBank(
		enSoundName_TitleBGM, "Assets/sound/BGM/BGM_Title.wav");
	//剣のキラーン
	g_soundEngine->ResistWaveFileBank(
		enSoundName_StarShine, "Assets/sound/SE/outGame/StarShine.wav");
	//決定音
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
	//選択音
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");
}

void SoundFile::ResistBattleStartSoundFile()
{
	//バトルスタート
	g_soundEngine->ResistWaveFileBank(
		enSoundName_BattleStart, "Assets/sound/SE/inGame/BattleStart2.wav");
}

void SoundFile::ResistEntryBossSoundFile()
{
	//霧払い
	g_soundEngine->ResistWaveFileBank(
		enSoundName_FogRemoval, "Assets/sound/SE/inGame/FogRemoval.wav");
	//ゴゴゴゴゴゴゴ
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Gogogo, "Assets/sound/SE/inGame/gogogo_2.wav");
}

void SoundFile::ResistBattleSoundFile()
{
	//バトルBGM
	g_soundEngine->ResistWaveFileBank(
		enSoundName_BattleBGM, "Assets/sound/BGM/BGM_Battle.wav");

	//プレイヤーのサウンドファイル読み込み
	ResistPlayerSoundFile();
	//モンスターのサウンドファイル読み込み
	ResistMonsterSoundFile();
}

void SoundFile::ResistPlayerSoundFile()
{
	//ソード＆シールドのコンボ攻撃１，２
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldCombo_1_2, 
		"Assets/sound/SE/player/SwordShield/SwordShieldCombo_1_2.wav");
	//ソード＆シールドのコンボ攻撃３
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldCombo_3,
		"Assets/sound/SE/player/SwordShield/SwordShieldCombo_3.wav");
	//ソード＆シールドのスキル攻撃
	g_soundEngine->ResistWaveFileBank(
		enSoundName_SwordShieldSkillAttack,
		"Assets/sound/SE/player/SwordShield/SwordShieldSkillAttack.wav");

	//グレイトソードのコンボ攻撃１，２
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordCombo_1_2,
		"Assets/sound/SE/player/GreatSword/GreatSwordCombo1_2.wav");
	//グレイトソードのジャンプ
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordSkillRising,
		"Assets/sound/SE/player/SkillJamp.wav");
	//グレイトソードのスキル攻撃
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GreatSwordSkillAttack,
		"Assets/sound/SE/player/GreatSword/GreatSwordSkillAttack.wav");

}

void SoundFile::ResistMonsterSoundFile()
{
	//ボスのサウンドファイル読み込み
	ResistBossSoundFile();
	//モブモンスターのサウンドファイル読み込み
	ResistMobMonsterSoundFile();
}

void SoundFile::ResistBossSoundFile()
{
	//ボスのワープ
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");
	//ボスの怒りモード
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Angry, "Assets/sound/SE/monster/Angry.wav");
	//ダークボールの発生
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_DarkBall, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//ダークボールの爆発
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_DarkBall_Explosion, "Assets/sound/SE/monster/DarkBall_Explosion.wav");
	//メテオ撃つ
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Meteo_Shot, "Assets/sound/SE/monster/Shot_Meteo.wav");
	//メテオの爆発
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Meteo_Explosion, "Assets/sound/SE/monster/Meteo_Explosion.wav");
	//召喚前の詠唱
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Summon_charge, "Assets/sound/SE/monster/Summon_Charge.wav");
	//召喚の瞬間
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Boss_Summon_start, "Assets/sound/SE/monster/Summon.wav");
}

void SoundFile::ResistMobMonsterSoundFile()
{
	//モブモンスターのデス
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Mob_Die, "Assets/sound/SE/monster/Mob_Die.wav");
	//モンスターの攻撃音１
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Attack1, "Assets/sound/SE/monster/Attack1.wav");
	//スライムの攻撃音
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Slime_Attack, "Assets/sound/SE/monster/Slime_Attack.wav");
	//カクタスの攻撃音１
	g_soundEngine->ResistWaveFileBank(
		enSoundName_Cactus_Attack1, "Assets/sound/SE/monster/Cactus_Attack1.wav");
}

void SoundFile::ResistResultSoundFile()
{
	//ゲームクリア
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GameClear, "Assets/sound/SE/inGame/GameClear.wav");
	//ゲームオーバー
	g_soundEngine->ResistWaveFileBank(
		enSoundName_GameOver, "Assets/sound/SE/inGame/GameOver.wav");
}
