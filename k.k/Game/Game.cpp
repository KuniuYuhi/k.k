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

#include "Slime.h"

#include "SkyCube.h"
#include "InitEffect.h"

namespace {
	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.6f, 0.6f, 0.6f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);

	//�{�X�ƃv���C���[�̐���������W
	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 600.0f);
	const Vector3 PLAYER_CREATE_POSITION = Vector3(0.0f, 0.0f, -400.0f);

	const float AMBIENT_COLOR = 0.7f;

	const float SECOND = 60.0f;

	const float DEFAULT_BATTLE_BGM = 2.0f;
}

Game::Game()
{
}

Game::~Game()
{
	g_soundManager->StopAllSound();

	DeleteGO(m_skyCube);
	DeleteGO(m_bossStage1);
	
	DeleteGO(m_player);
	DeleteGO(m_lich);

	DeleteGO(m_gameCamera);

	DeleteGO(m_gameUI);
}

bool Game::Start()
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

	//���x��2D
	//level2DSp.Init(
	//	"Assets/Level2D/testLevel2D.casl",
	//	[&](Level2DObjectData& objData) {
	//		if (objData.EqualObjectName("pointer_black") == true)
	//		{
	//			spriteTest.Init("Assets/sprite/pointer_black.DDS", 220.0f, 220.0f);
	//			spriteTest.SetPosition(objData.position);
	//			//spriteTest.SetGrayScale(true);
	//			spriteTest.Update();

	//			return true;
	//		}

	//		return false;
	//	});


	//�G�t�F�N�g�������N���X�̐����B
	InitEffect* initEffect = new InitEffect();
	initEffect->InitEFK();
	delete initEffect;

	//�t�F�[�h�N���X�̃C���X�^���X��T��
	m_fade = FindGO<Fade>("fade");
	//�X�J�C�L���[�u�̏�����
	InitSkyCube();

	m_bossStage1 = NewGO<BossStage1>(0, "bossstage1");
	m_player = NewGO<Player>(0, "player");
	m_player->SetPosition(PLAYER_CREATE_POSITION);
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//�{�X�̐����B��A�N�e�B�u�ɂ���
	CreateBoss();

	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//��ʂ𖾂邭����
	m_fade->StartFadeOut(2.0f);
	//�Q�[���X�e�[�g���X�^�[�g�ɂ���
	m_enGameState = enGameState_GameStart;

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
	m_lich = NewGO<Lich>(0, "lich");
	m_lich->SetPosition(BOSS_CREATE_POSITION);
	m_lich->Deactivate();
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

void Game::GoResult(EnOutCome outcome)
{
	if (m_result == nullptr)
	{
		m_result = NewGO<ResultSeen>(0, "result");
		switch (outcome)
		{
		case Game::enOutCome_Win:
			g_soundManager->InitAndPlaySoundSource(enSoundName_GameClear);
			m_result->SetOutcome(ResultSeen::enOutcome_Win);
			break;
		case Game::enOutCome_Lose:
			g_soundManager->InitAndPlaySoundSource(enSoundName_GameOver);
			m_result->SetOutcome(ResultSeen::enOutcome_Lose);
			break;
		default:
			break;
		}
	}
	//��ʂ����U���g�̉摜�ɂȂ���
	//�~�`���C�v���I�������
	if (m_result->GetRoundWipeEndFlag() == true)
	{
		DeleteGO(this);
	}
}

bool Game::CalcTimeLimit()
{
	//0�b�ȉ��Ȃ�
	if (m_second <= 0) {
		//1�����炷
		m_minute--;
		//����0���Ȃ�A�b��0�ɂ���
		if (m_minute < 0) {
			m_second = 0.0f;
			m_minute = 0.0f;
			//�������ԂɒB����
			// �Q�[���I�[�o�[
			return true;
		}
		//60�b�ɖ߂�
		else
		{
			m_second = SECOND;
		}
	}
	else
	{
		//�b�����炷
		m_second -= g_gameTime->GetFrameDeltaTime();
	}
	return false;
}

