#pragma once

namespace {
	const int SPEAR_ROWS = 3;		//槍の列の数
	const int SPEAR_CREATE_PLACE = 8;	//8 一列に槍を生成する場所の数

	const int SPEARS = SPEAR_ROWS * SPEAR_CREATE_PLACE;

	const float FIRST_CREATE_POS_LENGTH = 200.0f;

	const float ADD_CREATE_POS_LENGTH = 250.0f;

	const int CREATE_TIMER_LIMMIT = 1.0f;
}

class DarkSpearObj;

class DarkSpear: public IGameObject
{
public:
	DarkSpear();
	~DarkSpear();

	bool Start() override;
	void Update() override;
	
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// 前方向を設定
	/// </summary>
	/// <param name="forward"></param>
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}

	/// <summary>
	/// 生成が終わったかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetCreateEndFlag() const
	{
		return m_createEndFlag;
	}

	/// <summary>
	/// 生成開始フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetCreateStartFlag(bool flag)
	{
		m_createStartFlag = flag;
	}

private:

	/// <summary>
	/// 槍を生成する座標を決める
	/// </summary>
	void DecideCreatePosition();

	/// <summary>
	/// ダークスピアのオブジェクトを生成
	/// </summary>
	void CreateDarkSpearObj();

	/// <summary>
	/// 生成するタイミングか
	/// </summary>
	/// <returns></returns>
	bool IsCreateTiming();

	/// <summary>
	/// 範囲エフェクトを再生
	/// </summary>
	void PlayRangeEffect();
	/// <summary>
	/// 範囲エフェクトを削除
	/// </summary>
	void DeleteRangeEffect();

private:

	EffectEmitter* m_rangeEffect[SPEAR_CREATE_PLACE][SPEAR_ROWS] = { nullptr };

	DarkSpearObj* m_darkSpearObj[SPEAR_CREATE_PLACE][SPEAR_ROWS] = { nullptr };

	Vector3 m_position = g_vec3Zero;
	Vector3 m_forward = g_vec3Zero;

	Vector3 m_createPosition[SPEAR_CREATE_PLACE][SPEAR_ROWS];


	bool m_playRangeEffectFlag = false;

	bool m_createEndFlag = false;

	bool m_createStartFlag = false;

	float m_timer = 0.0f;

	int m_rowsCount = 0;

	float m_createTimer = CREATE_TIMER_LIMMIT;

};

