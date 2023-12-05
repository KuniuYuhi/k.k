#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
#include "Lich.h"
#include "Result.h"
#include "GameUI.h"
#include "Fade.h"
#include "EntryBoss.h"
#include "BattleStart.h"
#include "Pause.h"
#include "Title.h"

#include "SkyCube.h"
#include "InitEffect.h"

#include "CharactersInfoManager.h"
#include "GameManager.h"

#include "Boss.h"

//todo ���b�`�������鎞�ɑ����鏟�s���������ĂȂ��̂ɕς���Ă���

namespace {

	//�X�J�C�L���[�u�̏����̖��邳
	const float START_SKY_CUBE_LMINANCE = 1.07f;

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.5f, 0.5f, 0.5f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);

	//�{�X�ƃv���C���[�̐���������W
	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 600.0f);
	const Vector3 PLAYER_CREATE_POSITION = Vector3(0.0f, 0.0f, -400.0f);

	const float AMBIENT_COLOR = 0.7f;

	const float SECOND = 60.0f;

	const float DEFAULT_BATTLE_BGM = 2.0f;

	const float MUTE_SPEED = 5.0f;
}

Game::Game()
{
}

Game::~Game()
{
	g_soundManager->StopAllSound();

	DeleteGO(m_skyCube);
	DeleteGO(m_bossStage1);
	
	GameManager::GetInstance()->DeletePlayerClass();
	GameManager::GetInstance()->DeleteBossClass();
	DeleteGO(m_player);
	DeleteGO(m_boss);

	DeleteGO(m_gameCamera);
	DeleteGO(m_pause);

	if (m_gameUI != nullptr)
	{
		DeleteGO(m_gameUI);
	}
}

bool Game::Start()
{
	//���C�e�B���O�̏�����
	InitLighting();

	//�Q�[���i�s�}�l�[�W���[�N���X�̐���
	//�Q�[���V�[���X�e�[�g�̐ݒ�
	GameManager::CreateInstance(GameManager::enGameSeenState_Game);
	//����������
	GameManager::GetInstance()->Init();

	//�G�t�F�N�g�������N���X�̐����B
	InitEffect* initEffect = new InitEffect();
	initEffect->InitEFK();
	delete initEffect;

	//�Q�[���I�u�W�F�N�g�̏�����
	InitGameObject();

	//�t�F�[�h�N���X�̃C���X�^���X��T��
	m_fade = FindGO<Fade>("fade");
	//��ʂ𖾂邭����
	m_fade->StartFadeOut(2.0f);
	//��ʊE�[�x�̖�����
	g_renderingEngine->DisableDof();
	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	return true;
}

void Game::Update()
{	
	ManageState();
}

bool Game::Fadecomplete()
{
	//�����łȂ��Ȃ�
	if (m_fade->GetCurrentAlpha() != 0.0f)
	{
		return false;
	}
	//�����Ȃ�
	return true;
}

void Game::CreateBoss()
{
	//m_boss = NewGO<Boss>(0, "boss");
	//�{�X�N���X�̐���
	GameManager::GetInstance()->CreateBoss();
	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	/*m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition(BOSS_CREATE_POSITION);*/
	//���b�`�̃C���X�^���X����
	//CharactersInfoManager::GetInstance()->SetLichInstance(m_lich);
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetScale(800.0f);
	m_skyCube->SetPosition(m_skyPos);
	m_skyCube->SetLuminance(1.07f);
	m_skyCube->SetType(enSkyCubeType_DayToon_3);
	m_skyCube->Update();
}

void Game::GoResult()
{
	if (m_result == nullptr)
	{
		m_result = NewGO<ResultSeen>(0, "result");
	}
	//��ʂ����U���g�̉摜�ɂȂ���
	//�~�`���C�v���I�������
	//todo�~�`���C�v�̊ԉ������������Ă���
	if (m_result->GetRoundWipeEndFlag() == true)
	{
		DeleteGO(this);
	}
}

bool Game::IsWinnerDecision()
{
	return GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameOver ||
		GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameClear;
}

void Game::CalcMuteBGM()
{
	if (m_muteBGMFlag != true)
	{
		return;
	}

	m_bgmVolume = Math::Lerp(g_gameTime->GetFrameDeltaTime() * MUTE_SPEED, m_bgmVolume, 0.0f);

	if (m_bgmVolume < 0.1f)
	{
		m_muteBGMFlag = false;
		g_soundManager->StopSound(enSoundName_BattleBGM);
		return;
	}
	else
	{
		g_soundManager->GetSoundSource(enSoundName_BattleBGM)->SetVolume(m_bgmVolume);
	}
}

