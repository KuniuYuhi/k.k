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
		//enum�^��int�^�ɕϊ�
		int num = 0;
		num = static_cast<int>(soundName);

		SoundSource* so = NewGO<SoundSource>(0);
		so->Init(soundName, Sound3DFlag, bgmFlag);
		//���W�̐ݒ�
		if (Sound3DFlag == true)
		{
			so->SetPosition(soundPosition);
		}
		so->SetNumber(num);
		so->SetVolume(volume);
		so->Play(roopFlag);
		//���X�g�ɒǉ�
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
		//���X�g�̍Ō�łȂ��Ȃ�
		if (it != m_soundSourceList.end())
		{
			m_soundSourceList.erase(it);
		}

	}

	void SoundManager::StopSound(EnSoundName soundName)
	{
		//���O����
		//enum�^��int�^�ɕϊ�
		int num = 0;
		num = static_cast<int>(soundName);
		//�����ԍ���T����stop����
		for (auto nowSoundSouce : m_soundSourceList)
		{
			if (nowSoundSouce->GetNumber() == num)
			{
				//�Đ����Ȃ����
				if (nowSoundSouce->IsPlaying() == true)
				{
					nowSoundSouce->Release();
				}
			}
		}
	}

	void SoundManager::StopAllSound()
	{
		//���X�g�ɓ����Ă���T�E���h�\�[�X���~�߂�
		for (auto nowSoundSouce : m_soundSourceList)
		{
			//�Đ����Ȃ����
			if (nowSoundSouce->IsPlaying() == true)
			{
				nowSoundSouce->Release();
			}
		}
	}

	void SoundManager::PauseAllSound()
	{
		//���X�g�ɓ����Ă���T�E���h�\�[�X���ꎞ��~
		for (auto nowSoundSouce : m_soundSourceList)
		{
			nowSoundSouce->Pause();
		}
	}

	void SoundManager::PlayAllSound()
	{
		//���X�g�ɓ����Ă���T�E���h�\�[�X���v���C
		for (auto nowSoundSouce : m_soundSourceList)
		{
			nowSoundSouce->Play(nowSoundSouce->GetLoopFlag());
		}
	}

	SoundSource* SoundManager::GetSoundSource(EnSoundName soundName)
	{
		//���O����
		//enum�^��int�^�ɕϊ�
		int num = 0;
		num = static_cast<int>(soundName);
		//�����ԍ���T��
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


