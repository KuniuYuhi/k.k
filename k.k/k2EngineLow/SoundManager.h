#pragma once

namespace nsK2EngineLow {

	enum EnSoundName
	{
		//アウトゲーム
		enSoundName_Decision,					//決定
		enSoundName_Select,						//選択
		//BGM
		enSoundName_TitleBGM,
		enSoundName_BattleBGM,
		//タイトル関連
		enSoundName_StarShine,					//剣のキラーンのやつ


		//ゲーム終了
		enSoundName_GameClear,
		enSoundName_GameOver,
		//SE
		//プレイヤー側
		enSoundName_Sword_Hit,					//剣がヒットした時
		enSoundName_Wand_Hit,					//杖がヒットした時
		enSoundName_CriticalHit,				//クリティカルヒット
		enSoundName_Dash,						//ダッシュ

		//モンスター側
		enSoundName_Boss_Warp,					//ボスのワープ

		enSoundName_Num
	};

	class SoundManager:public Noncopyable
	{
	public:
		SoundManager();
		~SoundManager();

		

		void Init();

		/// <summary>
		/// サウンドファイルを読み込む
		/// </summary>
		void SetSoundFile();

		/// <summary>
		/// サウンドソースを読み込み、再生
		/// </summary>
		/// <param name="soundName">読み込みたいサウンドの名前</param>
		void InitAndPlaySoundSource(EnSoundName soundName, bool Sound3DFlag = false, bool roopFlag = false);

		/// <summary>
		/// サウンドソースをリストに追加
		/// </summary>
		/// <param name="soundSource">追加するサウンドソース</param>
		void AddSoundSourceList(SoundSource* addSoundSource);
		/// <summary>
		/// サウンドソースをリストから削除
		/// </summary>
		/// <param name="elaseSoundSource">削除するサウンドソース</param>
		void EraseSoundSourceList(SoundSource* elaseSoundSource);


		void StopSound(EnSoundName soundName);

		/// <summary>
		/// リストに入っているサウンドを一時停止
		/// </summary>
		void PauseAllSound();

		/// <summary>
		/// リストに入っているサウンドをプレイ
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



