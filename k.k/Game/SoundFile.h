#pragma once
class SoundFile
{
public:
	SoundFile();
	 ~SoundFile();

	 /// <summary>
	 /// 全てのサウンドファイルの読み込み
	 /// </summary>
	 void ResistAllSoundFile();

private:

	/// <summary>
	/// タイトルのサウンドファイル読み込み
	/// </summary>
	void ResistTitleSoundFile();
	/// <summary>
	/// バトル開始のサウンドファイル読み込み
	/// </summary>
	void ResistBattleStartSoundFile();
	/// <summary>
	/// ボス登場のサウンドファイル読み込み
	/// </summary>
	void ResistEntryBossSoundFile();
	/// <summary>
	/// バトル中のサウンドファイル読み込み
	/// </summary>
	void ResistBattleSoundFile();
	/// <summary>
	/// プレイヤーのサウンドファイル読み込み
	/// </summary>
	void ResistPlayerSoundFile();
	/// <summary>
	/// モンスターのサウンドファイル読み込み
	/// </summary>
	void ResistMonsterSoundFile();
	/// <summary>
	/// ボスのサウンドファイル読み込み
	/// </summary>
	void ResistBossSoundFile();
	/// <summary>
	/// モブモンスターのサウンドファイル読み込み
	/// </summary>
	void ResistMobMonsterSoundFile();
	/// <summary>
	/// リザルトのサウンドファイル読み込み
	/// </summary>
	void ResistResultSoundFile();

	
};

