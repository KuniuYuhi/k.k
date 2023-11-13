#include "stdafx.h"
#include "SelectWeapon.h"
#include "ManagerPreCompile.h"
#include "Fade.h"
#include "Game.h"

//�S�p�L�������̓O���t�Ɋ܂܂�Ă��Ȃ��̂ł���Ȃ��I�I�I

//todo�@�I�񂾕���͂��̂܂܃O���[�ɂ��Ēu���Ă�����
//		��i�K�ɂ��邩

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
	//����̍��W�̐ݒ�
	m_weaponPosition = WEAPON_POSITION;
	//����̊g�嗦�̐ݒ�
	m_scaleWeapon *= ADD_WEAPON_SCALE;
}

SelectWeapon::~SelectWeapon()
{
	//WeaponManager::DeleteInstance();
}

bool SelectWeapon::Start()
{
	//�E�F�|���}�l�[�W���[�쐬
	WeaponManager::CreateInstance();

	//�����̐ݒ�
	g_renderingEngine->SetAmbient({1.0f,1.0f,0.8f});
	//���핔���̏�����
	InitWeaponRoom();
	//����̏�����
	InitWeapon();
	//����̖��O�̏�����
	InitWeaponName();
	//�J�����̏�����
	InitCamera();
	//�摜�̏�����
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
		//�t�F�[�h���I����������
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
	//���̕���Ɍ���
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�I����ԂłȂ��Ȃ�
		if (m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect != true)
		{
			//���̕���Ɍ���
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = true;
			m_weaponInfo[m_nowSelectWeaponNumber].m_weaponOrder = weaponOrder;
			//�I�����镐��̃I�[�_�[��i�߂�
			m_enSelectWeaponOrder = static_cast<EnSelectWeaponOrder>(m_enSelectWeaponOrder + 1);
			//�I�[�_�[���S�Č��܂�����
			if (GetSelectWeaponOrder() == enSelectWeaponOrder_Complete)
			{
				return;
			}
		}
		//�������ɑI�����Ă��镐��Ȃ�I����Ԃ�����
		else
		{
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = false;
		}
		m_nowSelectWeaponNumber++;
		//���݂̑I��ԍ�������̎�ނ�葽���Ȃ�����
		//todo ���X�g�̃T�C�Y�ɂ�����
		if (m_nowSelectWeaponNumber >= enWeaponType_Num)
		{
			//�ŏ��ɖ߂�
			m_nowSelectWeaponNumber = 0;
		}
		return;
	}

	//��ɏオ��
	if (g_pad[0]->IsTrigger(enButtonUp) == true)
	{
		m_nowSelectWeaponNumber--;
		//���݂̑I��ԍ�������̎�ނ�葽���Ȃ�����
		//todo ���X�g�̃T�C�Y�ɂ�����
		if (m_nowSelectWeaponNumber < 0)
		{
			//�ŏ��ɖ߂�
			m_nowSelectWeaponNumber = enWeaponType_Num - 1;
		}
		return;
	}
	//���ɉ�����
	if (g_pad[0]->IsTrigger(enButtonDown) == true)
	{
		m_nowSelectWeaponNumber++;
		//���݂̑I��ԍ�������̎�ނ�葽���Ȃ�����
		//todo ���X�g�̃T�C�Y�ɂ�����
		if (m_nowSelectWeaponNumber >= enWeaponType_Num)
		{
			//�ŏ��ɖ߂�
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
		//���O�̐F�̏���
		ProcessWeaponName();
		break;
	case SelectWeapon::enSelectWeaponOrder_SubWeapon:
		ProcessChoice(enSelectWeaponOrder_SubWeapon);
		//���O�̐F�̏���
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
	//B�{�^������������A�����I�тȂ���
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�I����Ԃ����Z�b�g
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			m_weaponInfo[num].m_isSelect = false;
		}
		//�ŏ��̃��C�����킩��I�тȂ���
		SetEnSelectWeaponOrder(enSelectWeaponOrder_MainWeapon);
		return;
	}

	GoToPlayMode();

}

void SelectWeapon::ProcessWeaponName()
{
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		//���ɑI�΂�Ă��镐��Ȃ�F�ς��������Ȃ�
		if (m_weaponInfo[num].m_isSelect == true)
		{
			m_weaponInfo[num].m_weaponNameFont.SetColor(g_vec4Red);
		}
		//����ȊO�͔�
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
		//�������镐��̌���
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			//�I�΂ꂽ����Ȃ�
			if (m_weaponInfo[num].m_isSelect == true)
			{
				m_enWeaponTipe = static_cast<EnWeaponType>(num);
				switch (m_weaponInfo[num].m_weaponOrder)
				{
				case SelectWeapon::enSelectWeaponOrder_MainWeapon:
					//����̎�ނ����C������ɐݒ�
					WeaponManager::GetInstance()->SetMainWeapon(m_enWeaponTipe);
					break;
				case SelectWeapon::enSelectWeaponOrder_SubWeapon:
					//����̎�ނ��T�u����ɐݒ�
					WeaponManager::GetInstance()->SetSubWeapon(m_enWeaponTipe);
					break;
				default:
					break;
				}
			}
		}
		//�Q�[���N���X�ɑJ�ڌ���
		m_goToGameFlag = true;

		//g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
		//�t�F�[�h�J�n
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
	//����̃��f���̏�����
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

	//�X�V����
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
	swprintf_s(SwordShield, 255, L"�\�[�h&�V�[���h");
	wchar_t GrateSword[255];
	swprintf_s(GrateSword, 255, L"�O���C�g�\�[�h");
	wchar_t bow[255];
	swprintf_s(bow, 255, L"�{�E&�A���[");

	//�\�[�h���V�[���h�̃t�H���g�̏�����
	InitFontRender(
		m_weaponInfo[enWeaponType_SwordShield].m_weaponNameFont,
		m_namePosForSelectMainWeapon[enWeaponType_SwordShield],
		FONT_SCALE,
		SwordShield
	);

	//�O���C�g�\�[�h�̃t�H���g�̏�����
	InitFontRender(
		m_weaponInfo[enWeaponType_TwoHandSword].m_weaponNameFont,
		m_namePosForSelectMainWeapon[enWeaponType_TwoHandSword],
		FONT_SCALE,
		GrateSword
	);
	//�A���[�̃t�H���g�̏�����
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

	//�����_���王�_�܂ł̃x�N�g����ݒ�B300,400
	m_toCameraPos.Set(0.0f, 100.0f, 700.0f);
	//�J�������v���C���[�̌��ɂ���Ƃ��Ɏg��
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
	//�I���o�[
	m_selectBarSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBar.DDS", 1920, 256);
	m_selectBarSprite.SetPosition(m_namePosForSelectMainWeapon[enWeaponType_SwordShield]);
	m_selectBarSprite.SetScale(g_vec3One);
	m_selectBarSprite.SetRotation(g_quatIdentity);
	m_selectBarSprite.Update();
	//�w�i
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
	//����
	m_weaponRoomModel.Draw(rc);
	//�I�𒆂̕���
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Draw(rc);
	//�I��w�i
	m_selectBackSprite.Draw(rc);
	//�I���o�[
	m_selectBarSprite.Draw(rc);

	//�t�H���g
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponInfo[num].m_weaponNameFont.Draw(rc);
	}

}
