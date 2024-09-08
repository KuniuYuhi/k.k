#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"

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

#include "GameFinishCamera.h"


#include "Loading.h"


#include "GameSceneManager.h"
#include "AllGameSceneState.h"



#include "CharacterBase.h"
#include "Brave.h"
#include "PlayerMovement.h"

//////////////////////////////
#include "Slime.h"
#include "Cactus.h"
#include "BeholderEye.h"

#include "Summoner.h"
//////////////////////////////

#include "EnemyObjectPool.h"
#include "WaveManager.h"

#include "EnemyManager.h"

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
	DeleteGO(m_waveManager);
	
	DeleteGO(m_gameCamera);
	DeleteGO(m_brave);
	DeleteGO(m_pause);
}

bool Game::Start()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	//���C�e�B���O�̏�����
	InitLighting();

	//�G�t�F�N�g�������N���X�̐����B
	InitEffect* initEffect = new InitEffect();
	initEffect->InitEFK();
	delete initEffect;

	//�Q�[���I�u�W�F�N�g�̏�����
	InitGameObject();

	//CreatePlayerAndCamera();

	//�t�F�[�h�N���X�̃C���X�^���X��T��
	m_fade = FindGO<Fade>("fade");
	//��ʂ𖾂邭����
	m_fade->StartFadeOut(2.0f);

	//��ʊE�[�x�̖�����
	g_renderingEngine->DisableDof();
	//�������C�g�̗L����
	g_renderingEngine->UseLimLight();

	//�����蔻��̉���
	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	


	EnemyManager::GetInstance()->CrearMobEnemyList();


	

	

	



	return true;
}

void Game::Update()
{	
	m_brave = FindGO<Brave>("Brave");

	if (m_brave != nullptr)
	{
		EnemyManager::GetInstance()->ControlEnemyDistances(m_brave->GetPosition());
	}

	
	

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
	m_summoner = NewGO<Summoner>(0, "Summoner");
	
}

void Game::UpdateGameSystem()
{
	
	
}

void Game::DeleteGameCamera()
{
	
	DeleteGO(m_gameCamera);
	
}

void Game::DeleteThis()
{
	m_summoner = FindGO<Summoner>("Summoner");
	if (m_summoner != nullptr)
	{
		//�{�X���폜
		m_summoner->DieFlomOutside();
	}

	//���g���폜
	DeleteGO(this);
}

void Game::CreatePlayerAndCamera()
{

	//�Q�[���J�����̐���
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//�v���C���[�̐���
	m_brave = NewGO<Brave>(0, "Brave");
}

void Game::CreateBattleWave()
{
	//�E�F�[�u�Ǘ��N���X����
	m_waveManager = NewGO<WaveManager>(0, "WaveManager");
}

void Game::InitSkyCube()
{
	m_skyCube = NewGO<SkyCube>(0, "skycube");
	m_skyCube->SetScale(400.0f);
	m_skyCube->SetPosition(m_skyPos);
	m_skyCube->SetLuminance(1.01f);
	m_skyCube->SetType(enSkyCubeType_DayToon_3);
	m_skyCube->Update();
}

void Game::GoResult()
{
	
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
	//m_gameUI->SetPlayer(CharactersInfoManager::GetInstance()->GetPlayerInstance());
	//m_gameUI->SetBoss(CharactersInfoManager::GetInstance()->GetBossInstance());
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