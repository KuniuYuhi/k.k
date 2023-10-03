#include "k2EngineLowPreCompile.h"
#include "SoundManager.h"
#include <string>

namespace nsK2EngineLow
{
	SoundManager* g_soundManager = nullptr;


	SoundManager::SoundManager()
	{
		SetSoundFile();
	}

	SoundManager::~SoundManager()
	{
	}

	void SoundManager::Init()
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
		g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Attack1.wav");
		//ボスのワープ
		g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");



	}

	void SoundManager::InitAndPlaySoundSource(EnSoundName soundName, bool Sound3DFlag, bool roopFlag)
	{
		//enum型をint型に変換
		int num = 0;
		num = static_cast<int>(soundName);

		SoundSource* so = NewGO<SoundSource>(0);
		so->Init(soundName, Sound3DFlag);
		so->SetNumber(num);
		so->SetVolume(5.0f);
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
				nowSoundSouce->Release();

				////ループフラグがtrueなら明示的に削除
				//if (nowSoundSouce->GetLoopFlag() == true)
				//{
				//	nowSoundSouce->Release();
				//}
				//else
				//{
				//	nowSoundSouce->Stop();
				//}
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



	void SoundManager::SetSoundVolume(float volume)
	{
	}
}


