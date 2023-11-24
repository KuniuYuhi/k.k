#include "stdafx.h"
#include "SelectWeapon.h"
#include "ManagerPreCompile.h"
#include "Fade.h"
#include "Game.h"

//全角記号文字はグリフに含まれていないのでつかわない！！！

namespace {
	const Vector3 CAMERA_TARGET_POS = { 150.0f,200.0f,0.0f };
	//460,150,0
	const Vector3 WEAPON_POSITION = { 460.0f,150.0f,0.0f };

	const Vector3 WEAPON_MOVE_END_POSITION = { -180.0f,150.0f,0.0f };
	const Vector3 WEAPON_MOVE_START_POSITION = { -2080.0f,150.0f,0.0f };

	const Vector3 ROOM_POSITION = { 0.0f,-100.0f,-280.0f };

	const Vector3 READY_SPRITE_START_POSITION = { 0.0f,-830.0f,0.0f };
	const Vector3 READY_SPRITE_END_POSITION = { 0.0f,-430.0f,0.0f };
	//-40の差
	const Vector3 READY_BACK_SPRITE_START_POSITION = { 0.0f,-790.0f,0.0f };
	const Vector3 READY_BACK_SPRITE_END_POSITION = { 0.0f,-390.0f,0.0f };

	const Vector3 READY_LERP_START_POSITION = { 0.0f,-1000.0f,0.0f };

	const float ROTATION_SPEED = 3.0F;

	const float ADD_WEAPON_SCALE = 7.0f;

	const float FONT_SCALE = 1.8f;

	const Vector3 SELECT_BACK_SPRITE_POS = { 400.0f,0.0f, 0.0f };
	const Vector3 SELECT_BACK_SPRITE_MAX_POS = { 2300.0f,0.0f,0.0f };

	const Vector3 LERP_START_POS = { 0.0f,0.0f,0.0f };

	const Vector3 LERP_END_POS = { 2000.0f,0.0f,0.0f };

	const Vector3 WEAPON_NAME_DEFAULT_POS[enWeaponType_Num] = {
		{50.0f,250.0f,0.0f},
		{120.0f,50.0f,0.0f},
		{190.0f,-150.0f,0.0f},
	};

	const Vector3 WEAPON_NAME_MAX_POS[enWeaponType_Num] = {
		{1950.0f,250.0f,0.0f},
		{2020.0f,50.0f,0.0f},
		{2090.0f,-150.0f,0.0f},
	};

	const Vector3 DILECTION_LIGHT = { 1.0f,-1.0f,-1.0f };
	const Vector3 DILECTION_LIGHT_COLOR = { 0.6f,0.5f,0.6f };
	const Vector3 POINT_LIGHT_POSITION = { 340.0f,300.0f,300.0f };
}

SelectWeapon::SelectWeapon()
{
	//武器の座標の設定
	m_weaponPosition = WEAPON_POSITION;
	//武器の拡大率の設定
	m_scaleWeapon *= ADD_WEAPON_SCALE;
}

SelectWeapon::~SelectWeapon()
{
	//WeaponManager::DeleteInstance();
}

bool SelectWeapon::Start()
{
	//ウェポンマネージャー作成
	WeaponManager::CreateInstance();
	//各種ライトの初期化
	SettingLight();
	//武器部屋の初期化
	InitWeaponRoom();
	//武器の初期化
	InitWeapon();
	//武器の名前の初期化
	InitWeaponName();
	//カメラの初期化
	InitCamera();
	//画像の初期化
	InitSprite();
	//被写界深度の有効化
	g_renderingEngine->EnableDof();

	m_fade = FindGO<Fade>("fade");
	if (m_fade->IsFade() != true)
	{
		m_fade->StartFadeOut(10.0f);
	}

	return true;
}

void SelectWeapon::Update()
{
	//ゲーム移行フラグが立っているなら
	if (m_goToGameFlag == true)
	{
		//フェードが終わったら消す
		if (m_fade->GetCurrentAlpha() >= 1.0f)
		{
			//ゲーム画面に遷移
			Game* game = NewGO<Game>(0, "game");
			//自身を削除
			DeleteGO(this);
		}
		return;
	}
	//武器選択処理
	ProcessSelectWeapon();
	//武器の回転処理
	RotationWeapon();
}

