#pragma once
#include "IAttackObject.h"


class DarkSpearObj:public IAttackObject
{
public:
	DarkSpearObj();
	~DarkSpearObj();

	enum EnMaxScaleSize
	{
		enMaxScaleSize_Small,
		enMaxScaleSize_Medium,
		enMaxScaleSize_Large
	};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// ダークスピアの最大サイズを設定
	/// </summary>
	/// <param name="maxScaleSize">サイズ</param>
	void SetMaxScale(EnMaxScaleSize maxScaleSize);

	/// <summary>
	/// ダークスピアの最大サイズを設定
	/// </summary>
	/// <param name="sizeNumer">0から2まで、小→中→大</param>
	void SetMaxScale(int sizeNumer);


private:

	/// <summary>
	/// 当たり判定の作成
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// エフェクト再生
	/// </summary>
	void PlayEffect();

	/// <summary>
	/// ダークスピアのサイズを変更する。大きくなって小さくなる。
	/// </summary>
	void ChangeSizeDarkSpearObj();

private:

	CollisionObject* m_collision = nullptr;

	EffectEmitter* m_rangeEffect = nullptr;

	EffectEmitter* m_darkSpearEffect = nullptr;

	ModelRender m_darkSpearModel;

	Vector3 m_position = g_vec3Zero;

	Vector3 m_scale = g_vec3One;


	float m_deleteTimer = 0.0f;

	float m_maxScale = 1.0f;

	float m_oldMulSize = 0.0f;

	bool m_stopRangeEffectFlag = false;

	
};

