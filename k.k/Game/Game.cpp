#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "BossStage1.h"
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

#include "GameFinishCamera.h"
#include "BattlePhase.h"

#include "Loading.h"


#include "GameSceneManager.h"
#include "AllGameSceneState.h"


namespace {

	//�X�J�C�L���[�u�̏����̖��邳
	const float START_SKY_CUBE_LMINANCE = 1.02f;

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.1f, 0.1f, 0.1f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);

	//�{�X�ƃv���C���[�̐���������W
	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 600.0f);
	const Vector3 PLAYER_CREATE_POSITION = Vector3(0.0f, 0.0f, -400.0f);

	const float AMBIENT_COLOR = 1.0f;

	const float SECOND = 60.0f;

	const float DEFAULT_BATTLE_BGM = 2.0f;

	const float MUTE_SPEED = 5.0f;
}

Game::Game()
{
}

Game::~Game()
{
	//�S�Ẳ����X�g�b�v
	g_soundManager->StopAllSound();

	DeleteGO(m_gameUI);

	DeleteGO(m_skyCube);
	DeleteGO(m_bossStage1);
	DeleteGO(m_player);
	DeleteGO(m_boss);
	DeleteGO(m_gameCamera);
	DeleteGO(m_gameFinishCamera);
	DeleteGO(m_battlePhase);
	DeleteGO(m_pause);
	DeleteGO(m_result);
}

bool Game::Start()
{
	//���C�e�B���O�̏�����
	InitLighting();

	//�Q�[���i�s�}�l�[�W���[�N���X�̐���
	//�Q�[���V�[���X�e�[�g�̐ݒ�
	GameManager::CreateInstance(GameManager::enGameSeenState_GameStart);
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
	//�������C�g�̗L����
	g_renderingEngine->UseLimLight();

	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	return true;
}

void Game::Update()
{	
	
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
	//�{�X�N���X�̐���
	GameManager::GetInstance()->CreateBoss();
	m_boss = CharactersInfoManager::GetInstance()->GetBossInstance();
	//�����X�^�[�����ȏ�߂Â��Ȃ��悤�ɖ��t���[�����ׂ�
	//�����擾�ł���悤�ɂ���
}

void Game::UpdateGameSystem()
{
	//�Q�[���}�l�[�W���[�̖��t���[���̍X�V����
	GameManager::GetInstance()->Execute();
	//�L�����N�^�[���}�l�[�W���[�̖��t���[���̍X�V����
	CharactersInfoManager::GetInstance()->Execute();
}

void Game::DeleteGameCamera()
{
	
	DeleteGO(m_gameCamera);
	
}

void Game::DeleteThis()
{
	DeleteGO(this);
}

void Game::CreatePlayerAndCamera()
{
	//�v���C���[�N���X�̐���
	GameManager::GetInstance()->CreatePlayerClass();
	m_player = CharactersInfoManager::GetInstance()->GetPlayerInstance();
	//�Q�[���J�����̐���
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");
}

void Game::CreateBattlePhase()
{
	//�o�g���t�F�[�Y�����N���X����
	m_battlePhase = NewGO<BattlePhase>(0, "battlephase");
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetScale(800.0f);
	m_skyCube->SetPosition(m_skyPos);
	m_skyCube->SetLuminance(1.01f);
	m_skyCube->SetType(enSkyCubeType_DayToon_3);
	m_skyCube->Update();
}

void Game::GoResult()
{
	if (m_result == nullptr)
	{
		m_result = NewGO<ResultSeen>(0, "result");
	}
	//���U���g�̏������I�������A�Q�[�����I��鏈��������
	if (m_result->GetResultEndFlag() == true)
	{
		//�Ō�̃X�e�[�g�ɐi��
		GameManager::GetInstance()->
			SetGameSeenState(GameManager::enGameSeenState_GameEnd);
	}
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

void Game::CreateGameUI()
{
	m_gameUI = NewGO<GameUI>(0, "gameUI");
	m_gameUI->SetGame(this);
	m_gameUI->SetPlayer(CharactersInfoManager::GetInstance()->GetPlayerInstance());
	m_gameUI->SetBoss(CharactersInfoManager::GetInstance()->GetBossInstance());
}

void Game::InitGameObject()
{
	//�X�J�C�L���[�u�̏�����
	InitSkyCube();
	//�{�X�X�e�[�W�̐���
	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");
	
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