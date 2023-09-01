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

	void move();

	void SizeUp();

	void Shot();

	Vector3 SetMeteoTargetPosition();

	void CreateCollision();

	void CreateMeteo(Vector3 targetposition);

	bool CreateTimer();

	bool IsHitGround(Vector3 targetposition,float up,float down);

	bool IsHitWall(Vector3 pos1, Vector3 pos2);

	void ShotManageState();

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

private:

	enum EnShotState
	{
		enShotState_Meteo,
		enShotState_BigMeteo,
		enShotState_End
	};
	EnShotState m_enShotState = enShotState_Meteo;


	Player* m_player = nullptr;
	Lich* m_lich = nullptr;

	std::vector<Meteo*> m_meteos;		//生成したメテオを格納するリスト

	ModelRender m_model;

	CollisionObject* m_collision = nullptr;

	Vector3 m_targetPosition = g_vec3Zero;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;
	Vector3 m_moveSpeed = g_vec3Zero;

	RigidBody m_rigidBody;		//剛体。
	BoxCollider	m_boxCollider;

	const Vector3 m_maxScale = { 15.0f,15.0f,15.0f };

	bool m_sizeUpFlag = false;

	bool m_ShotStartFlag = false;

	bool m_shotEndFlag = false;

	//メテオを生成する範囲
	const float m_maxLength = 600.0f;

	//
	const float m_createTime = 1.0f;
	float m_createTimer = 0.0f;

	//メテオを生成する数
	const int m_createMeteoCount = 6;
	int m_meteoCounter = 0;

	bool m_lastBigMeteoShotFlag = false;

	bool m_isBigMeteoYDownFlag = false;			//ビッグメテオのY座標を下げるか

	float m_bigMeteoMoveCount = 7.0f;				//ビッグメテオがプレイヤーに向かうベクトルに移動する回数


	const int m_bigMeteoAttack = 80;

};