void SelectWeapon::RotationWeapon()
{
	m_rotation.AddRotationDegY(ROTATION_SPEED);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.SetPosition(WEAPON_POSITION);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.SetRotation(m_rotation);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Update();

	if (m_moveWeapon != nullptr)
	{
		m_moveWeapon->m_weaponModel.SetRotation(m_rotation);
		m_moveWeapon->m_weaponModel.Update();
	}
}

void SelectWeapon::ProcessSelectWeapon()
{
	SelectWeaponManageState();
}

void SelectWeapon::SelectWeaponManageState()
{
	switch (m_enSelectWeaponOrder)
	{
		//メイン武器の選択処理
	case SelectWeapon::enSelectWeaponOrder_MainWeapon:
		ProcessChoice(enSelectWeaponOrder_MainWeapon);
		//名前の色の処理
		ProcessWeaponName();
		m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[m_nowSelectWeaponNumber]);
		m_selectBarSprite.Update();
		break;
		//サブ武器の選択処理
	case SelectWeapon::enSelectWeaponOrder_SubWeapon:
		ProcessChoice(enSelectWeaponOrder_SubWeapon);
		//名前の色の処理
		ProcessWeaponName();
		m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[m_nowSelectWeaponNumber]);
		m_selectBarSprite.Update();
		break;
		//武器決まったら
	case SelectWeapon::enSelectWeaponOrder_Complete:
		ProcessComplete();
		break;
	default:
		break;
	}
}

void SelectWeapon::ProcessChoice(EnSelectWeaponOrder weaponOrder)
{
	//この武器に決定
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//選択状態でないなら
		if (m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect != true)
		{
			//この武器に決定
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = true;
			m_weaponInfo[m_nowSelectWeaponNumber].m_weaponOrder = weaponOrder;
			//選択する武器のオーダーを進める
			m_enSelectWeaponOrder = static_cast<EnSelectWeaponOrder>(m_enSelectWeaponOrder + 1);
			//オーダーが全て決まったら
			if (GetSelectWeaponOrder() == enSelectWeaponOrder_Complete)
			{
				//メイン武器を画面内に移動させるために何の武器か探す
				FindAndSetMainWeaponInfo();
				return;
			}
		}
		//もし既に選択している武器なら選択状態を解除
		else
		{
			//選択する武器のオーダーを戻す
			m_enSelectWeaponOrder = static_cast<EnSelectWeaponOrder>(m_enSelectWeaponOrder - 1);
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = false;
			m_weaponInfo[m_nowSelectWeaponNumber].m_weaponOrder = enSelectWeaponOrder_None;
		}
		m_nowSelectWeaponNumber++;
		//現在の選択番号が武器の種類より多くなったら
		//todo リストのサイズにしたい
		if (m_nowSelectWeaponNumber >= enWeaponType_Num)
		{
			//最初に戻す
			m_nowSelectWeaponNumber = 0;
		}
		return;
	}
	//上に上がる
	if (g_pad[0]->IsTrigger(enButtonUp) == true)
	{
		m_nowSelectWeaponNumber--;
		//現在の選択番号が武器の種類より多くなったら
		//todo リストのサイズにしたい
		if (m_nowSelectWeaponNumber < 0)
		{
			//最初に戻す
			m_nowSelectWeaponNumber = enWeaponType_Num - 1;
		}
		return;
	}
	//下に下がる
	if (g_pad[0]->IsTrigger(enButtonDown) == true)
	{
		m_nowSelectWeaponNumber++;
		//現在の選択番号が武器の種類より多くなったら
		//todo リストのサイズにしたい
		if (m_nowSelectWeaponNumber >= enWeaponType_Num)
		{
			//最初に戻す
			m_nowSelectWeaponNumber = 0;
		}
	}
}

void SelectWeapon::ProcessComplete()
{
	switch (m_enCompleteState)
	{
	case SelectWeapon::enCompleteState_OffScreenObject:
		//ウィンドウを画面外に移動させる
		ScreenWeaponFontAndSprite(true, LERP_START_POS, LERP_END_POS);
		break;
	case SelectWeapon::enCompleteState_OnScreenObject:
		//ウィンドウを画面内に移動させる
		ScreenWeaponFontAndSprite(false, LERP_START_POS, LERP_END_POS);
		break;
	case SelectWeapon::enCompleteState_OffScreenReadySprite:
		//準備OKを画面外に移動
		MoveReadySpriteScreen(false,g_vec3Zero, READY_LERP_START_POSITION);
		break;
	case SelectWeapon::enCompleteState_OnScreenReadySprite:
		//準備OKを画面内に移動
		MoveReadySpriteScreen(true,g_vec3Zero, READY_LERP_START_POSITION);
		break;
	case SelectWeapon::enCompleteState_Complete:
		//武器選び直しかゲームに遷移処理
		ProcessResetOrGoToPlay();
		break;
	default:
		break;
	}
}

