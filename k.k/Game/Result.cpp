#include "stdafx.h"
#include "Result.h"
#include "Title.h"

#include "GameManager.h"

namespace {
	const float WIPE_SIZE = 5.0f;

	const Vector3 WIN_TEXT_POS = { 41.0f,346.0f,0.0f };
	const Vector3 RIGHT_POS = { 38.0f,442.0f,0.0f };

	const Vector3 LOSE_TEXT_POS = { 0.0f,363.0f,0.0f };

	const Vector3 GO_TO_TITLE_POS = { 0.0f,-407.0f,0.0f };

	const float WIPE_SPEED = 12.0f;

	const float RIGHT_SPEED = 20.0f;

}

ResultSeen::ResultSeen()
{
}

ResultSeen::~ResultSeen()
{
	//�Q�[���}�l�[�W���[�̍폜
	GameManager::DeleteInstance();
}

bool ResultSeen::Start()
{
	switch (GameManager::GetInstance()->GetOutComeState())
	{
		//�v���C���[�̏���
	case GameManager::enOutComeState_PlayerWin:
		SetOutcome(enOutcome_Win);
		//����SE�Đ�
		g_soundManager->InitAndPlaySoundSource(enSoundName_GameClear);
		break;
		//�v���C���[�̔s�k
	case GameManager::enOutComeState_PlayerLose:
		SetOutcome(enOutcome_Lose);
		//�s�kSE�Đ�
		g_soundManager->InitAndPlaySoundSource(enSoundName_GameOver);
		break;

	default:
		std::abort();
		break;
	}
	m_resultSprite.Init("Assets/sprite/InGame/Result_Lose/Fade_Black.DDS", 1920, 1080);
	m_resultSprite.SetRoundWipe(true);
	m_resultSprite.SetRoundWipeStartPosition(1920.0f / 2, 1080.0f / 2);
	m_resultSprite.SetWipeSize(m_wipeSize);
	m_resultSprite.Update();


	InitSprite();

	return true;
}

void ResultSeen::Update()
{
	ManageState();
}

void ResultSeen::ManageState()
{
	switch (m_enOutComeState)
	{
	case ResultSeen::enOutcomeNone:
		//���s��������Ȃ�������N���b�V��������
		std::abort;
		break;
	case ResultSeen::enOutcome_Win:
		WinState();
		break;
	case ResultSeen::enOutcome_Lose:
		LoseState();
		break;
	default:
		break;
	}

}

void ResultSeen::WinState()
{
	switch (m_enResultStep)
	{
	case ResultSeen::enResultStep_main:
		OnProcessGameClearMainTranstion();
		break;
	case ResultSeen::enResultStep_end:
		OnProcessEndTranstion();
		break;
	default:
		break;
	}

	//���C�g�̉�]
	m_rightRotation.AddRotationDegZ(g_gameTime->GetFrameDeltaTime() * RIGHT_SPEED);
	m_rightRender.SetRotation(m_rightRotation);
	m_rightRender.Update();
}

void ResultSeen::LoseState()
{
	switch (m_enResultStep)
	{
	case ResultSeen::enResultStep_main:
		OnProcessGameOverMainTranstion();
		break;
	case ResultSeen::enResultStep_end:
		OnProcessEndTranstion();
		break;
	default:
		break;
	}
}

void ResultSeen::ResultEnd()
{
	//��ʂ������Ȃ�܂ŉ~�����������Ă���
	if (m_roundWipeEndFlag != true)
	{
		m_resultSprite.SetWipeSize(m_wipeSize);
		m_wipeSize -= WIPE_SPEED;
		//��ʑS�̂������Ȃ�����
		if (m_wipeSize < 0.0)
		{
			m_roundWipeEndFlag = true;
		}
	}
	//�~�`���C�v���I�������
	else
	{
		//�Q�[�����I����ă^�C�g���ɖ߂�
		m_resultEndFlag = true;
	}
}

void ResultSeen::OnProcessGameClearMainTranstion()
{
	//todo �����m������Ȃ�
	if (g_soundManager->GetSoundSource(enSoundName_GameClear) == nullptr)
	{
		//���̍Đ����I������玟�̃X�e�b�v�ɐi��
		m_enResultStep = enResultStep_end;
	}
}

void ResultSeen::OnProcessGameOverMainTranstion()
{
	if (g_soundManager->GetSoundSource(enSoundName_GameOver) == nullptr)
	{
		//���̍Đ����I������玟�̃X�e�b�v�ɐi��
		m_enResultStep = enResultStep_end;
	}
}

void ResultSeen::OnProcessEndTranstion()
{
	if (m_roundWipeStartFlag == true)
	{
		ResultEnd();
		return;
	}
	else
	{
		//�^�C�g���ɖ߂�悤�ɂ��邽�߂ɉ~�`���C�v���J�n
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_roundWipeStartFlag = true;
		}
	}
}

void ResultSeen::Render(RenderContext& rc)
{
	switch (m_enOutComeState)
	{
	case ResultSeen::enOutcome_Win:
		m_rightRender.Draw(rc);
		m_winTextRender.Draw(rc);
		break;
	case ResultSeen::enOutcome_Lose:
		m_loseTextRender.Draw(rc);
		break;
	default:
		break;
	}

	m_goToTitleRender.Draw(rc);

	m_resultSprite.Draw(rc);
}

void ResultSeen::InitSprite()
{
	m_winTextRender.Init("Assets/sprite/InGame/Result_Win/Win!.DDS", 860, 340);
	SettingSpriteRender(m_winTextRender, WIN_TEXT_POS, g_vec3One, g_quatIdentity);

	m_rightRender.Init("Assets/sprite/InGame/Result_Win/light.DDS", 1410, 1410);
	SettingSpriteRender(m_rightRender, RIGHT_POS, g_vec3One, g_quatIdentity);

	m_loseTextRender.Init("Assets/sprite/InGame/Result_Lose/Lose_2.DDS", 1290, 265);
	SettingSpriteRender(m_loseTextRender, LOSE_TEXT_POS, g_vec3One, g_quatIdentity);

	m_goToTitleRender.Init("Assets/sprite/InGame/Result_Win/GoToTitle.DDS", 520, 70);
	SettingSpriteRender(m_goToTitleRender, GO_TO_TITLE_POS, g_vec3One, g_quatIdentity);


}
