#include "k2EngineLowPreCompile.h"
#include "SoundManager.h"
#include <string>

namespace nsK2EngineLow
{
	SoundManager* g_soundManager = nullptr;


	SoundManager::SoundManager()
	{
	}

	SoundManager::~SoundManager()
	{
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

		return nullptr;
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