void SelectWeapon::ScreenWeaponFontAndSprite(bool moveScreenFlag, Vector3 start, Vector3 end)
{
	if (m_screenTimer > 1.0f)
	{
		//次のステートに遷移
		m_enCompleteState = enCompleteState_OnScreenReadySprite;
		m_screenTimer = 0.0f;
		//画面内に移動し終わったなら
		if (moveScreenFlag == false)
		{
			//中身をリセット
			m_moveWeapon = nullptr;
			//最初のメイン武器から選びなおし
			SetEnSelectWeaponOrder(enSelectWeaponOrder_MainWeapon);
			//コンプリートステートをリセット
			m_enCompleteState = enCompleteState_OffScreenObject;
		}
		return;
	}
	else
	{
		m_screenTimer += g_gameTime->GetFrameDeltaTime() * 1.0f;
		//線形補間
		Vector3 pos;
		pos.Lerp(m_screenTimer, start, end);
		if (moveScreenFlag == false)
		{
			pos *= 1.5f;
		}
		//スクリーンフラグによって計算の仕方を変える
		ProcessMoveScreen(moveScreenFlag, pos);
	}
}

void SelectWeapon::ProcessMoveScreen(bool moveScreenFlag, const Vector3 movePos)
{
	//画面外に移動させるなら
	if (moveScreenFlag == true)
	{
		//画像の移動
		MoveSpriteOffScreen(movePos);
		//武器の移動
		MoveInWeapon(movePos);
	}
	//画面内に移動させるなら
	else
	{
		//画像の移動
		MoveSpriteOnScreen(movePos);
		//武器の移動
		MoveOutWeapon(movePos);
	}
	//更新処理
	m_selectBarSprite.Update();
	m_selectBackSprite.Update();
}

void SelectWeapon::MoveSpriteOffScreen(Vector3 movePos)
{
	Vector3 newPos = g_vec3Zero;
	//フォント
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		newPos = WEAPON_NAME_DEFAULT_POS[num] + movePos;
		//武器のフォントの座標
		if (m_weaponInfo[num].m_weaponNameFont.GetPosition().x >= WEAPON_NAME_MAX_POS[num].x)
		{
			m_weaponInfo[num].m_weaponNameFont.SetPosition(WEAPON_NAME_MAX_POS[num]);
		}
		else
			m_weaponInfo[num].m_weaponNameFont.SetPosition(newPos);

		//選択バーと同じ番号なら
		if (num == m_nowSelectWeaponNumber)
		{
			if (m_selectBarSprite.GetPosition().x >= WEAPON_NAME_MAX_POS[num].x)
			{
				m_selectBarSprite.SetPosition(WEAPON_NAME_MAX_POS[num]);
			}
			else
				m_selectBarSprite.SetPosition(newPos);
		}
	}
	//選択背景の座標の移動
	newPos = SELECT_BACK_SPRITE_POS + movePos;
	if (m_selectBackSprite.GetPosition().x >= SELECT_BACK_SPRITE_MAX_POS.x)
	{
		m_selectBackSprite.SetPosition(SELECT_BACK_SPRITE_MAX_POS);
	}
	else
		m_selectBackSprite.SetPosition(newPos);
}

void SelectWeapon::MoveSpriteOnScreen(Vector3 movePos)
{
	Vector3 newPos = g_vec3Zero;
	//フォント
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		//初期位置に戻っていく
		newPos = WEAPON_NAME_MAX_POS[num] - movePos;
		//武器のフォントの座標
		if (m_weaponInfo[num].m_weaponNameFont.GetPosition().x <= WEAPON_NAME_DEFAULT_POS[num].x)
		{
			m_weaponInfo[num].m_weaponNameFont.SetPosition(WEAPON_NAME_DEFAULT_POS[num]);
		}
		else
			m_weaponInfo[num].m_weaponNameFont.SetPosition(newPos);

		//選択バーと同じ番号なら
		if (num == m_nowSelectWeaponNumber)
		{
			if (m_selectBarSprite.GetPosition().x <= WEAPON_NAME_DEFAULT_POS[num].x)
			{
				m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[num]);
			}
			else
				m_selectBarSprite.SetPosition(newPos);
		}
	}

	newPos = SELECT_BACK_SPRITE_MAX_POS - movePos;
	//選択背景の座標
	if (m_selectBackSprite.GetPosition().x <= SELECT_BACK_SPRITE_POS.x)
	{
		m_selectBackSprite.SetPosition(SELECT_BACK_SPRITE_POS);
	}
	else
		m_selectBackSprite.SetPosition(newPos);
}

