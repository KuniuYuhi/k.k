#include "stdafx.h"
#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "BattleStart.h"
#include "Player.h"
#include "Boss.h"

namespace {
	const float SECOND_MAX = 60.0f;

	const float PHASE_TIME_LIMMIT = 50.0f;
	const float BREAK_TIME_LIMMIT = 10.0f;
	const float SHORT_BREAK_LIMMIT = 5.0f;

	const Vector3 BOSS_START_POSITON = { 0.0f, 0.0f, 600.0f };

	const Vector3 PLAYER_START_POSITON = { 0.0f, 0.0f, -400.0f };
}

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager(EnGameSeenState startGameSeenState)
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
	//�Q�[���J�n���̃X�e�[�g��ݒ�
	m_enGameSeenState = startGameSeenState;
}

GameManager::~GameManager()
{
	//�L�������}�l�[�W���[�폜
	CharactersInfoManager::DeleteInstance();

	m_instance = nullptr;
}

void GameManager::Init()
{
	//�L�����N�^�[�̏��}�l�[�W���[�̐���
	CharactersInfoManager::CreateInstance();
}

void GameManager::CreateBattleStartClass()
{
	//�o�g���X�^�[�g�N���X����
	BattleStart* battleStart = NewGO<BattleStart>(0, "battlestart");
}

void GameManager::CreatePlayerClass()
{
	Player* player = NewGO<Player>(0, "player");
	//�������W�̐ݒ�
	player->SetPosition(PLAYER_START_POSITON);
	CharactersInfoManager::GetInstance()->SetPlayerInstance(player);
}

void GameManager::DeletePlayerClass()
{
	CharactersInfoManager::GetInstance()->DeletePlayerInstance();
}

void GameManager::CreateBoss()
{
	Boss* boss = NewGO<Boss>(0, "boss");
	//�������W�̐ݒ�
	boss->SetPosition(BOSS_START_POSITON);
	CharactersInfoManager::GetInstance()->SetBossInstance(boss);
}

void GameManager::DeleteBossClass()
{
	CharactersInfoManager::GetInstance()->DeleteBossInstance();
}

void GameManager::Execute()
{
	//���s���������珈�����Ȃ�
	if (GetOutComeState() != enOutComeState_None)
	{
		return;
	}
	//�������Ԃ̌v�Z
	CalcTimeLimmit();
	//�t�F�[�Y�̏���
	ProcessPhase();
	//���s����������
	IsOutComeDecided();
}

void GameManager::ProcessPhase()
{
	switch (m_enPhaseState)
	{
	case GameManager::EnPhaseState_Phase1:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_Phase2:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_Phase3:
		OnProcessPhaseTransition();
		break;
	case GameManager::EnPhaseState_BreakTime:
		OnProcessBreakTimeTransition();
		break;
	default:
		break;
	}
}

void GameManager::OnProcessPhaseTransition()
{
	switch (m_enPhaseStep)
	{
		//�t�F�[�Y�̐������Ԃ̌v�Z
	case GameManager::enPhaseStep_PhaseTime:
		CalcPhaseTime();
		break;
		//���x�e�̎��Ԃ̌v�Z
	case GameManager::enPhaseStep_ShortBreak:
		ProcessShortBreak();
		break;
	default:
		break;
	}
}

void GameManager::CalcPhaseTime()
{
	//��̃t�F�[�Y�̎��Ԃ��I�������
	if (m_phaseTimer > PHASE_TIME_LIMMIT)
	{
		//�t�F�[�Y�^�C�}�[�����Z�b�g
		m_phaseTimer = 0.0f;
		//�t�F�[�Y���I������̂ŁA�����X�^�[���폜����悤�ɂ���
		SetDAndCMonstersState(enMonsters_Delete);
		//���̃t�F�[�Y���x�e���ԂȂ�A�X�e�b�v��i�߂Ȃ�
		if ((m_enPhaseState + 1) == EnPhaseState_BreakTime)
		{
			//�x�e���ԃt�F�[�Y�ɐi��
			m_enPhaseState = static_cast<EnPhaseState>(m_enPhaseState + 1);
			return;
		}
		//���̃X�e�b�v�ɐi��
		m_enPhaseStep = enPhaseStep_ShortBreak;
		
	}
	else
	{
		//�^�C�}�[�����Z
		m_phaseTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::ProcessShortBreak()
{
	if (m_shortBreakTimer > SHORT_BREAK_LIMMIT)
	{
		//���x�e�^�C�}�[�����Z�b�g
		m_shortBreakTimer = 0.0f;
		//���̃t�F�[�Y�ɐi��
		m_enPhaseState = static_cast<EnPhaseState>(m_enPhaseState + 1);
		//���̃t�F�[�Y���x�e���ԂłȂ��Ȃ�A�����X�^�[����
		if (m_enPhaseState != EnPhaseState_BreakTime)
		{
			//�t�F�[�Y���؂�ւ�����̂ŁA�����X�^�[�𐶐�����
			SetDAndCMonstersState(enMonsters_Create);
		}
		//�X�e�b�v�����Z�b�g
		m_enPhaseStep = enPhaseStep_PhaseTime;
	}
	else
	{
		//�^�C�}�[�����Z
		m_shortBreakTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::OnProcessBreakTimeTransition()
{
	//��莞�Ԍo������x�e�I��
	if (m_breakTimeTimer > BREAK_TIME_LIMMIT)
	{
		m_breakTimeTimer = 0.0f;
		//�t�F�[�Y����ԍŏ��ɖ߂�
		m_enPhaseState = EnPhaseState_Phase1;
		//�t�F�[�Y���؂�ւ�����̂ŁA�����X�^�[�𐶐�����
		SetDAndCMonstersState(enMonsters_Create);
		return;
	}
	else
	{
		//�^�C�}�[�����Z
		m_breakTimeTimer += g_gameTime->GetFrameDeltaTime();
	}

}

void GameManager::CalcTimeLimmit()
{
	//0�b�ȉ��Ȃ�
	if (m_second <= 0) {
		//1�����炷
		m_minute--;
		//����0���Ȃ�A�b��0�ɂ���
		if (m_minute < 0) {
			m_second = 0.0f;
			m_minute = 0.0f;
			//�������ԂɒB�����̂Ńv���C���[�̕���
			m_playerLoseFlag = true;
		}
		//60�b�ɖ߂�
		else
		{
			m_second = SECOND_MAX;
		}
	}
	else
	{
		//�b�����炷
		m_second -= g_gameTime->GetFrameDeltaTime();
	}
}

void GameManager::IsOutComeDecided()
{
	//����
	if (m_playerLoseFlag == true ||
		CharactersInfoManager::GetInstance()
		->GetPlayerInstance()->IsDeadPlayer() == true)
	{
		SetOutComeState(enOutComeState_PlayerLose);
	}
	//����
	if (m_playerWinFlag == true)
	{
		SetOutComeState(enOutComeState_PlayerWin);
	}
}






