#include "stdafx.h"
#include "SelectWeapon.h"
#include "ManagerPreCompile.h"
#include "Fade.h"
#include "Game.h"

//全角記号文字はグリフに含まれていないのでつかわない！！！

//todo　選んだ武器はそのままグレーにして置いておくか
//		二段階にするか

namespace {
	const Vector3 WEAPON_POSITION = { 460.0f,150.0f,0.0f };
	const Vector3 ROOM_POSITION = { 0.0f,-100.0f,-280.0f };

	const float ROTATION_SPEED = 3.0F;

	const float ADD_WEAPON_SCALE = 7.0f;

	const float FONT_SCALE = 1.8f;

	const Vector2 SELECT_BACK_SPRITE_POS = { 400.0f,0.0f };
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
	//画像の初期化
	InitSprite();


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

	//GoToPlayMode();

	ProcessSelectWeapon();

	RotationWeapon();

	
}

void SelectWeapon::RotationWeapon()
{
	m_rotation.AddRotationDegY(ROTATION_SPEED);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.SetRotation(m_rotation);
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Update();
}

void SelectWeapon::ProcessSelectWeapon()
{
	SelectWeaponManageState();
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
				return;
			}
		}
		//もし既に選択している武器なら選択状態を解除
		else
		{
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = false;
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

void SelectWeapon::SelectWeaponManageState()
{
	switch (m_enSelectWeaponOrder)
	{
	case SelectWeapon::enSelectWeaponOrder_MainWeapon:
		ProcessChoice(enSelectWeaponOrder_MainWeapon);
		//名前の色の処理
		ProcessWeaponName();
		break;
	case SelectWeapon::enSelectWeaponOrder_SubWeapon:
		ProcessChoice(enSelectWeaponOrder_SubWeapon);
		//名前の色の処理
		ProcessWeaponName();
		break;
	case SelectWeapon::enSelectWeaponOrder_Complete:
		ProcessComplete();
		break;
	default:
		break;
	}

	m_selectBarSprite.SetPosition(m_namePosForSelectMainWeapon[m_nowSelectWeaponNumber]);
	m_selectBarSprite.Update();
}

void SelectWeapon::ProcessComplete()
{
	//Bボタンを押したら、武器を選びなおす
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//選択状態をリセット
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			m_weaponInfo[num].m_isSelect = false;
		}
		//最初のメイン武器から選びなおし
		SetEnSelectWeaponOrder(enSelectWeaponOrder_MainWeapon);
		return;
	}

	GoToPlayMode();

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

	wchar_t SwordShield[255];
	swprintf_s(SwordShield, 255, L"ソード&シールド");
	wchar_t GrateSword[255];
	swprintf_s(GrateSword, 255, L"グレイトソード");
	wchar_t bow[255];
	swprintf_s(bow, 255, L"ボウ&アロー");

	//ソード＆シールドのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont,
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield],
		FONT_SCALE,
		SwordShield
	);

	//グレイトソードのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_TwoHandSword].m_weaponNameFont,
		m_namePosForSelectMainWeapon[enWeaponType_TwoHandSword],
		FONT_SCALE,
		GrateSword
	);
	//アローのフォントの初期化
	InitFontRender(
		m_weaponInfo[enWeaponType_Bow].m_weaponNameFont,
		m_namePosForSelectMainWeapon[enWeaponType_Bow],
		FONT_SCALE,
		bow
	);



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

void SelectWeapon::InitSprite()
{
	//選択バー
	m_selectBarSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBar.DDS", 1920, 256);
	m_selectBarSprite.SetPosition(m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_selectBarSprite.SetScale(g_vec3One);
	m_selectBarSprite.SetRotation(g_quatIdentity);
	m_selectBarSprite.Update();
	//背景
	m_selectBackSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBack.DDS", 1152, 1080);
	m_selectBackSprite.SetPosition(SELECT_BACK_SPRITE_POS);
	m_selectBackSprite.SetScale(g_vec3One);
	m_selectBackSprite.SetRotation(g_quatIdentity);
	m_selectBackSprite.Update();
}

void SelectWeapon::InitFontRender(
	FontRender& fontRender, 
	Vector2 position, 
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
	//選択背景
	m_selectBackSprite.Draw(rc);
	//選択バー
	m_selectBarSprite.Draw(rc);

	//フォント
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponInfo[num].m_weaponNameFont.Draw(rc);
	}

}
