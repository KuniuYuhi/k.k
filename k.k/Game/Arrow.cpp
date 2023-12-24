#include "stdafx.h"
#include "Arrow.h"
#include "Bow.h"


//todo　矢を回転させて飛ばす
//todo 前方向を回転に適応して線形補間で回転

namespace {
	//武器が収納状態の時の座標
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };
	//通常攻撃の当たり判定のサイズ
	const Vector3 ARROW_NORMAL_COLLISION_SIZE = { 100.0f,10.0f,10.0f };
	//スキル攻撃の当たり判定のサイズ
	const Vector3 ARROW_Skill_COLLISION_SIZE = { 150.0f,14.0f,20.0f };

	const float SKILL_RADIUS = 60.0f;

	const float ADD_FORWARD = 30.0f;

	const float SKILL_DELETE_RANGE = 600.0f;
	const float SKILL_ARROW_SPEED = 550.0f;

	const float DEFAULT_DELETE_RANGE = 400.0f;	//矢が消える距離
	const float DEFAULT_ARROW_SPEED = 450.0f;

	const float GRAVITY = 11.8f;					//重力
}

Arrow::Arrow()
{
}

Arrow::~Arrow()
{
	DeleteGO(m_arrowCollision);
}

bool Arrow::Start()
{
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
	SetShotFlag(shotFlag);
	SetForward(forward);
	ApplyMatrixToLocalPosition();
	SetShotStartPosition(m_arrowPos, angle);
	SetShotPatternState(shotPatternState);
	//当たり判定の初期化
	SelectInitCollision(shotPatternState);
	//落下地点の設定
	SetTargetPosition();

	//1.目標に向かう距離の計算
	Vector3 targetDistance = m_targetPosition - m_shotStartPosition;
	float distance = targetDistance.Length();
	//2.初速度の計算
	float verocity = distance / (sin(Math::DegToRad(2 * m_angle)) / GRAVITY);
	//3.初速度の分解
	m_shotArrowVerocity.Vx = sqrt(verocity) * cos(Math::DegToRad(m_angle));
	m_shotArrowVerocity.Vy = sqrt(verocity) * sin(Math::DegToRad(m_angle));
	//4.飛行時間の計算
	m_flightDuration = distance / m_shotArrowVerocity.Vx;
	
	m_oldArrowPos = m_arrowPos;
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
	//m_arrowCollision->SetIsEnable(false);

	m_arrowCollision->SetWorldMatrix(m_arrowMatrix);
	m_arrowCollision->Update();
}

void Arrow::NormalShot()
{
	//攻撃がヒットしたら
	if (m_bow->GetAttackHitFlag() == true)
	{
		//ヒットしたので、攻撃がヒットしたかのフラグをリセット
		m_bow->SetAttackHitFlag(false);
		DeleteGO(this);
	}
	//6.移動
	if (m_deleteTimer < m_flightDuration)
	{
		float X = m_forward.x * m_shotArrowVerocity.Vx * 
			g_gameTime->GetFrameDeltaTime() * 10.0f;
		float Z = m_forward.z * m_shotArrowVerocity.Vx * 
			g_gameTime->GetFrameDeltaTime() * 10.0f;

		//新しい座標
		m_arrowPos += {
			X,
			(m_shotArrowVerocity.Vy - (GRAVITY * m_deleteTimer)) * g_gameTime->GetFrameDeltaTime()*4.0f,
			Z
		};
		m_deleteTimer += g_gameTime->GetFrameDeltaTime() * 6.0f;

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
		float dotProduct = Dot(dot1,dot2);
		//ノルム(ベクトルの大きさ)
		
		//アークコサインの計算
		float acos = std::acos(dotProduct / (vec1 * vec2));
		//ラジアンから度に変換
		float rotationAngle = Math::RadToDeg(acos);

		//////////////////////////////////////////////////

		Quaternion rot = g_quatIdentity;
		//rot.SetRotation(m_arrowMatrix);
		Vector3 a = m_arrowPos;
		a.Normalize();
		Vector3 direction = m_arrowPos - m_shotStartPosition;
		/*direction.x *= a.x;
		direction.y *= a.y;
		direction.z *= a.z;*/

		//rot.SetRotation(direction, g_vec3AxisX);

		m_arrowMatrix.Apply(rot);

		m_rotation = rot;
	}
	else
	{
		//消滅
		DeleteGO(this);
	}
	//m_modelArrow.SetWorldMatrix(aa);
	
	m_modelArrow.SetPosition(m_arrowPos);
	//m_modelArrow.SetRotation(m_rotation);

	//前フレームの矢の座標を取得
	m_oldArrowPos = m_arrowPos;
}

void Arrow::SkillShot()
{
	//敵にダメージを与えられるタイミングを設定



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
	m_modelArrow.SetPosition(m_arrowPos);
}

void Arrow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelArrow.Draw(rc);
}
