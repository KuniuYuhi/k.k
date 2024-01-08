#pragma once

#include "WeaponBase.h"

class Bow;

class Arrow:public WeaponBase
{
public:
	Arrow();
	~Arrow();

	enum EnShotPatternState
	{
		enShotPatternState_Normal,
		enShotPatternState_Skill
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 武器の移動処理
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	void MoveStowed() override;

	/// <summary>
	/// 武器の当たり判定
	/// </summary>
	/// <returns></returns>
	bool IsHitCollision() override;

	/// <summary>
	/// 弓のインスタンスを設定
	/// </summary>
	/// <param name="bow"></param>
	void SetBow(Bow* bow)
	{
		m_bow = bow;
	}

	/// <summary>
	/// 矢のワールド座標を設定
	/// </summary>
	/// <param name="matrix"></param>
	void SetArrowMatrix(const Matrix matrix)
	{
		m_arrowMatrix = matrix;
	}
	/// <summary>
	/// 矢の情報を更新
	/// </summary>
	void ArrowUpdate()
	{
		m_modelArrow.Update();
	}

	/// <summary>
	/// 射撃フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetShotFlag(bool flag)
	{
		m_shotFlag = flag;
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
	/// 射撃開始時の設定
	/// </summary>
	/// <param name="startPosition">矢を発射する座標</param>
	/// <param name="deg">角度</param>
	void SetShotStartPosition(Vector3 startPosition,float angle)
	{
		m_shotStartPosition = startPosition;
		m_angle = angle;
	}
	/// <summary>
	/// 矢の座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_arrowPos;
	}
	/// <summary>
	/// ワールド座標をローカル座標に適応
	/// </summary>
	void ApplyMatrixToLocalPosition()
	{
		m_arrowPos = g_vec3Zero;
		m_arrowMatrix.Apply(m_arrowPos);
		m_rotation.SetRotation(m_arrowMatrix);
	}

	/// <summary>
	/// 矢を撃つ時の設定
	/// </summary>
	/// <param name="shotFlag">ショットフラグ</param>
	/// <param name="forward">矢を撃つキャラの前方向</param>
	/// <param name="angle">発射する角度</param>
	/// <param name="shotStartPosition">発射開始座標</param>
	void SetShotArrowSetting(
		bool shotFlag,
		Vector3 forward,
		Vector3 shotStartPosition,
		float angle,
		EnShotPatternState shotPatternState
	);
	/// <summary>
	/// 矢が落ちる地点の設定
	/// </summary>
	void SetTargetPosition();

	/// <summary>
	/// ショットパターンステートを設定
	/// </summary>
	/// <param name="shotPatternState"></param>
	void SetShotPatternState(EnShotPatternState shotPatternState)
	{
		m_enShotPatternState = shotPatternState;
	}

private:
	/// <summary>
	/// モデルの初期化
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// ショットパターンによって当たり判定を初期化
	/// </summary>
	/// <param name="shotPatternState">初期化したい当たり判定のステート</param>
	void SelectInitCollision(EnShotPatternState shotPatternState);
	
	/// <summary>
	/// 通常攻撃の当たり判定の初期化
	/// </summary>
	/// <param name="collisionName">当たり判定の名前</param>
	/// <param name="createPos">生成したい座標</param>
	/// <param name="rotation">回転</param>
	/// <param name="collisionSize">当たり判定のサイズ(ボックス)</param>
	void InitCollision(
		const char* collisionName,
		Vector3 createPos,
		Quaternion rotation,
		Vector3 collisionSize
		);

	/// <summary>
	/// 遠距離攻撃処理
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// 通常攻撃
	/// </summary>
	void NormalShot();
	/// <summary>
	/// スキル攻撃
	/// </summary>
	void SkillShot();

	/// <summary>
	/// 座標を行列に適応
	/// </summary>
	/// <param name="matrix">行列</param>
	/// <param name="position">座標</param>
	void ApplyVector3ToMatirx(Matrix& baseMatrix, Vector3 position);

	
	Bow* m_bow = nullptr;

	ModelRender m_modelArrow;		//矢モデル
	CollisionObject* m_arrowCollision = nullptr;

	Vector3 m_arrowPos = g_vec3Zero;
	Vector3 m_oldArrowPos = g_vec3Zero;
	Vector3 m_forward = g_vec3Zero;
	Vector3 m_shotStartPosition = g_vec3Zero;
	Vector3 m_targetPosition = g_vec3Zero;			//矢が落ちる地点
	Quaternion m_rotation = g_quatIdentity;

	Matrix m_arrowMatrix = g_matIdentity;

	EnShotPatternState m_enShotPatternState = enShotPatternState_Normal;
	

	bool m_shotFlag = false;			//矢を発射するかのフラグ

	float m_angle = 0.0f;
	float m_flightDuration = 0.0f;		//矢が落ちるまでの時間
	float m_deleteTimer = 0.0f;

	struct ShotArrowVerocity
	{
		float Vx = 0.0f;
		float Vy = 0.0f;
	};
	ShotArrowVerocity m_shotArrowVerocity;

};