void Game::ManageState()
{
	//�t���O�������Ă�����BGM������������
	CalcMuteBGM();

	switch (GameManager::GetInstance()->GetGameSeenState())
	{
		//�Q�[���X�^�[�g
	case GameManager::enGameSeenState_GameStart:
		OnProcessGameStartTransition();
		break;
		//�{�X�o�ꃀ�[�r�[
	case GameManager::enGameSeenState_AppearanceBoss:
		OnProcessAppearanceBossTransition();
		break;
		//�C���Q�[��
	case GameManager::enGameSeenState_Game:
		OnProcessGameTransition();
		break;
		//�|�[�Y���
	case GameManager::enGameSeenState_Pause:
		OnProcessPauseTransition();
		break;
		//�Q�[���I�[�o�[
	case GameManager::enGameSeenState_GameOver:
		OnProcessGameOverTransition();
		break;
		//�Q�[���N���A
	case GameManager::enGameSeenState_GameClear:
		OnProcessGameClearTransition();
		break;
	default:
		break;
	}
}

void Game::OnProcessGameStartTransition()
{
	//��ʂ����S�Ƀt�F�[�h�C��������
	if (Fadecomplete() != true && m_fade->IsFade() == false)
	{
		//�o�g���X�^�[�g�N���X�폜
		DeleteGO(m_battleStart);
		//���̃X�e�b�v�̃X�e�[�g�ɐ؂�ւ���
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_AppearanceBoss);
	}
	return;
}

void Game::OnProcessAppearanceBossTransition()
{
	//�{�X�̏o���A�j���[�V�������Đ�����B�I�������{�X����
	//�{�X�o�ꃀ�[�r�[���I�������
	if (m_bossMovieEndFlag == true)
	{
		//���̃X�e�[�g�Ɉڂ鎞
		//���[�r�[�p�̃��f��������
		DeleteGO(m_entryBoss);
		//�{�X�̃A�N�e�B�u��
		CreateBoss();
		//�X�e�[�g��؂�ւ���
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Game);
		return;
	}

	//�{�X�̓o�ꃀ�[�r�[�N���X����
	if (m_entryBoss == nullptr)
	{
		m_entryBoss = NewGO<EntryBoss>(0, "entryboss");
		m_entryBoss->SetPosition(BOSS_CREATE_POSITION);
		m_entryBoss->SetGame(this);
		m_entryBoss->SetSkyCube(m_skyCube);
	}
}

void Game::OnProcessGameTransition()
{
	switch (m_enGameStep)
	{
		//�t�F�[�h�A�E�g
	case Game::enGameStep_FadeOut:
		OnProcessGame_FadeOutTransition();
		break;
		//�t�F�[�h�C��
	case Game::enGameStep_FadeNone:
		OnProcessGame_FadeNoneTransition();
		break;
		//�o�g���J�n
	case Game::enGameStep_Battle:
		OnProcessGame_BattleTransition();
		break;
	default:
		break;
	}
}

void Game::OnProcessGameOverTransition()
{
	//�Q�[����ʂ��烊�U���g��ʂɑJ�ڂ���܂ł̏���
	GoResult();
}

void Game::OnProcessGameClearTransition()
{
	if (m_displayResultFlag == true)
	{
		GoResult();
		return;
	}
	//���U���g��ʂ��Ȃ���
	//���b�`������Ԃ̓J�����Ɉڂ�


	//m_lich = FindGO<Lich>("lich");
	//if (m_lich == nullptr)
	//{
	//	//�t���[�����[�g�𗎂Ƃ�
	//	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
	//	//�{�X�����Ȃ��Ȃ����̂ŃJ�����̑Ώۂ�ς���
	//	SetClearCameraState(Game::enClearCameraState_Player);
	//	//���U���g��ʕ\��
	//	m_displayResultFlag = true;
	//}

}

void Game::OnProcessPauseTransition()
{
	//�|�[�Y��ʂ̃Q�[���I���t���O��true�Ȃ�
	if (m_pause->GetGameExitFlag() == true)
	{
		//���[�f�B���O��ʋ���Ń^�C�g���ɖ߂�
		Title* title = NewGO<Title>(0, "title");
		DeleteGO(this);
		return;
	}

	//�X�^�[�g�{�^������������Q�[���ɖ߂�
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		//�|�[�Y��ʂ̍폜
		DeleteGO(m_pause);
		//�Q�[���ɖ߂�
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Game);
		return;
	}
}

