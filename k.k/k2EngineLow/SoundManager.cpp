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
		//�^�C�g��BGM
		g_soundEngine->ResistWaveFileBank(enSoundName_TitleBGM, "Assets/sound/BGM/BGM_Title.wav");
		//�o�g��BGM
		g_soundEngine->ResistWaveFileBank(enSoundName_BattleBGM, "Assets/sound/BGM/BGM_Battle.wav");

		//���̃L���[��
		g_soundEngine->ResistWaveFileBank(enSoundName_StarShine, "Assets/sound/SE/outGame/StarShine.wav");

		//�Q�[���N���A
		g_soundEngine->ResistWaveFileBank(enSoundName_GameClear, "Assets/sound/SE/inGame/GameClear.wav");
		//�Q�[���I�[�o�[

		//���艹
		g_soundEngine->ResistWaveFileBank(enSoundName_Decision, "Assets/sound/SE/outGame/Decision.wav");
		//�I����
		g_soundEngine->ResistWaveFileBank(enSoundName_Select, "Assets/sound/SE/outGame/Select.wav");

		//�_�b�V��
		g_soundEngine->ResistWaveFileBank(enSoundName_Dash, "Assets/sound/SE/player/Dash.wav");
		//�\�[�h�q�b�g
		g_soundEngine->ResistWaveFileBank(enSoundName_Sword_Hit, "Assets/sound/SE/player/Sword_Slash.wav");
		//�N���e�B�J���q�b�g
		g_soundEngine->ResistWaveFileBank(enSoundName_CriticalHit, "Assets/sound/SE/player/CriticalHit.wav");


		//�����X�^�[�̍U�����P
		g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Attack1.wav");
		//�{�X�̃��[�v
		g_soundEngine->ResistWaveFileBank(enSoundName_Boss_Warp, "Assets/sound/SE/monster/Warp.wav");



	}

	void SoundManager::InitAndPlaySoundSource(EnSoundName soundName, bool Sound3DFlag, bool roopFlag)
	{
		//enum�^��int�^�ɕϊ�
		int num = 0;
		num = static_cast<int>(soundName);

		SoundSource* so = NewGO<SoundSource>(0);
		so->Init(soundName, Sound3DFlag);
		so->SetNumber(num);
		so->SetVolume(5.0f);
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
				nowSoundSouce->Release();

				////���[�v�t���O��true�Ȃ疾���I�ɍ폜
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



	void SoundManager::SetSoundVolume(float volume)
	{
	}
}

