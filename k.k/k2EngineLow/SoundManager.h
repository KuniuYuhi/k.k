#pragma once

namespace nsK2EngineLow {

	enum EnSoundName
	{
		//�A�E�g�Q�[��
		enSoundName_Decision,					//����
		enSoundName_Select,						//�I��
		//BGM
		enSoundName_TitleBGM,
		enSoundName_BattleBGM,
		//�^�C�g���֘A
		enSoundName_StarShine,					//���̃L���[���̂��


		//�Q�[���I��
		enSoundName_GameClear,
		enSoundName_GameOver,
		//SE
		//�v���C���[��
		enSoundName_Sword_Hit,					//�����q�b�g������
		enSoundName_Wand_Hit,					//�񂪃q�b�g������
		enSoundName_CriticalHit,				//�N���e�B�J���q�b�g
		enSoundName_Dash,						//�_�b�V��

		//�����X�^�[��
		enSoundName_Boss_Warp,					//�{�X�̃��[�v

		enSoundName_Num
	};

	class SoundManager:public Noncopyable
	{
	public:
		SoundManager();
		~SoundManager();

		

		void Init();

		/// <summary>
		/// �T�E���h�t�@�C����ǂݍ���
		/// </summary>
		void SetSoundFile();

		/// <summary>
		/// �T�E���h�\�[�X��ǂݍ��݁A�Đ�
		/// </summary>
		/// <param name="soundName">�ǂݍ��݂����T�E���h�̖��O</param>
		void InitAndPlaySoundSource(EnSoundName soundName, bool Sound3DFlag = false, bool roopFlag = false);

		/// <summary>
		/// �T�E���h�\�[�X�����X�g�ɒǉ�
		/// </summary>
		/// <param name="soundSource">�ǉ�����T�E���h�\�[�X</param>
		void AddSoundSourceList(SoundSource* addSoundSource);
		/// <summary>
		/// �T�E���h�\�[�X�����X�g����폜
		/// </summary>
		/// <param name="elaseSoundSource">�폜����T�E���h�\�[�X</param>
		void EraseSoundSourceList(SoundSource* elaseSoundSource);


		void StopSound(EnSoundName soundName);

		/// <summary>
		/// ���X�g�ɓ����Ă���T�E���h���ꎞ��~
		/// </summary>
		void PauseAllSound();

		/// <summary>
		/// ���X�g�ɓ����Ă���T�E���h���v���C
		/// </summary>
		void PlayAllSound();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="volume"></param>
		void SetSoundVolume(float volume);

		//void 

	private:
		std::vector<SoundSource*> m_soundSourceList;

	};

	extern SoundManager* g_soundManager;
}



