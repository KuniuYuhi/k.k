#pragma once

class Meteo;

class DarkMeteorite : public IGameObject
{
public:
	DarkMeteorite();
	~DarkMeteorite();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void SizeUp();

	void Shot();

	void CreateMeteo(Vector3 targetposition);

	bool CreateTimer();

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

private:
	Meteo* m_meteo = nullptr;
	ModelRender m_model;

	CollisionObject* m_collision = nullptr;

	Vector3 m_targetPosition = g_vec3Zero;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;

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

};