void SelectWeapon::MoveOutWeapon(Vector3 movePos)
{
	Vector3 newPos = g_vec3Zero;
	newPos = WEAPON_MOVE_END_POSITION - movePos;
	//選択背景の座標
	if (m_moveWeapon->m_weaponModel.GetPosition().x <= WEAPON_MOVE_START_POSITION.x)
	{
		m_moveWeapon->m_weaponModel.SetPosition(WEAPON_MOVE_START_POSITION);
	}
	else
		m_moveWeapon->m_weaponModel.SetPosition(newPos);
}

void SelectWeapon::MoveInWeapon(Vector3 movePos)
{
	Vector3 newPos = g_vec3Zero;
	newPos = WEAPON_MOVE_START_POSITION + movePos;
	//選択背景の座標
	if (m_moveWeapon->m_weaponModel.GetPosition().x >= WEAPON_MOVE_END_POSITION.x)
	{
		m_moveWeapon->m_weaponModel.SetPosition(WEAPON_MOVE_END_POSITION);
	}
	else
		m_moveWeapon->m_weaponModel.SetPosition(newPos);
}

void SelectWeapon::MoveReadySpriteScreen(
	bool OnOrOffScreenFlag, Vector3 start, Vector3 end)
{
	//タイマーの上限に達したら、またはスプライトの移動が終わったら
	if (m_readyscreenTimer > 1.0f||
		((m_readySpriteMoveFlags.m_readySpriteMoveEndFlag)&&
		(m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag)))
	{
		//画面外に出たなら
		if (OnOrOffScreenFlag == false)
		{
			//オブジェクトを画面外に移動させるステートに遷移
			m_enCompleteState = enCompleteState_OnScreenObject;
		}
		else
		//次のステートに遷移
		m_enCompleteState = enCompleteState_Complete;
		//タイマーとフラグをリセット
		m_readyscreenTimer = 0.0f;
		m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = false;
		m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = false;
		return;
	}
	//タイマーを加算
	m_readyscreenTimer += g_gameTime->GetFrameDeltaTime()*2.5f;
	//準備OKの移動処理
	Vector3 movePos;
	movePos.Lerp(m_readyscreenTimer, start, end);
	Vector3 newPos = g_vec3Zero;
	if (OnOrOffScreenFlag)
	{
		newPos = READY_SPRITE_START_POSITION - movePos;
		//上限に達したら
		if (m_ReadySprite.GetPosition().y >= READY_SPRITE_END_POSITION.y)
		{
			m_ReadySprite.SetPosition(READY_SPRITE_END_POSITION);
			//既定の位置に移動が完了したのでフラグを立てる
			m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = true;
		}
		else
			m_ReadySprite.SetPosition(newPos);
	}	
	else
	{
		newPos = READY_SPRITE_END_POSITION + movePos;
		//上限に達したら
		if (m_ReadySprite.GetPosition().y <= READY_SPRITE_START_POSITION.y)
		{
			m_ReadySprite.SetPosition(READY_SPRITE_START_POSITION);
			//既定の位置に移動が完了したのでフラグを立てる
			m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = true;
		}
		else
			m_ReadySprite.SetPosition(newPos);
	}
	//背景の移動処理
	if (OnOrOffScreenFlag)
	{
		newPos = READY_BACK_SPRITE_START_POSITION - movePos;
		//上限に達したら
		if (m_ReadyBackSprite.GetPosition().y >= READY_BACK_SPRITE_END_POSITION.y)
		{
			m_ReadyBackSprite.SetPosition(READY_BACK_SPRITE_END_POSITION);
			//既定の位置に移動が完了したのでフラグを立てる
			m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = true;
		}
		else
			m_ReadyBackSprite.SetPosition(newPos);
	}
	else
	{
		newPos = READY_BACK_SPRITE_END_POSITION + movePos;
		//上限に達したら
		if (m_ReadyBackSprite.GetPosition().y <= READY_BACK_SPRITE_START_POSITION.y)
		{
			m_ReadyBackSprite.SetPosition(READY_BACK_SPRITE_START_POSITION);
			//既定の位置に移動が完了したのでフラグを立てる
			m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = true;
		}
		else
			m_ReadyBackSprite.SetPosition(newPos);
	}
	//更新
	m_ReadySprite.Update();
	m_ReadyBackSprite.Update();
}

