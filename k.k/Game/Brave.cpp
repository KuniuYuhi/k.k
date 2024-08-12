#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//コンポーネント
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"



///////////////////////////////////////
//ステートコンテキスト
///////////////////////////////////////
//#include "BraveStateContext.h"


#include "WeaponManager.h"

#include "SwordShield.h"
#include "GreateSword.h"
#include "Bow.h"


Brave::~Brave()
{
	m_braveAnimClip.release();
	WeaponManager::DeleteInstance();
}

bool Brave::Start()
{
	EnWeaponType firstWeaponType = enSwordShield;

	//アニメーションクリップロードクラスを生成
	m_braveAnimClip = std::make_unique<LoadBraveAnimationClips>();
	//アニメーションクリップをロードする
	m_braveAnimClip.get()->RoadWeaponsAnimClips();
	//アニメーションの最初の番号を設定
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(firstWeaponType);

	//モデル初期化
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_braveAnimClip.get()->GetBraveAnimationClip(),
		m_braveAnimClip.get()->GetNumAnimationClips(),
		enModelUpAxisZ
	);

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);

	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});


	//キャラコンを生成
	CreateCharacterController();
	m_charaCon.get()->Init(20.0f, 50.0f, m_position);

	//ステータスを初期化
	m_status.InitPlayerStatus("Brave");

	//ステート管理クラスを生成
	m_braveStateCotext = std::make_unique<BraveStateContext>();
	//初期化
	m_braveStateCotext.get()->Init(this, enBraveState_Idle);
	

	//各種コンポーネントのセッティング
	SettingDefaultComponent();


	//武器管理マネージャーを生成
	WeaponManager::CreateInstance();
	//初期化処理
	WeaponManager::GetInstance()->Init(enSwordShield,enGreateSword,enBowArrow);




	return true;
}

void Brave::Update()
{

	//武器の処理


	ButtonAction();

	//移動処理
	Movement();


	//回転が最後
	Rotation();

	m_braveStateCotext.get()->UpdateCurrentState();
	m_braveStateCotext.get()->PlayAnimationCurrentState();


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

void Brave::SetCurrentAnimationStartIndexNoForMainWeaponType()
{
	m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
		WeaponManager::GetInstance()->GetMainWeaponType()
	);
}

bool Brave::IsButtonAction()
{
	////アクション中は他の処理をしない
	//if (IsAction() == true)
	//{
	//	return true;
	//}


	//Attack();


	//ChangeWeapon();

	return false;
}

void Brave::ProcessCommonStateTransition()
{
	if (fabsf(GetMoveSpeed().x) >= 0.001f ||
		fabsf(GetMoveSpeed().z) >= 0.001f)
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Sprint);
	}
	else
	{
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_Idle);
	}
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
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//コントローラーの入力を受け付けないなら移動処理しない
	if (!m_playerContoller->IsControllerInputEnabledFlag()) return;

	//移動量を取得
	m_moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
		m_status, m_moveSpeed, m_playerContoller->GetLStickInput()
	);

	//仮のジャンプ処理
	if (m_playerContoller->IsButtonTrigger(enButtonA) && m_charaCon.get()->IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}

	//重力の計算
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//キャラコンを使って座標を移動
	m_position = m_charaCon.get()->Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.get()->IsOnGround()) {
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

void Brave::ButtonAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//攻撃処理
	AttackAction();

	//武器切り替え
	ChangeWeaponAction();

}

void Brave::AttackAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	//攻撃ボタン
	if (m_playerContoller->IsButtonTrigger(enButtonA))
	{

	}


}

void Brave::ChangeWeaponAction()
{
	//アクション中は他の処理をしない
	if (IsAction() == true)
	{
		return;
	}

	bool isTrigger = false;

	//サブ武器１に切り替える
	if (m_playerContoller->IsButtonTrigger(enButtonRB1))
	{
		//サブ武器１を切り替え武器にする
		WeaponManager::GetInstance()->ChangeSubWeaponTypeToChangeWeaponType();
		isTrigger = true;
	}
	//サブ武器２に切り替える
	if (m_playerContoller->IsButtonTrigger(enButtonLB1))
	{
		//サブ武器２を切り替え武器にする
		WeaponManager::GetInstance()->ChangeSubWeaponType2ToChangeWeaponType();
		isTrigger = true;
	}

	//ボタンを押していたら
	if (isTrigger)
	{
		//切り替える武器のアニメーションの最初の番号を切り替え対象武器の番号にする
		m_braveAnimClip.get()->SetCurrentWeaponAnimationStartIndexNo(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//ステートを切り替える
		m_braveStateCotext.get()->ChangeBraveState(enBraveState_ChangeWeapon);


		//アクション中にする
		ActionActive();
	}

}




void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}

void Brave::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

	//武器入れ替えタイミング
	if (wcscmp(eventName, L"ArmedSwordShield") == 0)
	{
		//武器自体と武器タイプを入れ替える
		WeaponManager::GetInstance()->ChangeArmedWeapon(
			WeaponManager::GetInstance()->GetChangeTargetWeaponType()
		);
		//切り替え武器タイプをメイン武器タイプに切り替える
		//WeaponManager::GetInstance()->ChangeChangeWeaponTypeToMainWeaponType();
	}

}
