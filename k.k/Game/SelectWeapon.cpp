#include "stdafx.h"
#include "SelectWeapon.h"
#include "ManagerPreCompile.h"
#include "Fade.h"
#include "Game.h"

//�S�p�L�������̓O���t�Ɋ܂܂�Ă��Ȃ��̂ł���Ȃ��I�I�I

namespace {
	const Vector3 CAMERA_TARGET_POS = { 150.0f,200.0f,0.0f };
	//460,150,0
	const Vector3 WEAPON_POSITION = { 460.0f,150.0f,0.0f };

	const Vector3 WEAPON_MOVE_END_POSITION = { -180.0f,150.0f,0.0f };
	const Vector3 WEAPON_MOVE_START_POSITION = { -2080.0f,150.0f,0.0f };

	const Vector3 ROOM_POSITION = { 0.0f,-100.0f,-280.0f };

	const Vector3 READY_SPRITE_START_POSITION = { 0.0f,-830.0f,0.0f };
	const Vector3 READY_SPRITE_END_POSITION = { 0.0f,-430.0f,0.0f };
	//-40�̍�
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
	//�e�탉�C�g�̏�����
	SettingLight();
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
	//��ʊE�[�x�̗L����
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
	//�Q�[���ڍs�t���O�������Ă���Ȃ�
	if (m_goToGameFlag == true)
	{
		//�t�F�[�h���I����������
		if (m_fade->GetCurrentAlpha() >= 1.0f)
		{
			//�Q�[����ʂɑJ��
			Game* game = NewGO<Game>(0, "game");
			//���g���폜
			DeleteGO(this);
		}
		return;
	}
	//����I������
	ProcessSelectWeapon();
	//����̉�]����
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
		//���C������̑I������
	case SelectWeapon::enSelectWeaponOrder_MainWeapon:
		ProcessChoice(enSelectWeaponOrder_MainWeapon);
		//���O�̐F�̏���
		ProcessWeaponName();
		m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[m_nowSelectWeaponNumber]);
		m_selectBarSprite.Update();
		break;
		//�T�u����̑I������
	case SelectWeapon::enSelectWeaponOrder_SubWeapon:
		ProcessChoice(enSelectWeaponOrder_SubWeapon);
		//���O�̐F�̏���
		ProcessWeaponName();
		m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[m_nowSelectWeaponNumber]);
		m_selectBarSprite.Update();
		break;
		//���팈�܂�����
	case SelectWeapon::enSelectWeaponOrder_Complete:
		ProcessComplete();
		break;
	default:
		break;
	}
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
				//���C���������ʓ��Ɉړ������邽�߂ɉ��̕��킩�T��
				FindAndSetMainWeaponInfo();
				return;
			}
		}
		//�������ɑI�����Ă��镐��Ȃ�I����Ԃ�����
		else
		{
			//�I�����镐��̃I�[�_�[��߂�
			m_enSelectWeaponOrder = static_cast<EnSelectWeaponOrder>(m_enSelectWeaponOrder - 1);
			m_weaponInfo[m_nowSelectWeaponNumber].m_isSelect = false;
			m_weaponInfo[m_nowSelectWeaponNumber].m_weaponOrder = enSelectWeaponOrder_None;
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

void SelectWeapon::ProcessComplete()
{
	switch (m_enCompleteState)
	{
	case SelectWeapon::enCompleteState_OffScreenObject:
		//�E�B���h�E����ʊO�Ɉړ�������
		ScreenWeaponFontAndSprite(true, LERP_START_POS, LERP_END_POS);
		break;
	case SelectWeapon::enCompleteState_OnScreenObject:
		//�E�B���h�E����ʓ��Ɉړ�������
		ScreenWeaponFontAndSprite(false, LERP_START_POS, LERP_END_POS);
		break;
	case SelectWeapon::enCompleteState_OffScreenReadySprite:
		//����OK����ʊO�Ɉړ�
		MoveReadySpriteScreen(false,g_vec3Zero, READY_LERP_START_POSITION);
		break;
	case SelectWeapon::enCompleteState_OnScreenReadySprite:
		//����OK����ʓ��Ɉړ�
		MoveReadySpriteScreen(true,g_vec3Zero, READY_LERP_START_POSITION);
		break;
	case SelectWeapon::enCompleteState_Complete:
		//����I�ђ������Q�[���ɑJ�ڏ���
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
		//���̃X�e�[�g�ɑJ��
		m_enCompleteState = enCompleteState_OnScreenReadySprite;
		m_screenTimer = 0.0f;
		//��ʓ��Ɉړ����I������Ȃ�
		if (moveScreenFlag == false)
		{
			//���g�����Z�b�g
			m_moveWeapon = nullptr;
			//�ŏ��̃��C�����킩��I�тȂ���
			SetEnSelectWeaponOrder(enSelectWeaponOrder_MainWeapon);
			//�R���v���[�g�X�e�[�g�����Z�b�g
			m_enCompleteState = enCompleteState_OffScreenObject;
		}
		return;
	}
	else
	{
		m_screenTimer += g_gameTime->GetFrameDeltaTime() * 1.0f;
		//���`���
		Vector3 pos;
		pos.Lerp(m_screenTimer, start, end);
		if (moveScreenFlag == false)
		{
			pos *= 1.5f;
		}
		//�X�N���[���t���O�ɂ���Čv�Z�̎d����ς���
		ProcessMoveScreen(moveScreenFlag, pos);
	}
}

void SelectWeapon::ProcessMoveScreen(bool moveScreenFlag, const Vector3 movePos)
{
	//��ʊO�Ɉړ�������Ȃ�
	if (moveScreenFlag == true)
	{
		//�摜�̈ړ�
		MoveSpriteOffScreen(movePos);
		//����̈ړ�
		MoveInWeapon(movePos);
	}
	//��ʓ��Ɉړ�������Ȃ�
	else
	{
		//�摜�̈ړ�
		MoveSpriteOnScreen(movePos);
		//����̈ړ�
		MoveOutWeapon(movePos);
	}
	//�X�V����
	m_selectBarSprite.Update();
	m_selectBackSprite.Update();
}

void SelectWeapon::MoveSpriteOffScreen(Vector3 movePos)
{
	Vector3 newPos = g_vec3Zero;
	//�t�H���g
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		newPos = WEAPON_NAME_DEFAULT_POS[num] + movePos;
		//����̃t�H���g�̍��W
		if (m_weaponInfo[num].m_weaponNameFont.GetPosition().x >= WEAPON_NAME_MAX_POS[num].x)
		{
			m_weaponInfo[num].m_weaponNameFont.SetPosition(WEAPON_NAME_MAX_POS[num]);
		}
		else
			m_weaponInfo[num].m_weaponNameFont.SetPosition(newPos);

		//�I���o�[�Ɠ����ԍ��Ȃ�
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
	//�I��w�i�̍��W�̈ړ�
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
	//�t�H���g
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		//�����ʒu�ɖ߂��Ă���
		newPos = WEAPON_NAME_MAX_POS[num] - movePos;
		//����̃t�H���g�̍��W
		if (m_weaponInfo[num].m_weaponNameFont.GetPosition().x <= WEAPON_NAME_DEFAULT_POS[num].x)
		{
			m_weaponInfo[num].m_weaponNameFont.SetPosition(WEAPON_NAME_DEFAULT_POS[num]);
		}
		else
			m_weaponInfo[num].m_weaponNameFont.SetPosition(newPos);

		//�I���o�[�Ɠ����ԍ��Ȃ�
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
	//�I��w�i�̍��W
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
	//�I��w�i�̍��W
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
	//�I��w�i�̍��W
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
	//�^�C�}�[�̏���ɒB������A�܂��̓X�v���C�g�̈ړ����I�������
	if (m_readyscreenTimer > 1.0f||
		((m_readySpriteMoveFlags.m_readySpriteMoveEndFlag)&&
		(m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag)))
	{
		//��ʊO�ɏo���Ȃ�
		if (OnOrOffScreenFlag == false)
		{
			//�I�u�W�F�N�g����ʊO�Ɉړ�������X�e�[�g�ɑJ��
			m_enCompleteState = enCompleteState_OnScreenObject;
		}
		else
		//���̃X�e�[�g�ɑJ��
		m_enCompleteState = enCompleteState_Complete;
		//�^�C�}�[�ƃt���O�����Z�b�g
		m_readyscreenTimer = 0.0f;
		m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = false;
		m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = false;
		return;
	}
	//�^�C�}�[�����Z
	m_readyscreenTimer += g_gameTime->GetFrameDeltaTime()*2.5f;
	//����OK�̈ړ�����
	Vector3 movePos;
	movePos.Lerp(m_readyscreenTimer, start, end);
	Vector3 newPos = g_vec3Zero;
	if (OnOrOffScreenFlag)
	{
		newPos = READY_SPRITE_START_POSITION - movePos;
		//����ɒB������
		if (m_ReadySprite.GetPosition().y >= READY_SPRITE_END_POSITION.y)
		{
			m_ReadySprite.SetPosition(READY_SPRITE_END_POSITION);
			//����̈ʒu�Ɉړ������������̂Ńt���O�𗧂Ă�
			m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = true;
		}
		else
			m_ReadySprite.SetPosition(newPos);
	}	
	else
	{
		newPos = READY_SPRITE_END_POSITION + movePos;
		//����ɒB������
		if (m_ReadySprite.GetPosition().y <= READY_SPRITE_START_POSITION.y)
		{
			m_ReadySprite.SetPosition(READY_SPRITE_START_POSITION);
			//����̈ʒu�Ɉړ������������̂Ńt���O�𗧂Ă�
			m_readySpriteMoveFlags.m_readySpriteMoveEndFlag = true;
		}
		else
			m_ReadySprite.SetPosition(newPos);
	}
	//�w�i�̈ړ�����
	if (OnOrOffScreenFlag)
	{
		newPos = READY_BACK_SPRITE_START_POSITION - movePos;
		//����ɒB������
		if (m_ReadyBackSprite.GetPosition().y >= READY_BACK_SPRITE_END_POSITION.y)
		{
			m_ReadyBackSprite.SetPosition(READY_BACK_SPRITE_END_POSITION);
			//����̈ʒu�Ɉړ������������̂Ńt���O�𗧂Ă�
			m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = true;
		}
		else
			m_ReadyBackSprite.SetPosition(newPos);
	}
	else
	{
		newPos = READY_BACK_SPRITE_END_POSITION + movePos;
		//����ɒB������
		if (m_ReadyBackSprite.GetPosition().y <= READY_BACK_SPRITE_START_POSITION.y)
		{
			m_ReadyBackSprite.SetPosition(READY_BACK_SPRITE_START_POSITION);
			//����̈ʒu�Ɉړ������������̂Ńt���O�𗧂Ă�
			m_readySpriteMoveFlags.m_readyBackSpriteMoveEndFlag = true;
		}
		else
			m_ReadyBackSprite.SetPosition(newPos);
	}
	//�X�V
	m_ReadySprite.Update();
	m_ReadyBackSprite.Update();
}

void SelectWeapon::ProcessResetOrGoToPlay()
{
	//B�{�^������������A�����I�тȂ���
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�I����Ԃ����Z�b�g
		for (int num = 0; num < enWeaponType_Num; num++)
		{
			m_weaponInfo[num].m_isSelect = false;
			m_weaponInfo[num].m_weaponOrder = enSelectWeaponOrder_None;
			m_weaponInfo[num].m_weaponNameFont.SetColor(g_vec4White);
		}
		//�O�̃X�e�[�g�ɑJ�ځB�摜����ʓ��ɖ߂�
		m_enCompleteState = enCompleteState_OffScreenReadySprite;
		return;
	}
	//�Q�[���ɑJ�ڌ���
	GoToPlayMode();
}