void Game::OnProcessGame_FadeOutTransition()
{
	//�t�F�[�h��Ԃ��Ȃ��Ȃ�t�F�[�h�A�E�g����
	if (m_fade->IsFade() == false)
	{
		//UI����
		CreateGameUI();
		//���̃X�e�[�g�ɓ����ăt�F�[�h�A�E�g����Ƃ�
		m_fade->StartFadeOut(3.0f);
		//���̃X�e�b�v�ɐi��
		m_enGameStep = enGameStep_FadeNone;
	}
}

void Game::OnProcessGame_FadeNoneTransition()
{
	if (m_fade->IsFade() == false)
	{
		//BGM�̉��ʂ��グ��
		//BGM�̍Đ�
		g_soundManager->InitAndPlaySoundSource(enSoundName_BattleBGM, DEFAULT_BATTLE_BGM, false, true, true);
		//���̃X�e�b�v�ɐi��
		m_enGameStep = enGameStep_Battle;
	}
}

void Game::OnProcessGame_BattleTransition()
{
	//���s����������
	if (IsOutcome() == true)
	{
		return;
	}
	//�X�^�[�g�{�^������������|�[�Y���
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		//�|�[�Y��ʂ̐���
		m_pause = NewGO<Pause>(0, "pause");
		//�|�[�Y��ʃX�e�[�g�ɐ؂�ւ�
		GameManager::GetInstance()->SetGameSeenState(
			GameManager::enGameSeenState_Pause);
		return;
	}

	//�Q�[���}�l�[�W���[�̖��t���[���̍X�V����
	GameManager::GetInstance()->Execute();
}

bool Game::IsOutcome()
{
	//���s�����ĂȂ��Ȃ珈�����Ȃ�
	if (GameManager::GetInstance()->GetOutComeState() ==
		GameManager::enOutComeState_None)
	{
		return false;
	}

	//�v���C���[�̏����Ȃ�
	if (GameManager::GetInstance()->GetOutComeState() ==
		GameManager::enOutComeState_PlayerWin)
	{
		ProcessWin();
	}
	//�v���C���[�̕����Ȃ�
	else
	{
		ProcessLose();
	}
	return true;
}

void Game::ProcessWin()
{
	DeleteGO(m_gameUI);
	//�X�e�[�g��؂�ւ���
	GameManager::GetInstance()->SetGameSeenState(
		GameManager::enGameSeenState_GameClear);
	//�J���������b�`�Ɍ�����
	m_gameCamera->SetLich(m_lich);
	//�J���������Z�b�g����
	m_gameCamera->CameraRefresh();
	//BGM�������n�߂�
	m_muteBGMFlag = true;
	m_bgmVolume = g_soundManager->GetBGMVolume();
}

void Game::ProcessLose()
{
	DeleteGO(m_gameUI);
	//�X�e�[�g��؂�ւ���
	GameManager::GetInstance()->SetGameSeenState(
		GameManager::enGameSeenState_GameOver);
	//BGM�������n�߂�
	m_muteBGMFlag = true;
	m_bgmVolume = g_soundManager->GetBGMVolume();
}

void Game::CreateGameUI()
{
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gameUI->SetGame(this);
	m_gameUI->SetPlayer(CharactersInfoManager::GetInstance()->GetPlayerInstance());
	//m_gameUI->GetLich(m_lich);
	m_gameUI->SetBoss(CharactersInfoManager::GetInstance()->GetBossInstance());
}

void Game::InitGameObject()
{
	//�X�J�C�L���[�u�̏�����
	InitSkyCube();
	//�{�X�X�e�[�W�̐���
	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");
	//�v���C���[�N���X�̐���
	GameManager::GetInstance()->CreatePlayerClass();
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();
	//�Q�[���J�����̐���
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//�Q�[���V�[���X�e�[�g���Q�[���X�^�[�g�Ȃ�
	if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_GameStart)
	{
		//�o�g���X�^�[�g�N���X����
		m_battleStart = NewGO<BattleStart>(0, "battlestart");
	}
	//�Q�[���V�[���X�e�[�g���Q�[���Ȃ�
	else if (GameManager::GetInstance()->GetGameSeenState() ==
		GameManager::enGameSeenState_Game)
	{
		//�{�X�̐���
		CreateBoss();
	}
}

void Game::InitLighting()
{
	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = DIRECTION_RIGHT_COLOR;
	//�f�B���N�V�������C�g
	g_renderingEngine->SetDerectionLight(0, directionLightDir, directionLightColor);
	//����
	g_renderingEngine->SetAmbient(g_vec3One * AMBIENT_COLOR);
	//�������C�g
	g_renderingEngine->UseHemiLight();
}