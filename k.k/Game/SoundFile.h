#pragma once
class SoundFile
{
public:
	SoundFile();
	 ~SoundFile();

	 /// <summary>
	 /// �S�ẴT�E���h�t�@�C���̓ǂݍ���
	 /// </summary>
	 void ResistAllSoundFile();

private:

	/// <summary>
	/// �^�C�g���̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistTitleSoundFile();
	/// <summary>
	/// �o�g���J�n�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistBattleStartSoundFile();
	/// <summary>
	/// �{�X�o��̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistEntryBossSoundFile();
	/// <summary>
	/// �o�g�����̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistBattleSoundFile();
	/// <summary>
	/// �v���C���[�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistPlayerSoundFile();
	/// <summary>
	/// �����X�^�[�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistMonsterSoundFile();
	/// <summary>
	/// �{�X�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistBossSoundFile();
	/// <summary>
	/// ���u�����X�^�[�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistMobMonsterSoundFile();
	/// <summary>
	/// ���U���g�̃T�E���h�t�@�C���ǂݍ���
	/// </summary>
	void ResistResultSoundFile();

	
};