void SelectWeapon::ProcessResetOrGoToPlay()
{
	//Bボタンを押したら、武器を選びなおす
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//選択状態をリセット
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			m_weaponInfo[num].m_isSelect = false;
			m_weaponInfo[num].m_weaponOrder = enSelectWeaponOrder_None;
			m_weaponInfo[num].m_weaponNameFont.SetColor(g_vec4White);
		}
		//前のステートに遷移。画像を画面内に戻す
		m_enCompleteState = enCompleteState_OffScreenReadySprite;
		return;
	}
	//ゲームに遷移決定
	GoToPlayMode();
}

void SelectWeapon::FindAndSetMainWeaponInfo()
{
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		if (m_weaponInfo[num].m_weaponOrder == enSelectWeaponOrder_MainWeapon)
		{
			//新しく生成せずに直接動かす
			m_moveWeapon = &m_weaponInfo[num];
			//座標を開始座標を設定と更新
			m_moveWeapon->m_weaponModel.SetPosition(WEAPON_MOVE_START_POSITION);
			m_moveWeapon->m_weaponModel.Update();
			return;
		}
	}
}

void SelectWeapon::ProcessWeaponName()
{
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		//既に選ばれている武器なら色変え処理しない
		if (m_weaponInfo[num].m_isSelect == true)
		{
			m_weaponInfo[num].m_weaponNameFont.SetColor(g_vec4Red);
		}
		//それ以外は白
		else
		{
			m_weaponInfo[num].m_weaponNameFont.SetColor(g_vec4White);
		}
	}
}

void SelectWeapon::GoToPlayMode()
{
	if (g_pad[0]->IsTrigger(enButtonA) && m_goToGameFlag != true)
	{
		//装備する武器の決定
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			//選ばれた武器なら
			if (m_weaponInfo[num].m_isSelect == true)
			{
				m_enWeaponTipe = static_cast<EnWeaponType>(num);
				switch (m_weaponInfo[num].m_weaponOrder)
				{
				case SelectWeapon::enSelectWeaponOrder_MainWeapon:
					//武器の種類をメイン武器に設定
					WeaponManager::GetInstance()->SetMainWeapon(m_enWeaponTipe);
					break;
				case SelectWeapon::enSelectWeaponOrder_SubWeapon:
					//武器の種類をサブ武器に設定
					WeaponManager::GetInstance()->SetSubWeapon(m_enWeaponTipe);
					break;
				default:
					break;
				}
			}
		}
		//ゲームクラスに遷移決定
		m_goToGameFlag = true;

		//g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
		//フェード開始
		m_fade->StartFadeIn(2.0f);
	}
}

void SelectWeapon::SettingLight()
{
	//環境光の設定
	g_renderingEngine->SetAmbient({ 1.0f,1.0f,0.8f });
	//ディレクションライトの設定
	g_renderingEngine->SetDerectionLight(0, DILECTION_LIGHT, DILECTION_LIGHT_COLOR);
	//ポイントライト
	g_renderingEngine->SetPointLight(
		POINT_LIGHT_POSITION, { 10.0f,10.0f,10.0f }, {400.0f,3.2f,0.0f});
}

void SelectWeapon::InitWeaponRoom()
{
	m_weaponRoomModel.Init(
		"Assets/modelData/SelectWeaponBg/WeaponRoom.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, true, false
	);

	m_weaponRoomModel.SetScale(5.0f);
	m_weaponRoomModel.SetPosition(ROOM_POSITION);
	m_weaponRoomModel.Update();
}

void SelectWeapon::InitWeapon()
{
	//武器のモデルの初期化
	m_weaponInfo[enWeaponType_SwordShield].m_weaponModel.Init(
		"Assets/modelData/SelectWeaponBg/SwordShield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, true, true
	);

	m_weaponInfo[enWeaponType_TwoHandSword].m_weaponModel.Init(
		"Assets/modelData/SelectWeaponBg/TwoHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, true, true
	);

	m_weaponInfo[enWeaponType_Bow].m_weaponModel.Init(
		"Assets/modelData/SelectWeaponBg/BowArrow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ, false, true, true
	);

	//更新処理
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponInfo[num].m_weaponModel.SetTransform(
			m_weaponPosition, m_rotation, m_scaleWeapon
		);

		m_weaponInfo[num].m_weaponModel.Update();
	}
}

