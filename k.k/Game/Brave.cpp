#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//コンポーネント
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"

//モデルのアニメーション読み込みファイル作ること

///////////////////////////////////////
//
///////////////////////////////////////





Brave::~Brave()
{
}

bool Brave::Start()
{
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS");

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);


	m_charaCon.Init(20.0f, 50.0f, m_position);

	//ステータスを初期化
	m_status.InitPlayerStatus("Brave");
	
	//各種コンポーネントのセッティング
	SettingDefaultComponent();

	return true;
}

void Brave::Update()
{

	//武器の処理

	//移動処理
	Movement();


	//回転が最後
	Rotation();

	//m_playerMovement->UpdateComponent();


	//モデルの更新処理
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Brave::Damage()
{
}

void Brave::Dead()
{
}

void Brave::SettingDefaultComponent()
{
	//プレイヤー移動コンポーネントを取得
	AddComponent<PlayerMovement>();
	m_playerMovement = GetComponent<PlayerMovement>();

	//コントローラーコンポーネントを取得
	AddComponent<PlayerController>();
	m_playerContoller = GetComponent<PlayerController>();

}

void Brave::Movement()
{
	//コントローラーの入力を受け付けないなら移動処理しない
	if (!m_playerContoller->IsControllerInputEnabledFlag()) return;

	//移動量を取得
	m_moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
		m_status, m_moveSpeed, m_playerContoller->GetLStickInput()
	);

	//仮のジャンプ処理
	if (m_playerContoller->IsButtonTrigger(enButtonA) && m_charaCon.IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}

	//重力の計算
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//キャラコンを使って座標を移動
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}

	//回転用ベクトルに移動量を保存
	m_rotateDirection = m_moveSpeed;
}

void Brave::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}

void Brave::Attack()
{
}




void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
