#pragma once

namespace nsK2EngineLow {

	//todo 同じ番号のサウンドソースが複数あると望んだサウンドソースが取得できない

	/// <summary>
	/// サウンドの名前
	/// </summary>
	enum EnSoundName
	{
		////////////////////////////////////////////////////////////////
		//BGM
		////////////////////////////////////////////////////////////////
		enSoundName_TitleBGM,
		enSoundName_BattleBGM,
		////////////////////////////////////////////////////////////////
		//タイトル
		////////////////////////////////////////////////////////////////
		enSoundName_Decision,					//決定
		enSoundName_Select,						//選択
		enSoundName_StarShine,					//剣のキラーンのやつ
		enSoundName_DaahCliff,					//ダッシュ
		enSoundName_JampCliff,					//ジャンプ
		////////////////////////////////////////////////////////////////
		//勇者登場
		////////////////////////////////////////////////////////////////
		enSoundName_Fall,						//落下中
		enSoundName_Landing,					//着地
		////////////////////////////////////////////////////////////////
		//ボス登場
		////////////////////////////////////////////////////////////////
		enSoundName_Gogogo,						//ゴゴゴゴゴゴゴ
		enSoundName_FogRemoval,					//霧払い
		enSoundName_BossNameDon,				//ボスの名前出すときのドン
		////////////////////////////////////////////////////////////////
		//リザルト
		////////////////////////////////////////////////////////////////
		enSoundName_GameClear,					//ゲームクリア
		enSoundName_GameOver,					//ゲームオーバー

		//SE
		 
		////////////////////////////////////////////////////////////////
		//プレイヤー
		////////////////////////////////////////////////////////////////
		enSoundName_ChangeWeapon,				//武器の切り替え
		//ソード＆シールドの音
		enSoundName_SwordShieldCombo_1_2,		//
		enSoundName_SwordShieldCombo_3,			//
		enSoundName_SwordShieldSkillRising,		//
		enSoundName_SwordShieldSkillAttack,		//
		//シールドの音
		enSoundName_SwordShieldDefendHit,		//
		
		//両手剣の音
		enSoundName_GreatSwordCombo_1_2,		//
		enSoundName_GreatSwordCombo_3,			//
		enSoundName_GreatSwordSkillRising,		//
		enSoundName_GreatSwordSkillAttack,		//

		//ボウ＆アローの音
		enSoundName_BowArrowNormalShot,			//
		enSoundName_BowArrowSkillCharge,		//
		enSoundName_BowArrowSkillAttack,		//


		//モンスター側
		enSoundName_Monster_Hit,				//被ダメージ
		enSoundName_Monster_DieHit,				//死亡確定ヒット
		enSoundName_Attack1,					//基本モブモンスターの攻撃１
		enSoundName_Slime_Attack,				//スライムの攻撃
		enSoundName_Cactus_Attack1,				//カクタスの攻撃1
		enSoundName_Mob_Die,					//モブモンスターの死亡

		enSoundName_Boss_Slash,					//斬撃
		enSoundName_Boss_Poke,					//突き
		enSoundName_Boss_ShockWave,				//ショックウェーブ
		enSoundName_Boss_DarkBall,				//ダークボールの発生
		enSoundName_Boss_DarkBall_Explosion,	//ダークボールの爆発
		enSoundName_Boss_Meteo_Shot,			//メテオを撃つ
		enSoundName_Boss_Meteo_Fly,				//メテオが空中にいるとき
		enSoundName_Boss_Meteo_Explosion,		//メテオの爆発
		enSoundName_Boss_DarkMeteo,				//ダークメテオ
		enSoundName_Boss_Angry,					//怒りモード
		enSoundName_Boss_Summon_charge,			//召喚前の詠唱
		enSoundName_Boss_Summon_start,			//召喚の瞬間
		enSoundName_Boss_KnockBackWind,			//ノックバックの風
		enSoundName_Boss_Break_SuperArmor,		//スーパーアーマーブレイク
		enSoundName_Boss_Warp,					//ワープ
		enSoundName_Boss_Die,					//ボスの死亡



		enSoundName_Num
	};

	class SoundManager:public Noncopyable
	{
	public:
		SoundManager();
		~SoundManager();

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

		/// <summary>
		/// デフォルトの効果音ボリュームを取得
		/// </summary>
		/// <returns></returns>
		const float& GetDefaultSEVolume() const
		{
			return m_defaultSEVolume;
		}
		/// <summary>
		/// デフォルトのBGMボリュームを取得
		/// </summary>
		/// <returns></returns>
		const float& GetDefaultBGVolume() const
		{
			return m_defaultBGMVolume;
		}
		/// <summary>
		/// 効果音のボリュームを取得
		/// </summary>
		/// <returns></returns>
		const float& GetSEVolume() const
		{
			return m_seVolume;
		}
		/// <summary>
		/// BGMのボリュームを取得
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



