#pragma once

class Meteo;

class DarkMeteo:public IGameObject
{
public:
	DarkMeteo();
	~DarkMeteo();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// メテオを撃つ一連の処理の流れ
	/// </summary>
	enum EnDarkMeteoState
	{
		enDarkMeteoState_Charge,			//メテオをチャージする(大きくする)
		enDarkMeteoState_ShotMeteo,			//メテオを撃つ
		enDarkMeteoState_ShotDarkMeteo,		//特定の状況でチャージしたダークメテオを撃つ
		enDarkMeteoState_End				//処理終わり
	};

	/// <summary>
	/// チャージ完了フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetChargeCompleteFlag() const
	{
		return m_chargeCompleteFlag;
	}

	const bool& GetEndFlag() const
	{
		return m_endFlag;
	}

	/// <summary>
	/// ダークメテオを撃つかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShotDarkMeteoFlag(bool flag)
	{
		m_shotDarkMeteoFlag = flag;
	}

	/// <summary>
	/// ダークメテオの座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetDarkMeteoPosition(Vector3 position)
	{
		m_darkMeteoPositon = position;
	}

	/// <summary>
	/// 次のメテオステートを設定
	/// </summary>
	/// <param name="nextDarkMeteoState"></param>
	void SetNextDarkMeteoState(EnDarkMeteoState nextDarkMeteoState)
	{
		m_enDarkMeteoState = nextDarkMeteoState;
	}

private:

	/// <summary>
	/// ダークメテオの初期化
	/// </summary>
	void InitDarkMeteoEffect();

	/// <summary>
	/// メテオの生成
	/// </summary>
	/// <param name="endPosition">終点</param>
	void CreateMeteo(Vector3 endPosition);

	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();

	void OnProcessDarkMeteoChargeTranstion();
	void OnProcessShotMeteoTranstion();
	void OnProcessShotDarkMeteoTranstion();
	void OnProcessEndTranstion();

	/// <summary>
	/// メテオが壁にヒットするか
	/// </summary>
	/// <returns></returns>
	bool IsMeteoHitWall(Vector3 endPosition);

	/// <summary>
	/// メテオを撃ってもよいか
	/// </summary>
	/// <returns></returns>
	bool IsShotMeteo();

private:

	ModelRender model[5];

	std::vector<Meteo*>				m_meteoList;
	
	EnDarkMeteoState m_enDarkMeteoState = enDarkMeteoState_Charge;


	Vector3 m_darkMeteoPositon = g_vec3Zero;
	Vector3 m_darkMeteoScale = g_vec3One;

	EffectEmitter* m_darkMeteoriteEffect;//ダークメテオ本体のエフェクト

	float m_mulScaleDarkMeteo = 1.0f;	//ダークメテオのスケールの拡大率

	bool m_chargeCompleteFlag = false;

	bool m_shotDarkMeteoFlag = false;

	bool m_endFlag = false;


	float m_shotMeteoTimer = 0.0f;

	int m_shotMeteoCounter = 0;

};