void SelectWeapon::FindAndSetMainWeaponInfo()
{
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		if (m_weaponInfo[num].m_weaponOrder == enSelectWeaponOrder_MainWeapon)
		{
			//�V�������������ɒ��ړ�����
			m_moveWeapon = &m_weaponInfo[num];
			//���W���J�n���W��ݒ�ƍX�V
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

void SelectWeapon::SettingLight()
{
	//�����̐ݒ�
	g_renderingEngine->SetAmbient({ 1.0f,1.0f,0.8f });
	//�f�B���N�V�������C�g�̐ݒ�
	g_renderingEngine->SetDerectionLight(0, DILECTION_LIGHT, DILECTION_LIGHT_COLOR);
	//�|�C���g���C�g
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
		"Assets/modelData/SelectWeaponBg/BowArrow.tkm",
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
		WEAPON_NAME_DEFAULT_POS[enWeaponType_SwordShield],
		FONT_SCALE,
		SwordShield
	);

	//�O���C�g�\�[�h�̃t�H���g�̏�����
	InitFontRender(
		m_weaponInfo[enWeaponType_TwoHandSword].m_weaponNameFont,
		WEAPON_NAME_DEFAULT_POS[enWeaponType_TwoHandSword],
		FONT_SCALE,
		GrateSword
	);
	//�A���[�̃t�H���g�̏�����
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

	Vector3 pos = CAMERA_TARGET_POS + m_positionCamera;

	m_springCamera.SetTarget(CAMERA_TARGET_POS);
	m_springCamera.SetPosition(pos);
	g_camera3D->SetPosition(pos);
	m_springCamera.Update();
}

void SelectWeapon::InitSprite()
{
	//�I���o�[
	m_selectBarSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBar.DDS", 1920, 256);
	m_selectBarSprite.SetPosition(WEAPON_NAME_DEFAULT_POS[enWeaponType_SwordShield]);
	m_selectBarSprite.SetScale(g_vec3One);
	m_selectBarSprite.SetRotation(g_quatIdentity);
	m_selectBarSprite.Update();
	//�w�i
	m_selectBackSprite.Init("Assets/sprite/InGame/SelectWeapon/SelectBack.DDS", 1152, 1080);
	m_selectBackSprite.SetPosition(SELECT_BACK_SPRITE_POS);
	m_selectBackSprite.SetScale(g_vec3One);
	m_selectBackSprite.SetRotation(g_quatIdentity);
	m_selectBackSprite.Update();
	//����OK
	m_ReadySprite.Init("Assets/sprite/InGame/SelectWeapon/Ready.DDS", 1070, 228);
	m_ReadySprite.SetPosition(READY_SPRITE_START_POSITION);
	m_ReadySprite.SetScale(g_vec3One);
	m_ReadySprite.SetRotation(g_quatIdentity);
	m_ReadySprite.Update();
	//����OK�̔w�i
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
	//����
	m_weaponRoomModel.Draw(rc);
	//�I�𒆂̕���
	m_weaponInfo[m_nowSelectWeaponNumber].m_weaponModel.Draw(rc);

	if (m_moveWeapon != nullptr)
	{
		m_moveWeapon->m_weaponModel.Draw(rc);
	}

	//�I��w�i
	m_selectBackSprite.Draw(rc);
	//�I���o�[
	m_selectBarSprite.Draw(rc);

	//�t�H���g
	for (int num = 0; num < enWeaponType_Num; num++)
	{
		m_weaponInfo[num].m_weaponNameFont.Draw(rc);
	}

	//����OK
	m_ReadyBackSprite.Draw(rc);
	m_ReadySprite.Draw(rc);
	

}
