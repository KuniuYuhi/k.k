#include "stdafx.h"
#include "SelectWeapon.h"

#include "Fade.h"

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
}

bool SelectWeapon::Start()
{
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

	fade = FindGO<Fade>("fade");
	if (fade->IsFade() != true)
	{
		fade->StartFadeOut(10.0f);
	}

	return true;
}

void SelectWeapon::Update()
{
	ProcessSelectWeapon();

	RotationWeapon();

	
}

void SelectWeapon::Render(RenderContext& rc)
{
	m_weaponRoomModel.Draw(rc);

	m_weaponModel[m_nowSelectWeaponNumber].Draw(rc);

	//フォント
	/*for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponNameFont[num].Draw(rc);
	}*/
	m_weaponNameFont[m_nowSelectWeaponNumber].Draw(rc);

	//m_font.Draw(rc);

	//m_weaponNameFont[enWeaponType_SwordShield].Draw(rc);

	/*for (auto font : m_weaponNameFont)
	{
		font->Draw(rc);
	}*/

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
	m_weaponModel[enWeaponType_SwordShield].Init(
		"Assets/modelData/SelectWeaponBg/SwordShield.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, true, true
	);
	m_weaponModel[enWeaponType_TwoHandSword].Init(
		"Assets/modelData/SelectWeaponBg/TwoHandSword.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0, 0, enModelUpAxisZ, false, true, true
	);

	//更新処理
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponModel[num].SetTransform(
			m_weaponPosition, m_rotation, m_scaleWeapon
		);

		m_weaponModel[num].Update();
	}
}

void SelectWeapon::InitWeaponName()
{

	wchar_t SS[255];
	swprintf_s(SS, 255, L"ソード＆シールド");
	wchar_t BS[255];
	swprintf_s(BS, 255, L"グレイトソード");
	
	//フォントによって違うところでだけ設定
	m_weaponNameFont[enWeaponType_SwordShield].SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_weaponNameFont[enWeaponType_SwordShield].SetText(
		SS);
	m_weaponNameFont[enWeaponType_SwordShield].SetColor(g_vec4White);
	m_weaponNameFont[enWeaponType_SwordShield].SetScale(FONT_SCALE);
	m_weaponNameFont[enWeaponType_SwordShield].SetShadowParam(true, 2.0f, g_vec4Black);
	

	m_weaponNameFont[enWeaponType_TwoHandSword].SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_TwoHandSword]);
	m_weaponNameFont[enWeaponType_TwoHandSword].SetText(
		BS);
	m_weaponNameFont[enWeaponType_TwoHandSword].SetColor(g_vec4White);
	m_weaponNameFont[enWeaponType_TwoHandSword].SetScale(FONT_SCALE);
	m_weaponNameFont[enWeaponType_TwoHandSword].SetShadowParam(true, 2.0f, g_vec4Black);
	
	/*for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponNameFont[num].SetColor(g_vec4White);
		m_weaponNameFont[num].SetScale(FONT_SCALE);
		m_weaponNameFont[num].SetShadowParam(true, 2.0f, g_vec4Black);
	}*/

	m_font.SetPosition(
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_font.SetColor(g_vec4White);
	m_font.SetText(L"グレイトソード");
	m_font.SetShadowParam(true, 2.0f, g_vec4Black);

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
	m_weaponModel[m_nowSelectWeaponNumber].SetRotation(m_rotation);
	m_weaponModel[m_nowSelectWeaponNumber].Update();
}

void SelectWeapon::ProcessSelectWeapon()
{
	ProcessChoice();

}

void SelectWeapon::ProcessChoice()
{
	if (g_pad[0]->IsTrigger(enButtonDown) == true)
	{
		m_nowSelectWeaponNumber++;
		//現在の選択番号が武器の種類より多くなったら
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
