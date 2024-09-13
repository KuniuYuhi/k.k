#pragma once

#include "ArrowStatus.h"

class Brave;
class ArrowStatus;
class DamageProvider;
class Bow;

class UseEffect;

/// <summary>
/// 武器：ボウが使うアロークラス
/// </summary>
class Arrow:public IGameObject
{
public:

	Arrow();
	~Arrow();

	/// <summary>
	/// 攻撃パターンステート
	/// </summary>
	enum EnShotPatternState
	{
		enNormalShot,		//通常攻撃
		enSkillShot,		//スキル攻撃
		enNone,
	};
	/// <summary>
	/// 武器のステート
	/// </summary>
	enum EnWeaponState
	{
		enStowed,		//収納状態
		enArmed			//装備状態
	};


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;


	/// <summary>
	/// 弓のインスタンスを設定
	/// </summary>
	/// <param name="bow"></param>
	void SetBowInstance(Bow* bow)
	{
		m_bow = bow;
	}

	/// <summary>
	/// ダメージプロバイダーを取得
	/// </summary>
	/// <returns></returns>
	DamageProvider* GetDamageProvider()
	{
		return m_damageProvider;
	}

	/// <summary>
	/// ショットパターンを設定
	/// </summary>
	/// <param name="shotPattern"></param>
	void SetShotPatternState(EnShotPatternState shotPattern)
	{
		m_enShotPatternState = shotPattern;
	}

	/// <summary>
	/// 武器のステートを設定
	/// </summary>
	/// <param name="setWeaponState"></param>
	void SetWeaponState(EnWeaponState setWeaponState)
	{
		m_enWeaponState = setWeaponState;
	}

	/// <summary>
	/// スキル攻撃時の情報の設定
	/// </summary>
	/// <param name="power">スキル攻撃力</param>
	/// <param name="interval">攻撃情報更新インターバル</param>
	void SetSkillShotInfo(int power, int interval)
	{
		SetDefaultSkillAttackPower(power);
		SetAttackInfoUpdateTimeLimit(interval);
	}
	/// <summary>
	/// 基本スキル攻撃力を設定
	/// </summary>
	/// <param name="power"></param>
	void SetDefaultSkillAttackPower(int power)
	{
		m_defaultSkillAttackPower = power;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="timeLimit"></param>
	void SetAttackInfoUpdateTimeLimit(float timeLimit)
	{
		m_attackInfoUpdateTimeLimit = timeLimit;
	}

	/// <summary>
	/// 矢を放つときのパラメータの設定
	/// </summary>
	/// <param name="shotpatternState">通常攻撃かスキル攻撃か</param>
	/// <param name="forward">矢を放つキャラクターの前方向</param>
	void SetShotArrowParameters(
		EnShotPatternState shotpatternState,
		Vector3 forward);

	/// <summary>
	/// 収納状態にする
	/// </summary>
	void ChangeStowed();
	/// <summary>
	/// 装備状態にする
	/// </summary>
	void ChangeArmed();

	/// <summary>
	/// 収納状態の移動処理
	/// </summary>
	void MoveStowed();

	/// <summary>
	/// 装備中の移動処理
	/// </summary>
	void MoveArmed();

	

private:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// コンポーネントを設定
	/// </summary>
	void InitComponent();

	/// <summary>
	/// ショットパターンによるステートの管理
	/// </summary>
	void ManageShotPatternState();

	/// <summary>
	/// ノーマルショットパターンステートの更新処理
	/// </summary>
	void UpdateNormalShotState();
	/// <summary>
	/// スキルショットパターンステートの更新処理
	/// </summary>
	void UpdateSkillShotState();
	/// <summary>
	/// ショットパターンステートなしの更新処理
	/// </summary>
	void UpdateNoneState();
	
	/// <summary>
	/// 放たれた矢の移動
	/// </summary>
	void ShotArrowMove(EnShotPatternState shotPattern);



	/// <summary>
	/// 攻撃時の矢のトランスフォームの修正
	/// </summary>
	void FixedAttaackArrowTransform();


	/// <summary>
	/// 当たり判定を作成
	/// </summary>
	/// <param name="shotPatternState">ショットパターンステート</param>
	/// <param name="position">生成座標</param>
	/// <param name="rotation">回転</param>
	void CreateCollision(
		EnShotPatternState shotPatternState,
		Vector3 createPosition,
		Quaternion rotation
	);


	/// <summary>
	/// コリジョンに何かヒットしたか
	/// </summary>
	/// <returns></returns>
	bool IsHitCollision();


	/// <summary>
	/// 矢のエフェクトを生成
	/// </summary>
	void PlayArrowEffect(EnShotPatternState shotpatternState);

	/// <summary>
	/// 
	/// </summary>
	void DeleteArrow();


private:

	UseEffect* m_arrowEffect = nullptr;

	Brave* m_brave = nullptr;

	Bow* m_bow = nullptr;

	ModelRender m_arrowModelRender;

	DamageProvider* m_damageProvider = nullptr;

	CollisionObject* m_arrowCollision = nullptr;	//矢の当たり判定

	Matrix m_arrowMatrix;				//矢のワールド座標
	Matrix m_arrowCenterMatrix;			//矢の中心のワールド座標

	EnShotPatternState m_enShotPatternState = enNone;		//ショットパターンステート

	EnWeaponState m_enWeaponState = enStowed;

	Vector3 m_forward = g_vec3Zero;		//前方向

	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };

	Vector3 m_moveAttackArrowPosition = g_vec3Zero;


	std::map<EnShotPatternState, ArrowStatus> m_statusMap;


	//武器を持たせるときのボーンID
	int m_armedArrowBoonId = -1;

	int m_arrowCentorBoonId = -1;		//矢の真ん中のボーンID
	int m_arrowTipBoonId = -1;			//矢の先端のボーンID


	float m_deleteTimer = 0.0f;

	int m_defaultSkillAttackPower = 0;

	float m_attackInfoUpdateTimeLimit = 0.0f;
	float m_attackInfoUpdateTimer = 0.0f;


};

