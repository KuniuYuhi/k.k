#include "k2EngineLowPreCompile.h"
#include "SoundManager.h"
#include <string>

namespace nsK2EngineLow
{
	SoundManager* g_soundManager = nullptr;


	SoundManager::SoundManager()
	{
		//SetSoundFile();
	}

	SoundManager::~SoundManager()
	{
	}

	void SoundManager::SetSoundFile()
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


		//決定音
		g_soundEngine->ResistWaveFileBank(enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
		//選択音
		g_soundEngine->ResistWaveFileBank(enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");

		//ダッシュ
		g_soundEngine->ResistWaveFileBank(enSoundName_Dash, "Assets/sound/SE/player/Dash.wav");
		//ソードヒット
		g_soundEngine->ResistWaveFileBank(enSoundName_Sword_Hit, "Assets/sound/SE/player/Sword_Slash.wav");
		//クリティカルヒット
		g_soundEngine->ResistWaveFileBank(enSoundName_CriticalHit, "Assets/sound/SE/player/CriticalHit.wav");


		//モンスターの攻撃音１
		g_soundEngine->ResistWaveFileBank(enSoundName_Attack1, "Assets/sound/SE/monster/Attack1.wav");
		//ボスのワープ
		g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");



	}

	void SoundManager::InitAndPlaySoundSource(
		EnSoundName soundName, float volume, bool Sound3DFlag, bool roopFlag, bool bgmFlag, Vector3 soundPosition)
	{
		//enum型をint型に変換
		int num = 0;
		num = static_cast<int>(soundName);

		SoundSource* so = NewGO<SoundSource>(0);
		so->Init(soundName, Sound3DFlag, bgmFlag);
		//座標の設定
		if (Sound3DFlag == true)
		{
			so->SetPosition(soundPosition);
		}
		so->SetNumber(num);
		so->SetVolume(volume);
		so->Play(roopFlag);
		//リストに追加
		AddSoundSourceList(so);
	}

	void SoundManager::AddSoundSourceList(SoundSource* addSoundSource)
	{
		m_soundSourceList.emplace_back(addSoundSource);
	}

	void SoundManager::EraseSoundSourceList(SoundSource* elaseSoundSource)
	{
		std::vector<SoundSource*>::iterator it = std::find(
			m_soundSourceList.begin(),
			m_soundSourceList.end(),
			elaseSoundSource
		);
		//リストの最後でないなら
		if (it != m_soundSourceList.end())
		{
			m_soundSourceList.erase(it);
		}

	}

	void SoundManager::StopSound(EnSoundName soundName)
	{
		//名前検索
		//enum型をint型に変換
		int num = 0;
		num = static_cast<int>(soundName);
		//同じ番号を探してstopする
		for (auto nowSoundSouce : m_soundSourceList)
		{
			if (nowSoundSouce->GetNumber() == num)
			{
				//再生中なら消す
				if (nowSoundSouce->IsPlaying() == true)
				{
					nowSoundSouce->Release();
				}
			}
		}
	}

	void SoundManager::StopAllSound()
	{
		//リストに入っているサウンドソースを止める
		for (auto nowSoundSouce : m_soundSourceList)
		{
			//再生中なら消す
			if (nowSoundSouce->IsPlaying() == true)
			{
				nowSoundSouce->Release();
			}
		}
	}

	void SoundManager::PauseAllSound()
	{
		//リストに入っているサウンドソースを一時停止
		for (auto nowSoundSouce : m_soundSourceList)
		{
			nowSoundSouce->Pause();
		}
	}

	void SoundManager::PlayAllSound()
	{
		//リストに入っているサウンドソースをプレイ
		for (auto nowSoundSouce : m_soundSourceList)
		{
			nowSoundSouce->Play(nowSoundSouce->GetLoopFlag());
		}
	}

	SoundSource* SoundManager::GetSoundSource(EnSoundName soundName)
	{
		//名前検索
		//enum型をint型に変換
		int num = 0;
		num = static_cast<int>(soundName);
		//同じ番号を探す
		for (auto nowSoundSouce : m_soundSourceList)
		{
			if (nowSoundSouce->GetNumber() == num)
			{
				return nowSoundSouce;
			}
		}
	}

	void SoundManager::SetSoundVolume(float mulVolume, bool bgmFlag)
	{
		if (bgmFlag == true)
		{
			m_bgmVolume += mulVolume;
		}
		else
		{
			m_seVolume += mulVolume;
		}

		for (auto nowSoundSouce : m_soundSourceList)
		{
			if (bgmFlag == nowSoundSouce->GetBGMFlag())
			{
				float vol;
				vol = nowSoundSouce->GetVolume();
				vol += mulVolume;
				nowSoundSouce->SetVolume(vol);
			}
		}
	}

	void SoundManager::MakeAllSoundDefaultVolume()
	{
		for (auto nowSoundSouce : m_soundSourceList)
		{
			if (nowSoundSouce->GetBGMFlag() == true)
			{
				nowSoundSouce->SetVolume(GetDefaultBGVolume());
			}
			else
			{
				nowSoundSouce->SetVolume(GetDefaultSEVolume());
			}
			
		}
	}
}