void Game::CalcMuteBGM()
{
	if (m_muteBGMFlag != false)
	{
		return;
	}

	m_bgmVolume = Math::Lerp(g_gameTime->GetFrameDeltaTime() * 5.0f, m_bgmVolume, 0.0f);

	if (m_bgmVolume < 0.1f)
	{
		m_muteBGMFlag = true;
		g_soundManager->StopSound(enSoundName_BattleBGM);
		return;
	}
	else
	{
		g_soundManager->GetSoundSource(enSoundName_BattleBGM)->SetVolume(m_bgmVolume);
	}
}

bool Game::IsBossMovieSkipTime()
{
	if (g_pad[0]->IsPress(enButtonA))
	{
		//3�b��������X�L�b�v
		if (m_bossMovieSkipTime < m_bossMovieSkipTimer)
		{
			return true;
		}
		else
		{
			m_bossMovieSkipTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	else
	{
		m_bossMovieSkipTimer = 0.0f;
	}

	return false;
}

void Game::ManageState()
{
	switch (m_enGameState)
	{
	case enGameState_Fade:
		break;
	case enGameState_GameStart:
		OnProcessGameStartTransition();
		break;
	case enGameState_AppearanceBoss:
		OnProcessAppearanceBossTransition();
		break;
	case enGameState_Game:
		OnProcessGameTransition();
		break;
	case enGameState_Pause:
		break;
	case enGameState_GameOver:
		OnProcessGameOverTransition();
		break;
	case enGameState_GameOver_TimeUp:
		OnProcessGameOverTransition();
		break;
	case enGameState_GameClear:
		OnProcessGameClearTransition();
		break;
	default:
		break;
	}



}

void Game::OnProcessGameStartTransition()
{
	//��x�����o�g���X�^�[�g�N���X����
	if (m_battleStart == nullptr)
	{
		m_battleStart = NewGO<BattleStart>(0, "battlestart");
	}

	//�t�F�[�h�A�E�g�d�؂�Ȃ��Ə������Ȃ�
	if (Fadecomplete() != true)
	{
		//��ʂ����S�Ƀt�F�[�h�C��������
		if (m_fade->IsFade()==false && m_enFadeState == enFadeState_StartToBoss)
		{

			DeleteGO(m_battleStart);
			//���̃X�e�b�v
			//�X�e�[�g��؂�ւ���
			SetNextGameState(enGameState_AppearanceBoss);
			//�J���������Z�b�g����
			m_gameCamera->CameraRefresh();

		}

		return;
	}
	//�v���C���[�����Ă���
	m_cameraZoomOutTimer += g_gameTime->GetFrameDeltaTime();
	//������x�Y�[��������
	if (m_cameraZoomOutTime < m_cameraZoomOutTimer)
	{
		//�t�F�[�h�C�����n�߂�
		m_enFadeState = enFadeState_StartToBoss;
		m_fade->StartFadeIn(3.0f);
	}
}

void Game::OnProcessAppearanceBossTransition()
{
	//�{�X�̏o���A�j���[�V�������Đ�����B�I�������{�X����

	//�t�F�[�h�ɓ����Ă����ԂȂ�
	//�t�F�[�h�C������������X�e�[�g��؂�ւ���
	if (m_fade->IsFade() == false && m_enFadeState!=enFadeState_None)
	{
		switch (m_enFadeState)
		{
		case Game::enFadeState_StartToBoss:
			//���̃X�e�[�g�ɓ����ăt�F�[�h�A�E�g����Ƃ�
			m_fade->StartFadeOut(3.0f);
			//�t�F�[�h�X�e�[�g���Ȃ��ɂ���
			m_enFadeState = enFadeState_None;
			break;
		case Game::enFadeState_BossToPlayer:
			//���̃X�e�[�g�Ɉڂ鎞
			//���[�r�[�p�̃��f��������
			DeleteGO(m_entryBoss);
			//�{�X�̃A�N�e�B�u��
			m_lich->Activate(); 
		
			//�X�e�[�g��؂�ւ���
			SetNextGameState(enGameState_Game);
			break;
		default:
			break;
		}
		return;
	}

	//�X�L�b�v����
	if (IsBossMovieSkipTime() == true)
	{
		//�t�F�[�h�C�����n�߂�
		m_enFadeState = enFadeState_BossToPlayer;
		m_fade->StartFadeIn(3.0f);
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
	//�{�X�̓o�ꃀ�[�r�[���I�������
	if (m_bossMovieEndFlag == true)
	{
		//�t�F�[�h�C�����n�߂�
		m_enFadeState = enFadeState_BossToPlayer;
		m_fade->StartFadeIn(3.0f);
	}
}

void Game::OnProcessGameTransition()
{
	switch (m_enGameStep)
	{
	case Game::enGameStep_FadeOut:
		OnProcessGame_FadeOutTransition();
		break;
	case Game::enGameStep_FadeNone:
		OnProcessGame_FadeNoneTransition();
		break;
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
	GoResult(enOutCome_Lose);
}

void Game::OnProcessGameClearTransition()
{
	//BGM������������
	CalcMuteBGM();


	if (m_displayResultFlag == true)
	{
		GoResult(enOutCome_Win);
		return;
	}
	//���U���g��ʂ��Ȃ���
	//���b�`������Ԃ̓J�����Ɉڂ�
	m_lich = FindGO<Lich>("lich");
	if (m_lich == nullptr)
	{
		//�t���[�����[�g�𗎂Ƃ�
		g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);
		//�{�X�����Ȃ��Ȃ�����J�����̑Ώۂ�ς���
		SetClearCameraState(Game::enClearCameraState_Player);
		//���U���g��ʕ\��
		m_displayResultFlag = true;
	}

}

void Game::OnProcessGame_FadeOutTransition()
{
	//�t�F�[�h��Ԃ��Ȃ��Ȃ�t�F�[�h�A�E�g����
	if (m_fade->IsFade() == false && m_enFadeState == enFadeState_BossToPlayer)
	{
		//UI����
		/*m_gameUI = NewGO<GameUI>(0, "gameUI");
		m_gameUI->GetGame(this);
		m_gameUI->GetPlayer(m_player);
		m_gameUI->GetLich(m_lich);*/
		//���̃X�e�[�g�ɓ����ăt�F�[�h�A�E�g����Ƃ�
		m_fade->StartFadeOut(3.0f);
		//�t�F�[�h�X�e�[�g���Ȃ��ɂ���
		m_enFadeState = enFadeState_None;
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
	//�������Ԃ̏���
	if (CalcTimeLimit() == true)
	{
		//�^�C���A�b�v
		DeleteGO(m_gameUI);
		//�X�e�[�g��؂�ւ���
		SetNextGameState(enGameState_GameOver_TimeUp);
	}

	//�Q�[���N���A
	//�{�X�����ꂽ��
	//���ꂽ�u�Ԃ̏���
	if (m_DeathBossFlag == true)
	{
		DeleteGO(m_gameUI);
		//�X�e�[�g��؂�ւ���
		SetNextGameState(enGameState_GameClear);
		//�J���������b�`�Ɍ�����
		m_gameCamera->SetLich(m_lich);
		//�J���������Z�b�g����
		m_gameCamera->CameraRefresh();
		//BGM�������n�߂�
		m_bgmVolume = g_soundManager->GetBGMVolume();
		return;
	}
	//�Q�[���I�[�o�[
	//�L�����N�^�[���S�ł�����
	if (m_playerAnnihilationFlag == true)
	{
		DeleteGO(m_gameUI);
		//�X�e�[�g��؂�ւ���
		SetNextGameState(enGameState_GameOver);
		//BGM�������n�߂�
		m_muteBGMFlag = true;
		//BGM������
		g_soundManager->StopSound(enSoundName_BattleBGM);
	}
}
