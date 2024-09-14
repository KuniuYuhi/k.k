#pragma once

namespace nsK2EngineLow {

	//todo �����ԍ��̃T�E���h�\�[�X����������Ɩ]�񂾃T�E���h�\�[�X���擾�ł��Ȃ�

	/// <summary>
	/// �T�E���h�̖��O
	/// </summary>
	enum EnSoundName
	{
		////////////////////////////////////////////////////////////////
		//BGM
		////////////////////////////////////////////////////////////////
		enSoundName_TitleBGM,
		enSoundName_BattleBGM,
		////////////////////////////////////////////////////////////////
		//�^�C�g��
		////////////////////////////////////////////////////////////////
		enSoundName_Decision,					//����
		enSoundName_Select,						//�I��
		enSoundName_StarShine,					//���̃L���[���̂��
		enSoundName_DaahCliff,					//�_�b�V��
		enSoundName_JampCliff,					//�W�����v
		////////////////////////////////////////////////////////////////
		//�E�ғo��
		////////////////////////////////////////////////////////////////
		enSoundName_Fall,						//������
		enSoundName_Landing,					//���n
		////////////////////////////////////////////////////////////////
		//�{�X�o��
		////////////////////////////////////////////////////////////////
		enSoundName_Gogogo,						//�S�S�S�S�S�S�S
		enSoundName_FogRemoval,					//������
		enSoundName_BossNameDon,				//�{�X�̖��O�o���Ƃ��̃h��
		////////////////////////////////////////////////////////////////
		//���U���g
		////////////////////////////////////////////////////////////////
		enSoundName_GameClear,					//�Q�[���N���A
		enSoundName_GameOver,					//�Q�[���I�[�o�[

		//SE
		 
		////////////////////////////////////////////////////////////////
		//�v���C���[
		////////////////////////////////////////////////////////////////
		enSoundName_ChangeWeapon,				//����̐؂�ւ�
		//�\�[�h���V�[���h�̉�
		enSoundName_SwordShieldCombo_1_2,		//
		enSoundName_SwordShieldCombo_3,			//
		enSoundName_SwordShieldSkillRising,		//
		enSoundName_SwordShieldSkillAttack,		//
		//�V�[���h�̉�
		enSoundName_SwordShieldDefendHit,		//
		
		//���茕�̉�
		enSoundName_GreatSwordCombo_1_2,		//
		enSoundName_GreatSwordCombo_3,			//
		enSoundName_GreatSwordSkillRising,		//
		enSoundName_GreatSwordSkillAttack,		//

		//�{�E���A���[�̉�
		enSoundName_BowArrowNormalShot,			//
		enSoundName_BowArrowSkillCharge,		//
		enSoundName_BowArrowSkillAttack,		//


		//�����X�^�[��
		enSoundName_Monster_Hit,				//��_���[�W
		enSoundName_Monster_DieHit,				//���S�m��q�b�g
		enSoundName_Attack1,					//��{���u�����X�^�[�̍U���P
		enSoundName_Slime_Attack,				//�X���C���̍U��
		enSoundName_Cactus_Attack1,				//�J�N�^�X�̍U��1
		enSoundName_Mob_Die,					//���u�����X�^�[�̎��S

		enSoundName_Boss_Slash,					//�a��
		enSoundName_Boss_Poke,					//�˂�
		enSoundName_Boss_ShockWave,				//�V���b�N�E�F�[�u
		enSoundName_Boss_DarkBall,				//�_�[�N�{�[���̔���
		enSoundName_Boss_DarkBall_Explosion,	//�_�[�N�{�[���̔���
		enSoundName_Boss_Meteo_Shot,			//���e�I������
		enSoundName_Boss_Meteo_Fly,				//���e�I���󒆂ɂ���Ƃ�
		enSoundName_Boss_Meteo_Explosion,		//���e�I�̔���
		enSoundName_Boss_DarkMeteo,				//�_�[�N���e�I
		enSoundName_Boss_Angry,					//�{�胂�[�h
		enSoundName_Boss_Summon_charge,			//�����O�̉r��
		enSoundName_Boss_Summon_start,			//�����̏u��
		enSoundName_Boss_KnockBackWind,			//�m�b�N�o�b�N�̕�
		enSoundName_Boss_Break_SuperArmor,		//�X�[�p�[�A�[�}�[�u���C�N
		enSoundName_Boss_Warp,					//���[�v
		enSoundName_Boss_Die,					//�{�X�̎��S



