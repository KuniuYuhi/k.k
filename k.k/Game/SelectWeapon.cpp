#include "stdafx.h"
#include "SelectWeapon.h"
#include "ManagerPreCompile.h"
#include "Fade.h"
#include "Game.h"

//todo フォント配列にしたらエラー

//todo　選んだ武器はそのままグレーにして置いておくか
//		二段階にするか

namespace {
	const Vector3 WEAPON_POSITION = { 370.0f,150.0f,0.0f };
	const Vector3 ROOM_POSITION = { 0.0f,-100.0f,-280.0f };

	const float ROTATION_SPEED = 3.0F;

	const float ADD_WEAPON_SCALE = 7.0f;

	const float FONT_SCALE = 1.5f;
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
	WeaponManager::DeleteInstance();
}

bool SelectWeapon::Start()
{
	//ウェポンマネージャー作成
	WeaponManager::CreateInstance();

	//環境光の設定
	g_renderingEngine->SetAmbient({1.0f,1.0f,0.8f});
	//武器部屋の初期化
	InitWeaponRoom();
	//武器の初期化
	InitWeapon();
	//武器の名前の初期化
	InitWeaponName();
	//カメラの初期化
	InitCamera();

	m_fade = FindGO<Fade>("fade");
	if (m_fade->IsFade() != true)
	{
		m_fade->StartFadeOut(10.0f);
	}

	return true;
}

void SelectWeapon::Update()
{
	

	if (m_goToGameFlag == true)
	{
		//フェードが終わったら消す
		if (m_fade->GetCurrentAlpha() >= 1.0f)
		{
			Game* game = NewGO<Game>(0, "game");
			DeleteGO(this);
		}
		return;
	}

	GoToPlayMode();

	ProcessSelectWeapon();

	RotationWeapon();

	
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
		"Assets/modelData/character/Player/NewHero/Bow.tkm",
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

	wchar_t SS[255];
	swprintf_s(SS, 255, L"ソード＆シールド");
	wchar_t BS[255];
	swprintf_s(BS, 255, L"グレイトソード");
	
	m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont.SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont.SetColor(g_vec4White);
	m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont.SetText(L"ソード＆シールド");
	m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont.SetShadowParam(
		true, 2.0f, g_vec4Black
	);



	aaa.m_weaponNameFont.SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	aaa.m_weaponNameFont.SetColor(g_vec4White);
	aaa.m_weaponNameFont.SetText(L"グレイトソード");
	aaa.m_weaponNameFont.SetShadowParam(true, 2.0f, g_vec4Black);


	m_font[0].SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_font[0].SetColor(g_vec4White);
	m_font[0].SetText(L"グレイトソード");
	m_font[0].SetShadowParam(true, 2.0f, g_vec4Black);

}

void SelectWeapon::InitCamera()
{
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

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

	m_targetPosition.y += 200.0f;
	m_targetPosition.x += 150.0f;
	Vector3 pos = m_targetPosition + m_positionCamera;

	m_springCamera.SetTarget(m_targetPosition);
	m_springCamera.SetPosition(pos);

	m_springCamera.Update();
}

void SelectWeapon::RotationWeapon()
{
	m_rotation.AddRotationDegY(ROTATION_SPEED);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.SetRotation(m_rotation);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Update();
}

void SelectWeapon::ProcessSelectWeapon()
{
	ProcessChoice();

}

void SelectWeapon::ProcessChoice()
{
	/*if (g_pad[0]->IsTrigger(enButtonA))
	{
		EnWeaponType weaponNum;
		weaponNum = static_cast<EnWeaponType>(m_nowSelectWeaponNumber);

		WeaponManager::SetMainWeapon(weaponNum);
		return;
	}*/

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

void SelectWeapon::SelectWeaponManageState()
{

}

void SelectWeapon::ProcessWeaponName()
{

}

void SelectWeapon::GoToPlayMode()
{
	if (g_pad[0]->IsTrigger(enButtonA) && m_goToGameFlag != true)
	{
		m_goToGameFlag = true;

		//g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
		//フェード開始
		m_fade->StartFadeIn(2.0f);
	}
}

void SelectWeapon::Render(RenderContext& rc)
{
	//部屋
	m_weaponRoomModel.Draw(rc);
	//選択中の武器
	//m_canSelectWeapon[0]->m_weaponModel.Draw(rc);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Draw(rc);


	//m_font[0].Draw(rc);

	//m_canSelectWeapon[0]->m_weaponNameFont.Draw(rc);
	//フォント
	m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont.Draw(rc);

	//aaa.m_weaponNameFont.Draw(rc);
}
