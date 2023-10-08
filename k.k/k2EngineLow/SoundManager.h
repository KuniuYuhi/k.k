#pragma once

namespace nsK2EngineLow {

	//todo 同じ番号のサウンドソース全てが参照されてしまう。製作者の名前あり？
	//todo 同じ番号のサウンドソースが複数あると望んだサウンドソースが取得できない

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

		//ボス登場
		enSoundName_Gogogo,						//ゴゴゴゴゴゴゴ
		enSoundName_FogRemoval,					//霧払い

		//インゲーム
		enSoundName_BattleStart,				//バトルスタート

		//ゲーム終了
		enSoundName_GameClear,
		enSoundName_GameOver,
		//SE
		//プレイヤー側
		enSoundName_ChangeCharacter,			//キャラ切り替え
		enSoundName_Sword_Hit,					//剣がヒットした時
		enSoundName_Wand_Hit,					//杖がヒットした時
		enSoundName_CriticalHit,				//クリティカルヒット
		enSoundName_Dash,						//ダッシュ
		enSoundName_SwordStorm_Charge,			//ソードストームのチャージ
		enSoundName_SwordStorm_Storm,			//ソードストームのストーム
		enSoundName_FlamePillar,				//フレイムピラーの炎
		enSoundName_FireBall_Start,				//ファイヤーボールの発生
		enSoundName_FireBall_Explosion,			//ファイヤーボールの爆発

		//モンスター側
		enSoundName_Boss_Warp,					//ボスのワープ
		enSoundName_Attack1,					//基本モブモンスターの攻撃１
		enSoundName_Slime_Attack,				//スライムの攻撃
		enSoundName_Cactus_Attack1,				//カクタスの攻撃1
		enSoundName_Mob_Die,					//モブモンスターの死亡
		enSoundName_Boss_DarkWall,				//ダークウォール
		enSoundName_Boss_DarkBall,				//ダークボールの発生
		enSoundName_Boss_DarkBall_Explosion,	//ダークボールの爆発
		enSoundName_Boss_Meteo_Shot,			//メテオを撃つ
		enSoundName_Boss_Meteo_Fly,				//メテオが空中にいるとき
		enSoundName_Boss_Meteo_Explosion,		//メテオの爆発
		enSoundName_Boss_DarkMeteo,				//ダークメテオ
		enSoundName_Boss_Angry,					//怒りモード
		enSoundName_Boss_Summon_charge,			//召喚前の詠唱
		enSoundName_Boss_Summon_start,			//召喚の瞬間

		enSoundName_Num
	};

	class SoundManager:public Noncopyable
	{
	public:
		SoundManager();
		~SoundManager();

		/// <summary>
		/// サウンドファイルを読み込む
		/// </summary>
		void SetSoundFile();

		/// <summary>
		/// サウンドソースを読み込み、再生
		/// </summary>
		/// <param name="soundName">読み込みたいサウンドソースのenumの型</param>
		/// <param name="volume">音量</param>
		/// <param name="Sound3DFlag">3Dサウンドにするか</param>
		/// <param name="roopFlag">ループするか</param>
		/// <param name="bgmFlag">BGMなのか</param>
		/// <param name="soundPosition">音の座標。3Dサウンドを使うときに設定</param>
		void InitAndPlaySoundSource(
			EnSoundName soundName, 
			float volume = 5.0f,
			bool Sound3DFlag = false, 
			bool roopFlag = false,
			bool bgmFlag = false,
			Vector3 soundPosition = g_vec3Zero
		);

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

		/// <summary>
		/// 指定したサウンドソースのストップ
		/// </summary>
		/// <param name="soundName"></param>
		void StopSound(EnSoundName soundName);
		/// <summary>
		/// リストに入っている全てのサウンドソースのストップ
		/// </summary>
		void StopAllSound();

		/// <summary>
		/// リストに入っているサウンドを一時停止
		/// </summary>
		void PauseAllSound();

		/// <summary>
		/// リストに入っているサウンドをプレイ
		/// </summary>
		void PlayAllSound();

		/// <summary>
		/// リストに入っているサウンドソースを取得
		/// </summary>
		/// <param name="soundName"></param>
		SoundSource* GetSoundSource(EnSoundName soundName);

		/// <summary>
		/// サウンドソースの音量を設定する
		/// </summary>
		/// <param name="mulVolume">掛ける音量</param>
		/// <param name="bgmFlag">BGMフラグ。trueならBGMの音量を設定、falseならBGM以外を設定</param>
		void SetSoundVolume(float mulVolume, bool bgmFlag);

		/// <summary>
		/// 全てのサウンドソースデフォルトの音量に変更する
		/// </summary>
		void MakeAllSoundDefaultVolume();

		const float& GetDefaultSEVolume() const
		{
			return m_defaultSEVolume;
		}

		const float& GetDefaultBGVolume() const
		{
			return m_defaultBGMVolume;
		}

		const float& GetSEVolume() const
		{
			return m_seVolume;
		}

		const float& GetBGMVolume() const
		{
			return m_bgmVolume;
		}

	private:
		std::vector<SoundSource*> m_soundSourceList;

		const float m_defaultSEVolume = 4.0f;
		const float m_defaultBGMVolume = 2.5f;

		float m_seVolume = m_defaultSEVolume;
		float m_bgmVolume = m_defaultBGMVolume;

	};

	extern SoundManager* g_soundManager;
}



