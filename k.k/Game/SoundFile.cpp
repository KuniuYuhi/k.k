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
	//タイトルBGM
	g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/sound/BGM/BGM_Title.wav");
	//バトルBGM
	g_soundEngine->ResistWaveFileBank(enSoundName_BattleBGM, "Assets/sound/BGM/BGM_Battle.wav");

	//剣のキラーン
	g_soundEngine->ResistWaveFileBank(enSoundName_StarShine, "Assets/sound/SE/outGame/StarShine.wav");

	//ゲームクリア
	g_soundEngine->ResistWaveFileBank(enSoundName_GameClear, "Assets/sound/SE/inGame/GameClear.wav");
	//ゲームオーバー
	g_soundEngine->ResistWaveFileBank(enSoundName_GameOver, "Assets/sound/SE/inGame/GameOver.wav");

	//ボス登場
	//霧払い
	g_soundEngine->ResistWaveFileBank(enSoundName_FogRemoval, "Assets/sound/SE/inGame/FogRemoval.wav");
	//ゴゴゴゴゴゴゴ
	g_soundEngine->ResistWaveFileBank(enSoundName_Gogogo, "Assets/sound/SE/inGame/gogogo.wav");

	//バトルスタート
	g_soundEngine->ResistWaveFileBank(enSoundName_BattleStart, "Assets/sound/SE/inGame/BattleStart.wav");


	//決定音
	g_soundEngine->ResistWaveFileBank(enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
	//選択音
	g_soundEngine->ResistWaveFileBank(enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");

	//ダッシュ
	g_soundEngine->ResistWaveFileBank(enSoundName_Dash, "Assets/sound/SE/player/Dash.wav");
	//ソードヒット
	g_soundEngine->ResistWaveFileBank(enSoundName_Sword_Hit, "Assets/sound/SE/player/Sword_Slash.wav");
	//杖ヒット
	g_soundEngine->ResistWaveFileBank(enSoundName_Wand_Hit, "Assets/sound/SE/player/Wand_Attack1.wav");
	//クリティカルヒット
	g_soundEngine->ResistWaveFileBank(enSoundName_CriticalHit, "Assets/sound/SE/player/CriticalHit.wav");
	//ソードストームチャージ
	g_soundEngine->ResistWaveFileBank(enSoundName_SwordStorm_Charge, "Assets/sound/SE/player/SwordStorm_Charge.wav");
	//ソードストーム
	g_soundEngine->ResistWaveFileBank(enSoundName_SwordStorm_Storm, "Assets/sound/SE/player/SwordStorm_Storm.wav");
	//フレイムピラー
	g_soundEngine->ResistWaveFileBank(enSoundName_FlamePillar, "Assets/sound/SE/player/FlamePillar.wav");
	//ファイヤーボール発生
	g_soundEngine->ResistWaveFileBank(enSoundName_FireBall_Start, "Assets/sound/SE/player/FireBall_Start.wav");




	//モブモンスターのデス
	g_soundEngine->ResistWaveFileBank(enSoundName_Mob_Die, "Assets/sound/SE/monster/Mob_Die.wav");
	//モンスターの攻撃音１
	g_soundEngine->ResistWaveFileBank(enSoundName_Attack1, "Assets/sound/SE/monster/Attack1.wav");
	//ボスのワープ
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");
	//ダークボールの発生
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_DarkBall, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//
	//g_soundEngine->ResistWaveFileBank(enSoundName_Boss_DarkBall_Explosion, "Assets/sound/SE/monster/DarkBall_Start.wav");
	//メテオの爆発
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Meteo_Explosion, "Assets/sound/SE/monster/Meteo_Explosion.wav");
	//召喚前の詠唱
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Summon_charge, "Assets/sound/SE/monster/Summon_Charge.wav");
	//召喚の瞬間
	g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Summon_start, "Assets/sound/SE/monster/Summon.wav");





}
