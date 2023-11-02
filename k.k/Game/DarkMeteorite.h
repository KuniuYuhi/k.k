#pragma once

class Meteo;
class Player;
class Lich;

class DarkMeteorite : public IGameObject
{
public:
	DarkMeteorite();
	~DarkMeteorite();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// サイズアップ
	/// </summary>
	void SizeUp();

	/// <summary>
	/// 撃つことに関する処理
	/// </summary>
	void ProcessShot();
	/// <summary>
	/// メテオを撃つ瞬間の処理
	/// </summary>
	/// <returns>メテオを生成したらtrue</returns>
	bool ShotMeteo();

	/// <summary>
	/// メテオのターゲット座標を設定
	/// </summary>
	/// <returns></returns>
	Vector3 SetMeteoTargetPosition();

	/// <summary>
	///	本体の当たり判定生成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// メテオの生成
	/// </summary>
	/// <param name="targetposition">ターゲット座標</param>
	void CreateMeteo(Vector3 targetposition);

	/// <summary>
	/// メテオを生成するタイマー
	/// </summary>
	/// <returns></returns>
	bool CreateMeteoTimer();

	/// <summary>
	/// 地面にヒットしたかの判定
	/// </summary>
	/// <param name="targetposition">ヒットしたかの判定したい座標</param>
	/// <param name="up">targetpositionのY座標をどれだけ上げるか</param>
	/// <param name="down">targetpositionのY座標をどれだけ下げるか</param>
	/// <returns></returns>
	bool IsHitGround(Vector3 targetposition,float up,float down);

	/// <summary>
	/// 壁にヒットしたかの判定
	/// </summary>
	/// <param name="pos1">始点</param>
	/// <param name="pos2">終点</param>
	/// <returns></returns>
	bool IsHitWall();

	/// <summary>
	/// ショットステート管理
	/// </summary>
	void ShotManageState();

	/// <summary>
	/// ダークメテオの移動ステート管理
	/// </summary>
	void DarkMeteoMoveManageState();

	void OnProcessFallStateTransition();
	void OnProcessChaseStateTransition();
	void OnProcessStraightStateTransition();

	/// <summary>
	/// ターゲットに向かうベクトルを設定
	/// </summary>
	void SetToTargetDirection();

	/// <summary>
	/// プレイヤーまでの距離が近いか。trueで近い
	/// </summary>
	bool IsNearDistanceToPlayer();

	/// <summary>
	/// 
	/// </summary>
	void SetTRS();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	void SetTargetPosition(Vector3 targetposition)
	{
		m_targetPosition = targetposition;
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	void SetShotStartFlag(bool flag)
	{
		m_ShotStartFlag = flag;
	}

	const bool& GetSizeUpFlag() const
	{
		return m_sizeUpFlag;
	}

	const bool& GetShotEndFlag() const
	{
		return m_shotEndFlag;
	}

	/// <summary>
	/// メテオを撃ち終わった後にでかいメテオを撃つかどうかのフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	/// <returns></returns>
	void SetmLastBigMeteoShotFlag(bool flag)
	{
		m_lastBigMeteoShotFlag = flag;
	}

	/// <summary>
	/// ビッグメテオの攻撃力を返す
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return m_bigMeteoAttack;
	}

	void SetChaseFlag(bool flag)
	{
		m_chaseFlag = flag;
	}

	/// <summary>
	/// ゲームが終わる時に自信を消すときに設定する
	/// </summary>
	/// <param name="flag"></param>
	void SetGameEndFlag(bool flag)
	{
		m_gameEndFlag = flag;
	}

private:

	//ショットステート
	enum EnShotState
	{
		enShotState_Meteo,
		enShotState_BigMeteo,
		enShotState_End
	};
	EnShotState						m_enShotState = enShotState_Meteo;

	//ダークメテオの移動ステート
	enum EnDarkMeteoMoveState
	{
		enDarkMeteoMoveState_None,
		enDarkMeteoMoveState_fall,
		enDarkMeteoMoveState_Chase,
		enDarkMeteoMoveState_straight
	};
	EnDarkMeteoMoveState			m_darkMeteoMoveState = enDarkMeteoMoveState_fall;

	Player*							m_player = nullptr;
	Lich*							m_lich = nullptr;
	EffectEmitter*					m_darkMeteoriteEffect;
	EffectEmitter*					m_windEffect;

	std::vector<Meteo*>				m_meteos;								//生成したメテオを格納するリスト

	ModelRender						m_model;

	CollisionObject*				m_collision = nullptr;

	Vector3							m_targetPosition = g_vec3Zero;
	Vector3							m_position = g_vec3Zero;
	Quaternion						m_rotation = g_quatIdentity;
	Vector3							m_scale = g_vec3One;
	Vector3							m_moveSpeed = g_vec3Zero;

	const Vector3					m_maxScale = { 10.0f,10.0f,10.0f };

	bool							m_sizeUpFlag = false;

	bool							m_ShotStartFlag = false;

	bool							m_shotEndFlag = false;

	float							m_createTime = 1.0f;
	float							m_createTimer = 0.0f;

	const int						m_createMeteoCount = 5;					//メテオを生成する数
	int								m_meteoCounter = 0;

	bool							m_lastBigMeteoShotFlag = false;

	bool							m_isBigMeteoYDownFlag = false;			//ビッグメテオのY座標を下げるか

	float							m_bigMeteoMoveCount = 4.0f;				//ビッグメテオがプレイヤーに向かうベクトルに移動する回数


	const int						m_bigMeteoAttack = 80;

	bool							m_chaseFlag = true;						//プレイヤーを追いかけるかのフラグ。trueで追いかける

	bool							m_gameEndFlag = false;

};