void SelectWeapon::InitWeaponName()
{

	wchar_t SwordShield[255];
	swprintf_s(SwordShield, 255, L"ソード&シールド");
	wchar_t GrateSword[255];
	swprintf_s(GrateSword, 255, L"グレイトソード");
	wchar_t bow[255];
	swprintf_s(bow, 255, L"ボウ&アロー");

	//ソード＆シールドのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont,
		WEAPON_NAME_DEFAULT_POS[enWeaponType_SwordShield],
		FONT_SCALE,
		SwordShield
	);

	//グレイトソードのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_TwoHandSword].m_weaponNameFont,
		WEAPON_NAME_DEFAULT_POS[enWeaponType_TwoHandSword],
		FONT_SCALE,
		GrateSword
	);
	//アローのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_Bow].m_weaponNameFont,
		WEAPON_NAME_DEFAULT_POS[enWeaponType_Bow],
		FONT_SCALE,
		bow
	);



}

void SelectWeapon::InitCamera()
{
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(5000.0f);

	//注視点から視点までのベクトルを設定。300,400
	m_toCameraPos.Set(0.0f, 100.0f, 700.0f);
	//カメラをプレイヤーの後ろにするときに使う
	m_positionCamera = m_toCameraPos;


	m_springCamera.Init(
		*g_camera3D,
		1000.0f,
		false,
		2.0f
	);

	Vector3 pos = CAMERA_TARGET_POS + m_positionCamera;

	m_springCamera.SetTarget(CAMERA_TARGET_POS);
	m_springCamera.SetPosition(pos);
	g_camera3D->SetPosition(pos);
	m_springCamera.Update();
}

void SelectWeapon::InitSprite()
{
	//選択バー
	m_selectBarSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBar.DDS", 1920, 256);
	m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[enWeaponType_SwordShield]);
	m_selectBarSprite.SetScale(g_vec3One);
	m_selectBarSprite.SetRotation(g_quatIdentity);
	m_selectBarSprite.Update();
	//背景
	m_selectBackSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBack.DDS", 1152, 1080);
	m_selectBackSprite.SetPosition(SELECT_BACK_SPRITE_POS);
	m_selectBackSprite.SetScale(g_vec3One);
	m_selectBackSprite.SetRotation(g_quatIdentity);
	m_selectBackSprite.Update();
	//準備OK
	m_ReadySprite.Init("Assets/sprite/InGame/SelectWeapon/Ready.DDS", 1070, 228);
	m_ReadySprite.SetPosition(READY_SPRITE_START_POSITION);
	m_ReadySprite.SetScale(g_vec3One);
	m_ReadySprite.SetRotation(g_quatIdentity);
	m_ReadySprite.Update();
	//準備OKの背景
	m_ReadyBackSprite.Init("Assets/sprite/InGame/SelectWeapon/Ready_Back.DDS", 2058, 421);
	m_ReadyBackSprite.SetPosition(READY_BACK_SPRITE_START_POSITION);
	m_ReadyBackSprite.SetScale(g_vec3One);
	m_ReadyBackSprite.SetRotation(g_quatIdentity);
	m_ReadyBackSprite.Update();


}

void SelectWeapon::InitFontRender(
	FontRender& fontRender, 
	Vector3 position, 
	float scale, 
	const wchar_t* name, 
	Vector4 color, 
	bool isShadowParam, float shadowOffset, Vector4 shadowColor)
{
	fontRender.SetPosition(position);
	fontRender.SetScale(scale);
	fontRender.SetColor(g_vec4White);
	fontRender.SetText(name);
	fontRender.SetShadowParam(
		isShadowParam, shadowOffset, shadowColor
	);
}

void SelectWeapon::Render(RenderContext& rc)
{
	//部屋
	m_weaponRoomModel.Draw(rc);
	//選択中の武器
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Draw(rc);

	if (m_moveWeapon != nullptr)
	{
		m_moveWeapon->m_weaponModel.Draw(rc);
	}

	//選択背景
	m_selectBackSprite.Draw(rc);
	//選択バー
	m_selectBarSprite.Draw(rc);

	//フォント
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponInfo[num].m_weaponNameFont.Draw(rc);
	}

	//準備OK
	m_ReadyBackSprite.Draw(rc);
	m_ReadySprite.Draw(rc);
	

}