		enSoundName_Num
	};

	class SoundManager:public Noncopyable
	{
	public:
		SoundManager();
		~SoundManager();

		/// <summary>
		/// �T�E���h�\�[�X��ǂݍ��݁A�Đ�
		/// </summary>
		/// <param name="soundName">�ǂݍ��݂����T�E���h�\�[�X��enum�̌^</param>
		/// <param name="volume">����</param>
		/// <param name="Sound3DFlag">3D�T�E���h�ɂ��邩</param>
		/// <param name="roopFlag">���[�v���邩</param>
		/// <param name="bgmFlag">BGM�Ȃ̂�</param>
		/// <param name="soundPosition">���̍��W�B3D�T�E���h���g���Ƃ��ɐݒ�</param>
		void InitAndPlaySoundSource(
			EnSoundName soundName, 
			float volume = 5.0f,
			bool Sound3DFlag = false, 
			bool roopFlag = false,
			bool bgmFlag = false,
			Vector3 soundPosition = g_vec3Zero
		);

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

		/// <summary>
		/// �w�肵���T�E���h�\�[�X�̃X�g�b�v
		/// </summary>
		/// <param name="soundName"></param>
		void StopSound(EnSoundName soundName);
		/// <summary>
		/// ���X�g�ɓ����Ă���S�ẴT�E���h�\�[�X�̃X�g�b�v
		/// </summary>
		void StopAllSound();

		/// <summary>
		/// ���X�g�ɓ����Ă���T�E���h���ꎞ��~
		/// </summary>
		void PauseAllSound();

		/// <summary>
		/// ���X�g�ɓ����Ă���T�E���h���v���C
		/// </summary>
		void PlayAllSound();

		/// <summary>
		/// ���X�g�ɓ����Ă���T�E���h�\�[�X���擾
		/// </summary>
		/// <param name="soundName"></param>
		SoundSource* GetSoundSource(EnSoundName soundName);

		/// <summary>
		/// �T�E���h�\�[�X�̉��ʂ�ݒ肷��
		/// </summary>
		/// <param name="mulVolume">�|���鉹��</param>
		/// <param name="bgmFlag">BGM�t���O�Btrue�Ȃ�BGM�̉��ʂ�ݒ�Afalse�Ȃ�BGM�ȊO��ݒ�</param>
		void SetSoundVolume(float mulVolume, bool bgmFlag);

		/// <summary>
		/// �S�ẴT�E���h�\�[�X�f�t�H���g�̉��ʂɕύX����
		/// </summary>
		void MakeAllSoundDefaultVolume();

		/// <summary>
		/// �f�t�H���g�̌��ʉ��{�����[�����擾
		/// </summary>
		/// <returns></returns>
		const float& GetDefaultSEVolume() const
		{
			return m_defaultSEVolume;
		}
		/// <summary>
		/// �f�t�H���g��BGM�{�����[�����擾
		/// </summary>
		/// <returns></returns>
		const float& GetDefaultBGVolume() const
		{
			return m_defaultBGMVolume;
		}
		/// <summary>
		/// ���ʉ��̃{�����[�����擾
		/// </summary>
		/// <returns></returns>
		const float& GetSEVolume() const
		{
			return m_seVolume;
		}
		/// <summary>
		/// BGM�̃{�����[�����擾
		/// </summary>
		/// <returns></returns>
		const float& GetBGMVolume() const
		{
			return m_bgmVolume;
		}

	private:
		std::vector<SoundSource*> m_soundSourceList;

		const float m_defaultSEVolume = 4.0f;
		const float m_defaultBGMVolume = 4.0f;

		float m_seVolume = m_defaultSEVolume;
		float m_bgmVolume = m_defaultBGMVolume;

	};

	extern SoundManager* g_soundManager;
}



