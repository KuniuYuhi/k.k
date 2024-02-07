#include "stdafx.h"
#include "Arrow.h"
#include "Bow.h"
#include "Player.h"
#include "Boss.h"
#include "CharactersInfoManager.h"


//todo　矢を回転させて飛ばす
//todo 前方向を回転に適応して線形補間で回転

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
	//通常攻撃の当たり判定のサイズ
	const Vector3 ARROW_NORMAL_COLLISION_SIZE = { 70.0f,30.0f,10.0f };
	//スキル攻撃の当たり判定のサイズ
	const Vector3 ARROW_Skill_COLLISION_SIZE = { 150.0f,14.0f,20.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	const float SKILL_DELETE_RANGE = 600.0f;
	const float SKILL_ARROW_SPEED = 1000.0f;

	const float DEFAULT_DELETE_RANGE = 400.0f;	//矢が消える距離
	const float DEFAULT_ARROW_SPEED = 450.0f;

	const float GRAVITY = 11.8f;					//重力

	const float HITTABLE_TIMER_LIMMIT = 0.1f;
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	DeleteGO(m_arrowCollision);

	if (m_arrowAttackEffect != nullptr)
	{
		m_arrowAttackEffect->Stop();
	}
}

bool Arrow::Start()
{
	//勇者のインスタンスを探す
	m_player = FindGO<Player>("player");

	//モデルを初期化
	InitModel();
	
	//装備
	//弓と同じ状態を設定
	SetWeaponState(m_bow->GetBowEnWeaponState());

	return true;
}

void Arrow::Update()
{
	if (m_shotFlag != true)
	{
		MoveWeapon();
	}
	else
	{
		//射撃時の移動処理
		ProcessLongRangeAttack();

		
	}

	m_modelArrow.Update();
}

void Arrow::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://収納状態
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://装備状態
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://なし
		break;
	default:
		break;
	}
}

void Arrow::MoveArmed()
{
	//矢の座標を設定
	m_arrowMatrix = m_bow->GetArrowMatrix();
	m_modelArrow.SetWorldMatrix(m_arrowMatrix);
}

void Arrow::MoveStowed()
{
	//矢の座標を設定
	m_arrowPos = STOWEDS_POSITION;
	m_modelArrow.SetPosition(m_arrowPos);
}

bool Arrow::IsHitCollision()
{
	//ボスとの当たり判定
	if (m_arrowCollision->IsHit(
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetCharacterController()
	))
	{
		//ヒットした
		return true;
	}

	//todo 生成された瞬間だとキャラコンが初期化されていないのでエラーになる

	//モブモンスターのリストの取得
	const auto& mobMonsters = 
		CharactersInfoManager::GetInstance()->GetMobMonsters();
	//モブモンスターとの当たり判定
	for (auto mobMonster : mobMonsters)
	{
		if (m_arrowCollision->IsHit(
			mobMonster->GetCharacterController()))
		{
			//ヒットした
			return true;
		}
	}

	//ヒットしなかった
	return false;
}

void Arrow::ProcessLongRangeAttack()
{
	switch (m_enShotPatternState)
	{
	case Arrow::enShotPatternState_Normal:
		NormalShot();
		break;
	case Arrow::enShotPatternState_Skill:
		SkillShot();
		break;
	default:
		break;
	}

	m_arrowCollision->SetPosition(m_arrowPos);
	m_arrowCollision->Update();
}

void Arrow::SetShotArrowSetting(
	bool shotFlag, 
	Vector3 forward, 
	Vector3 shotStartPosition, 
	float angle,
	EnShotPatternState shotPatternState
)
{
	//ショットフラグを設定
	SetShotFlag(shotFlag);
	//前方向を設定
	SetForward(forward);
	//ワールド座標をローカル座標に適応
	ApplyMatrixToLocalPosition();
	//ショット開始座標の設定
	SetShotStartPosition(m_arrowPos, angle);
	//ショットステートの設定
	SetShotPatternState(shotPatternState);
	//当たり判定の初期化
	SelectInitCollision(shotPatternState);
	//落下地点の設定
	SetTargetPosition();
	//通常攻撃の設定なら
	if (shotPatternState == enShotPatternState_Normal)
	{
		//通常攻撃に必要な情報の設定
		SetNormalShotInfo();
	}

	//攻撃方法によるエフェクト再生
	PlayArrowEffect();
}

