#pragma once
class DarkSpearObj:public IGameObject
{
public:
	DarkSpearObj();
	~DarkSpearObj();

	bool Start();
	void Update();

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

private:

	void PlayEffect();

	EffectEmitter* m_darkSpearEffect = nullptr;

	Vector3 m_position = g_vec3Zero;

};