void Arrow::SetTargetPosition()
{
	m_targetPosition = m_shotStartPosition;
	m_targetPosition += m_forward * DEFAULT_DELETE_RANGE;
	m_targetPosition.y = 0.0f;
}

void Arrow::InitModel()
{
	//矢モデルの初期化
	m_modelArrow.Init("Assets/modelData/character/Player/NewHero/Arrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);
}

void Arrow::PlayArrowEffect()
{
	Quaternion rot = g_quatIdentity;
	rot.SetRotationYFromDirectionXZ(m_forward);

	if (m_enShotPatternState==enShotPatternState_Normal)
	{
		m_arrowAttackEffect = NewGO<EffectEmitter>(0);
		m_arrowAttackEffect->Init(InitEffect::enEffect_Arrow);
		m_arrowAttackEffect->Play();
		m_arrowAttackEffect->SetScale(g_vec3One * 12.0f);
		
	}
	else
	{
		m_arrowAttackEffect = NewGO<EffectEmitter>(0);
		m_arrowAttackEffect->Init(InitEffect::enEffect_BowArrowSkillShot);
		m_arrowAttackEffect->Play();
		m_arrowAttackEffect->SetScale(g_vec3One * 10.0f);
	}

	m_arrowAttackEffect->SetPosition(m_shotStartPosition);
	m_arrowAttackEffect->SetRotation(rot);
	m_arrowAttackEffect->Update();

	
}

void Arrow::SelectInitCollision(EnShotPatternState shotPatternState)
{
	switch (shotPatternState)
	{
	case Arrow::enShotPatternState_Normal:
		InitCollision(
			"Attack",m_arrowPos,g_quatIdentity, ARROW_NORMAL_COLLISION_SIZE);
		break;
	case Arrow::enShotPatternState_Skill:
		InitCollision(
			"skillAttack", m_arrowPos, g_quatIdentity, ARROW_Skill_COLLISION_SIZE);
		break;
	default:
		break;
	}
}

void Arrow::InitCollision(
	const char* collisionName,
	Vector3 createPos,
	Quaternion rotation,
	Vector3 collisionSize
)
{
	//矢の通常攻撃用の当たり判定の生成
	m_arrowCollision = NewGO<CollisionObject>(0, collisionName);
	m_arrowCollision->CreateBox(
		createPos,
		rotation,
		collisionSize
	);
	m_arrowCollision->SetIsEnableAutoDelete(false);
	m_arrowCollision->SetWorldMatrix(m_arrowMatrix);
	m_arrowCollision->Update();
}

void Arrow::NormalShot()
{
	//コリジョンが敵に当たったら
	if (IsHitCollision() == true)
	{
		ProcessDelete();
		return;
	}
	
	//放物線を描く移動処理
	//消去するまでの制限時間内なら
	if (m_deleteTimer < m_flightDuration)
	{
		//矢の移動処理
		MoveNormalShot();
		//矢の回転処理　やってない
		RoatationNormalShot();
		//消去するまでのタイマーを加算
		m_deleteTimer += g_gameTime->GetFrameDeltaTime() * 6.0f;
		
	}
	else
	{
		//消滅
		DeleteGO(this);
	}
	
	//矢のワールド座標を取得
	Matrix arrowMatrix = m_arrowMatrix;
	//行列に座標を適応
	ApplyVector3ToMatirx(arrowMatrix, m_arrowPos);
	//モデルの行列を設定
	m_modelArrow.SetWorldMatrix(arrowMatrix);

	//エフェクトの座標の設定と更新
	m_arrowAttackEffect->SetPosition(m_arrowPos);
	m_arrowAttackEffect->Update();

	//前フレームの矢の座標を取得
	m_oldArrowPos = m_arrowPos;
}

void Arrow::SkillShot()
{
	//敵にダメージを与えられるタイミングを設定
	if (m_player->GetHittableFlag() != true&&
		m_hitDelection.IsHittable(HITTABLE_TIMER_LIMMIT))
	{
		//多段ヒット攻撃可能
		m_player->SetHittableFlag(true);
		//多段ヒット可能判定フラグをリセット
		m_hitDelection.SetHittableFlag(false);
	}

	//射撃開始座標から現在の座標に向かうベクトルを計算
	Vector3 diff = m_arrowPos - m_shotStartPosition;
	//矢が自然消滅する距離なら
	if (diff.Length() > SKILL_DELETE_RANGE)
	{
		//消滅
		DeleteGO(this);
	}
	//矢の座標を設定
	m_arrowPos += (m_forward * SKILL_ARROW_SPEED) * g_gameTime->GetFrameDeltaTime();

	Matrix arrowMatrix = m_arrowMatrix;
	//行列に座標を適応
	ApplyVector3ToMatirx(arrowMatrix, m_arrowPos);
	//行列を設定
	m_modelArrow.SetWorldMatrix(arrowMatrix);
	//エフェクトの座標の設定と更新
	m_arrowAttackEffect->SetPosition(m_arrowPos);
	m_arrowAttackEffect->Update();
}

void Arrow::ApplyVector3ToMatirx(Matrix& baseMatrix, Vector3 position)
{
	Matrix matrix = baseMatrix;
	baseMatrix.m[3][0] = position.x;
	baseMatrix.m[3][1] = position.y;
	baseMatrix.m[3][2] = position.z;
}

void Arrow::SetNormalShotInfo()
{
	//1.目標に向かう距離の計算
	Vector3 targetDistance = m_targetPosition - m_shotStartPosition;
	float distance = targetDistance.Length();
	//2.初速度の計算
	float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	//3.初速度の分解
	m_shotArrowVerocity.x = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	m_shotArrowVerocity.y = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	//4.飛行時間の計算
	m_flightDuration = distance / m_shotArrowVerocity.x;

	m_oldArrowPos = m_arrowPos;
}

void Arrow::MoveNormalShot()
{
	float X = m_forward.x * m_shotArrowVerocity.x *
		g_gameTime->GetFrameDeltaTime() * 10.0f;
	float Z = m_forward.z * m_shotArrowVerocity.x *
		g_gameTime->GetFrameDeltaTime() * 10.0f;

	//新しい座標
	m_arrowPos += {
		X,
			(m_shotArrowVerocity.y - (GRAVITY * m_deleteTimer))* g_gameTime->GetFrameDeltaTime() * 4.0f,
			Z
	};
}

void Arrow::RoatationNormalShot()
{
	//回転
	Vector3 Axis;
	Axis.Cross(m_shotStartPosition, m_arrowPos);

	//角度の計算
	//内積
	Vector3 dot1 = m_shotStartPosition;
	Vector3 dot2 = m_arrowPos;

	float vec1 = m_shotStartPosition.Length();
	float vec2 = m_arrowPos.Length();

	dot1.Normalize();
	dot2.Normalize();
	float dotProduct = Dot(dot1, dot2);
	//ノルム(ベクトルの大きさ)

	//アークコサインの計算
	float acos = std::acos(dotProduct / (vec1 * vec2));
	//ラジアンから度に変換
	float rotationAngle = Math::RadToDeg(acos);
}

void Arrow::ProcessDelete()
{
	//モンスターにダメージを与えられるようにフラグをリセット
	CharactersInfoManager::GetInstance()->SetAllMonsterDamgeHitFlag(true);
	//当たり判定の削除
	DeleteGO(m_arrowCollision);
	//矢が当たったので削除
	DeleteGO(this);
}

void Arrow::Render(RenderContext& rc)
{
	//収納状態なら表示しない
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelArrow.Draw(rc);
}
