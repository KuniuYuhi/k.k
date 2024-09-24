#include "stdafx.h"
#include "EntryBoss.h"
#include "Game.h"
#include "Fade.h"

#include "GameSceneManager.h"


#include "EffectNumbers.h"

using namespace EffectNumbers;

namespace {
	//�X�J�C�L���[�u�̏����̖��邳
	const float START_SKY_CUBE_LMINANCE = 1.008f;
	const float END_SKY_CUBE_LMINANCE = 0.2f;

	//���f���̃T�C�Y
	const float SCALE_UP = 4.0f;
	//���f����Y���W�Ɖ�]
	const float DEFAULT_MODEL_POS_Y = 380.0f;
	const float DEFAULT__ROT_Y = 180.0f;
	//Y���W�̉��Z��
	const float ADD_MODEL_Y_UP = 45.0f;

	//L�AI�AC�AH�A�̕\���^�C�~���O��Y���W
	const float LICH_SPRITE_DRAW_TIMING = -20.0f;

	const float LERP_START_Y = 430.0f;

	const float LERP_END_Y = -268.0f;
	//S,U,M,M,O,N,E,R,�̏������W�ƍŏI�I�ȍ��W
	const Vector3 S_START_POS = { -1100.0f,LERP_START_Y,0.0f };
	const Vector3 S_END_POS = { -757.0f,LERP_END_Y,0.0f };

	const Vector3 U_START_POS = { -900.0f,LERP_START_Y,0.0f };
	const Vector3 U_END_POS = { -538.0f,LERP_END_Y,0.0f };

	const Vector3 M_1_START_POS = { -600.0f,LERP_START_Y,0.0f };
	const Vector3 M_1_END_POS = { -342.0f,LERP_END_Y,0.0f };

	const Vector3 M_2_START_POS = { 400.0f,LERP_START_Y,0.0f };
	const Vector3 M_2_END_POS = { -115.0f,LERP_END_Y,0.0f };

	const Vector3 O_START_POS = { 450.0f,LERP_START_Y,0.0f };
	const Vector3 O_END_POS = { 123.0f,LERP_END_Y,0.0f };

	const Vector3 N_START_POS = { 650.0f,LERP_START_Y,0.0f };
	const Vector3 N_END_POS = { 336.0f,LERP_END_Y,0.0f };

	const Vector3 E_START_POS = { 900.0f,LERP_START_Y,0.0f };
	const Vector3 E_END_POS = { 542.0f,LERP_END_Y,0.0f };

	const Vector3 R_START_POS = { 1100.0f,LERP_START_Y,0.0f };
	const Vector3 R_END_POS = { 740.0f,LERP_END_Y,0.0f };

	//LICH�摜�̃T�C�Y
	const Vector3 START_SCALE = { 4.0f,4.0f,4.0f };
	const Vector3 END_SCALE = g_vec3One;

	//BOSS�e�L�X�g�摜�̍��W
	const Vector3 BOSS_TEXT_POS = { 0.0f,-40.0f,0.0f };

	//A�{�^���������ƃX�L�b�v�̉摜�̍��W�ƃT�C�Y
	const Vector3 PRESS_A_POS = { 723.0f,-501.0f ,0.0f };
	const Vector3 PRESS_A_SCALE = { 0.6f,0.6f ,0.6f };

	//���@�w��Y���W�ƃT�C�Y
	const float ADD_CIRCLE_POS_Y = 5.0f;
	const float CIRICLE_EFFECT_SIZE = 15.0f;

	//�����̏����J���[�ƍŏI�I�ȃJ���[
	//0.6
	const float START_AMBIENT_COLOR = 1.0f;
	const float END_AMBIENT_COLOR = 0.01f;

	//�|�C���g���C�g
	const Vector3 ADD_POINT_LIGHT_POS = { 0.0f,50.0f,-140.0f };
	const Vector3 POINT_LIGHT_COLOR = { 14.0f, 7.0f, 12.0f };
	const Vector3 POINT_LIGHT_RANGE = { 1500.0f, 4.0f, 0.0f };

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.1f, 0.1f, 0.1f);

	const float MUTE_SPEED = 15.0f;

	const Vector3 START_POS = { 0.0f,670.0f,800.0f };
	const Vector3 CENTER_POS = { 700.0f,470.0f,0.0f };
	const Vector3 END_POS = { 160.0f,230.0f,-550.0f };

	const float BOSS_MOVIE_SKIP_TIME = 2.0f;

	//�{�X�𐶐�������W
	const Vector3 BOSS_CREATE_POSITION = Vector3(0.0f, 0.0f, 600.0f);

}

EntryBoss::EntryBoss()
{
}

EntryBoss::~EntryBoss()
{
	DeleteTask();
}

bool EntryBoss::Start()
{
	m_fade = FindGO<Fade>("fade");

	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_FogRemoval].Load("Assets/animData/character/Lich/FogRemoval.tka");
	m_animationClip[enAnimClip_FogRemoval].SetLoopFlag(false);

	m_model.Init(
		"Assets/modelData/character/Lich/Lich_real.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	SetPosition(BOSS_CREATE_POSITION);

	Vector3 pos = m_position;
	pos.y += ADD_CIRCLE_POS_Y;
	m_CircleEffect = NewGO<EffectEmitter>(0);
	m_CircleEffect->Init(enEffect_Boss_Summon_Circle);
	m_CircleEffect->Play();
	m_CircleEffect->SetPosition(pos);
	m_CircleEffect->SetScale(g_vec3One * CIRICLE_EFFECT_SIZE);
	m_CircleEffect->Update();

	//���f���̏����ݒ�
	m_position.y -= DEFAULT_MODEL_POS_Y;
	m_scale *= SCALE_UP;
	m_rotation.SetRotationDegY(DEFAULT__ROT_Y);
	m_model.SetTransform(m_position, m_rotation, m_scale);
	m_model.Update();

	//�|�C���g���C�g�̍��W
	Vector3 pointLightPos = m_position;
	pointLightPos.Add(ADD_POINT_LIGHT_POS);
	//�|�C���g���C�g
	g_renderingEngine->SetPointLight(
		pointLightPos,
		POINT_LIGHT_COLOR,
		POINT_LIGHT_RANGE
	);

	m_game = FindGO<Game>("game");

	m_skyCube = m_game->GetSkyCube();

	//�X�J�C�L���[�u�����邳���擾
	m_skyCube->SetLuminance(END_SKY_CUBE_LMINANCE);
	m_skyCube->Update();
	
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�摜�̓ǂݍ���
	InitSprite();
	//���`��ԂŎg���n�_�ƏI�_�̏�����
	InitLerpPosition();


	//����
	g_renderingEngine->SetAmbient({0.1f,0.1f,0.1f});
	//�����̊����̃J���[��ݒ�
	m_ambientColor = END_AMBIENT_COLOR;

	//�S�S�S�S�S�S�S�̍Đ�
	g_soundManager->InitAndPlaySoundSource(enSoundName_Gogogo, g_soundManager->GetSEVolume());
	//��ʊE�[�x�̖�����
	g_renderingEngine->DisableDof();

	//�������C�g�̖�����
	//g_renderingEngine->UnUseLimLight();
	g_renderingEngine->SetLimLIghtPower(5.0f);

	//�΂˃J�����̏������B
	m_springCamera.Init(
		*g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
		500.0f,			//�J�����̈ړ����x�̍ő�l�B
		false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
		1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
	);

	m_fade->StartFadeOut(3.0f);

	return true;
}

void EntryBoss::Update()
{
	//�S�Ă̏������������āA�t�F�[�h�C�����I�������
	if (m_completeFlag == true && m_fade->IsFade() == false)
	{
		//�o�ꃀ�[�r�[���I��������ƃQ�[���ɓ`����
		//m_game->SetBossMovieFlag(true);

		//�o��V�[�����I��������Ƃ��V�[���}�l�[�W���[
		GameSceneManager::GetInstance()->ChangeGameSceneState(enGameSceneState_Game);
	}
	else
	{
		//���[�r�[�X�L�b�v����
		BossMovieSkip();
		//��ʂ����X�ɈÂ�����B�Â��Ȃ����珈�����~�܂�
		slowlyDarkScreen();
		//�X�e�[�g�Ǘ�
		ManageState();
		//�J��������
		ChaseBossCamera();

		MamageActionBossState();

		Animation();
	}
	
	
	m_model.Update();
}

void EntryBoss::positionUp()
{
	//���W���グ�������珈�������Ȃ�
	if (m_positionUpEndFlag == true)
	{
		return;
	}

	if (m_position.y >= 0.0f)
	{
		m_position.y = 0.0f;
		SetNextAnimationState(enAnimationState_FogRemoval);
		m_positionUpEndFlag = true;
	}
	else
	{
		//�摜�̕\�����n�߂�
		if (m_position.y > LICH_SPRITE_DRAW_TIMING)
		{
			m_GoTextFlag = true;
		}
		//�����������オ��
		m_position.y += g_gameTime->GetFrameDeltaTime() * ADD_MODEL_Y_UP;
	}

	m_model.SetPosition(m_position);
	
}

void EntryBoss::InitSprite()
{
	m_summonerCharInfo[S].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/S.DDS", 310, 300);
	SettingSpriteRender(m_summonerCharInfo[S].m_CharRender, S_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[U].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/U.DDS", 280, 280);
	SettingSpriteRender(m_summonerCharInfo[U].m_CharRender, U_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[M_1].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/M_1.DDS", 290, 280);
	SettingSpriteRender(m_summonerCharInfo[M_1].m_CharRender, M_1_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[M_2].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/M_2.DDS", 270, 280);
	SettingSpriteRender(m_summonerCharInfo[M_2].m_CharRender, M_2_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[O].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/O.DDS", 240, 290);
	SettingSpriteRender(m_summonerCharInfo[O].m_CharRender, O_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[N].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/N.DDS", 280, 280);
	SettingSpriteRender(m_summonerCharInfo[N].m_CharRender, N_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[E].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/E.DDS", 320, 280);
	SettingSpriteRender(m_summonerCharInfo[E].m_CharRender, E_START_POS, START_SCALE, g_quatIdentity);
	m_summonerCharInfo[R].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/R.DDS", 330, 280);
	SettingSpriteRender(m_summonerCharInfo[R].m_CharRender, R_START_POS, START_SCALE, g_quatIdentity);
	
	m_bossTextRender.Init("Assets/sprite/InGame/BossAppearance/Boss.DDS", 284, 84);
	SettingSpriteRender(m_bossTextRender, BOSS_TEXT_POS, g_vec3One, g_quatIdentity);

	m_PressAButton.Init("Assets/sprite/InGame/BossAppearance/SkipText.DDS", 720, 76);
	SettingSpriteRender(m_PressAButton, PRESS_A_POS, PRESS_A_SCALE, g_quatIdentity);
}

void EntryBoss::InitLerpPosition()
{
	//�uS�v�̎n�_�ƏI�_
	m_summonerCharInfo[S].m_startPosition = S_START_POS;
	m_summonerCharInfo[S].m_endPosition = S_END_POS;
	//�uU�v�̎n�_�ƏI�_
	m_summonerCharInfo[U].m_startPosition = U_START_POS;
	m_summonerCharInfo[U].m_endPosition = U_END_POS;
	//�uM_1�v�̎n�_�ƏI�_
	m_summonerCharInfo[M_1].m_startPosition = M_1_START_POS;
	m_summonerCharInfo[M_1].m_endPosition = M_1_END_POS;
	//�uM_2�v�̎n�_�ƏI�_
	m_summonerCharInfo[M_2].m_startPosition = M_2_START_POS;
	m_summonerCharInfo[M_2].m_endPosition = M_2_END_POS;
	//�uO�v�̎n�_�ƏI�_
	m_summonerCharInfo[O].m_startPosition = O_START_POS;
	m_summonerCharInfo[O].m_endPosition = O_END_POS;
	//�uN�v�̎n�_�ƏI�_
	m_summonerCharInfo[N].m_startPosition = N_START_POS;
	m_summonerCharInfo[N].m_endPosition = N_END_POS;
	//�uE�v�̎n�_�ƏI�_
	m_summonerCharInfo[E].m_startPosition = E_START_POS;
	m_summonerCharInfo[E].m_endPosition = E_END_POS;
	//�uR�v�̎n�_�ƏI�_
	m_summonerCharInfo[R].m_startPosition = R_START_POS;
	m_summonerCharInfo[R].m_endPosition = R_END_POS;
}

void EntryBoss::SpriteMove()
{
	//�����J�E���g���Ō�ɒB������
	if (m_summonerTextCount == END)
	{
		//�t���O���Z�b�g
		m_drawBOSSTextFlag = true;
		return;
	}
	//��ԗ��̌v�Z
	m_time += g_gameTime->GetFrameDeltaTime() * 5.0f;

	//�����̍��W�̐��`���
	Vector3 LerpPos;
	LerpPos.Lerp(m_time, m_summonerCharInfo[m_summonerTextCount].m_startPosition, m_summonerCharInfo[m_summonerTextCount].m_endPosition);
	//�����̃T�C�Y�̐��`���
	Vector3 size;
	size.Lerp(m_time, START_SCALE, END_SCALE);
	//��]
	Quaternion rot = g_quatIdentity;

	//��b�������玟�̕����ɐ؂�ւ���
	if (m_time >= 1.0f)
	{
		//�����J�E���g��i�߂�
		m_summonerTextCount++;

		//���n�̉��Đ�
		g_soundManager->InitAndPlaySoundSource(
			enSoundName_BossNameDon,
			g_soundManager->GetBGMVolume()
		);
		m_time = 0.0f;
	}
	if (m_summonerTextCount == END)
	{
		return;
	}
	//���W�̐ݒ�
	m_summonerCharInfo[m_summonerTextCount].m_CharRender.SetTransForm(LerpPos, size, rot);
	//�X�V
	m_summonerCharInfo[m_summonerTextCount].m_CharRender.Update();
}

void EntryBoss::slowlyDarkScreen()
{
	//�Â��Ȃ����珈�����Ȃ�
	if (m_SlowlyDarkScreenEndFlag == true)
	{
		return;
	}

	m_lightTimer += g_gameTime->GetFrameDeltaTime() * 2.0f;

	//��ʂ��Â�����
	m_ambientColor = Math::Lerp(m_lightTimer, m_ambientColor, END_AMBIENT_COLOR);
	m_finalAmbientColor = g_vec3One * m_ambientColor;
	g_renderingEngine->SetAmbient(m_finalAmbientColor);

	//�������ő�܂ŏ������Ȃ�����
	if (m_ambientColor <= END_AMBIENT_COLOR)
	{
		m_SlowlyDarkScreenEndFlag = true;
		//���C�g�^�C�}�[���Z�b�g
		m_lightTimer = 0.0f;
	}
}

void EntryBoss::SlowlyBrightScreen()
{
	//�Â��Ȃ����珈�����Ȃ�
	if (m_SlowlyBrightScreenEndFlag == true)
	{
		return;
	}

	m_lightTimer += g_gameTime->GetFrameDeltaTime() * m_mulBrightTimer;

	m_mulBrightTimer += m_mulBrightTimer*0.1f;

	//��ʂ𖾂邭����
	m_ambientColor = Math::Lerp(m_lightTimer, m_ambientColor, START_AMBIENT_COLOR);
	m_finalAmbientColor = g_vec3One * m_ambientColor;
	g_renderingEngine->SetAmbient(m_finalAmbientColor);

	//��𖾂邭����
	m_skyLuminance = Math::Lerp(m_lightTimer, END_SKY_CUBE_LMINANCE, START_SKY_CUBE_LMINANCE);
	m_skyCube->SetLuminance(m_skyLuminance);
	m_skyCube->Update();

	//�������ő�܂ł��������Ȃ�����
	if (m_ambientColor >= START_AMBIENT_COLOR)
	{
		m_SlowlyBrightScreenEndFlag = true;
		g_renderingEngine->SetLimLIghtPower(1.5f);
	}
}

void EntryBoss::CompleteTime()
{
	//��������������炵�΂炭��~���Ă���
	if (m_endTime < m_endTimer)
	{
		//��邱�Ƃ��I������̂Ńt�F�[�h�C������
		m_fade->StartFadeIn(3.0f);
		//���������t���O�𗧂Ă�
		m_completeFlag = true;
	}
	else
	{
		m_endTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void EntryBoss::CalcMuteGogogoSE()
{
	if (m_muteFlag != true)
	{
		return;
	}
	//������`��Ԃŏ��������Ă���
	m_gogogoVolume = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * MUTE_SPEED, m_gogogoVolume, 0.1f);

	if (m_gogogoVolume <= 0.1f)
	{
		g_soundManager->StopSound(enSoundName_Gogogo);
		m_muteFlag = false;
	}
	else
	{
		//�{�����[����ݒ�
		g_soundManager->GetSoundSource(enSoundName_Gogogo)->SetVolume(m_gogogoVolume);
	}
}

void EntryBoss::ManageState()
{
	switch (m_enWholeState)
	{
	case EntryBoss::enWholeState_RiseBoss:
		OnProcessRiseBossTransition();
		break;
	case EntryBoss::enWholeState_FogRemoval:
		OnProcessFogRemovalTransition();
		break;
	case EntryBoss::enWholeState_Complete:
		OnProcessCompleteTransition();
		break;
	default:
		break;
	}
}

void EntryBoss::OnProcessRiseBossTransition()
{
	//�������S�ďo�āABOSS�̃e�L�X�g�摜�t���O��true
	// �ɂȂ����玟�̃X�e�[�g�ɑJ��
	if (m_drawBOSSTextFlag == true)
	{
		//�������X�e�[�g�ɑJ��
		m_enWholeState = enWholeState_FogRemoval;
		return;
	}

	//�{�X�̍��W���グ��
	positionUp();
	//LICH�̃e�L�X�g�摜�̕\���^�C�~���O
	if (m_GoTextFlag == true)
	{
		SpriteMove();
	}
}

void EntryBoss::OnProcessFogRemovalTransition()
{
	//�������G�t�F�N�g��nullpty�łȂ��Ȃ�
	if (m_FogRemovalEffect != nullptr)
	{
		//�S�S�S�S�S�S�������X�ɏ���������
		CalcMuteGogogoSE();
		//��ʂ����X�ɖ��邭����
		SlowlyBrightScreen();
		//�������G�t�F�N�g�����������
		if (m_FogRemovalEffect->IsPlay() == false)
		{
			//��邱�ƏI���
			//�Ō�̃X�e�[�g�ɑJ��
			m_enWholeState = enWholeState_Complete;
			return;

		}
	}
}

void EntryBoss::OnProcessCompleteTransition()
{
	//���΂炭�Î~���Ă���I���
	CompleteTime();
}

void EntryBoss::MamageActionBossState()
{
	switch (m_enAnimationState)
	{
	case EntryBoss::enAninationState_Idle:
		OnProcessCommonStateTransition();
		break;
	case EntryBoss::enAnimationState_FogRemoval:
		OnProcessFogRemovalStateTransition();
		break;
	default:
		break;
	}
}

void EntryBoss::Animation()
{
	switch (m_enAnimationState)
	{
	case EntryBoss::enAninationState_Idle:
		m_model.PlayAnimation(enAnimClip_Idle, 0.4f);
		break;
	case EntryBoss::enAnimationState_FogRemoval:
		m_model.PlayAnimation(enAnimClip_FogRemoval);
		break;
	default:
		break;
	}
}

void EntryBoss::OnProcessCommonStateTransition()
{
	SetNextAnimationState(enAninationState_Idle);
}

void EntryBoss::OnProcessFogRemovalStateTransition()
{
	if (m_model.IsPlayingAnimation() == false)
	{
		OnProcessCommonStateTransition();
	}
}

void EntryBoss::ChaseBossCamera()
{
	if (m_chaseBossTime > 1.0f)
	{
		return;
	}

	//�����_�̌v�Z
	m_target = GetPosition();
	m_target.y = 0.0f;

	//���`���
	m_pos1.Lerp(m_chaseBossTime, START_POS, CENTER_POS);
	m_pos2.Lerp(m_chaseBossTime, CENTER_POS, END_POS);
	m_toCameraPosForBoss.Lerp(m_chaseBossTime, m_pos1, m_pos2);

	m_chaseBossTime += g_gameTime->GetFrameDeltaTime() * 0.12f;
	//�J�����̒����_��ݒ�
	Vector3 finalCameraPos = m_toCameraPosForBoss + m_target;

	//���_�ƒ����_��ݒ�
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);
	//�J�����̍X�V�B
	m_springCamera.Update();
}

void EntryBoss::BossMovieSkip()
{
	//�X�L�b�v���m�肵���珈�����Ȃ�
	if (m_movieSkipFlag != false)
	{
		return;
	}

	if (g_pad[0]->IsPress(enButtonA))
	{
		//3�b��������X�L�b�v
		if (m_bossMovieSkipTimer > BOSS_MOVIE_SKIP_TIME)
		{
			//�t�F�[�h�C������
			m_fade->StartFadeIn(3.0f);
			m_completeFlag = true;
			m_movieSkipFlag = true;
		}
		else
		{
			//�^�C�}�[�����Z
			m_bossMovieSkipTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	else
	{
		//�^�C�}�[�����Z�b�g
		m_bossMovieSkipTimer = 0.0f;
	}
}

void EntryBoss::DeleteTask()
{
	//���̒�~
	g_soundManager->StopSound(enSoundName_FogRemoval);
	g_soundManager->StopSound(enSoundName_Gogogo);

	//�G�t�F�N�g�̒�~
	if (m_CircleEffect != nullptr)
	{
		m_CircleEffect->Stop();
	}
	//�������G�t�F�N�g�̒�~
	if (m_FogRemovalEffect != nullptr)
	{
		m_FogRemovalEffect->Stop();
	}

	//���C�g�����ɖ߂�
	g_renderingEngine->SetDirLightColor(DIRECTION_RIGHT_COLOR);
	//����
	g_renderingEngine->SetAmbient(g_vec3One * START_AMBIENT_COLOR);
	//�|�C���g���C�g������
	g_renderingEngine->UnUsePointLight();
	//�X�J�C�L���[�u�̖��邳��߂�
	m_skyCube->SetLuminance(START_SKY_CUBE_LMINANCE);
	//�������C�g�̗L����
	g_renderingEngine->SetLimLIghtPower(1.5f);
}

void EntryBoss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�������G�t�F�N�g�̍Đ�
	if (wcscmp(eventName, L"FogRemoval") == 0)
	{
		m_FogRemovalEffect = NewGO<EffectEmitter>(0);
		m_FogRemovalEffect->Init(enEffect_Boss_Summon_FogRemoval);
		m_FogRemovalEffect->Play();
		m_FogRemovalEffect->SetPosition(m_position);
		m_FogRemovalEffect->SetScale({ 15.0f,40.0f,15.0f });
		m_FogRemovalEffect->Update();

		//���������Đ�
		g_soundManager->InitAndPlaySoundSource(enSoundName_FogRemoval, g_soundManager->GetSEVolume());
		//gogogo�������������Ă���
		m_muteFlag = true;
		m_gogogoVolume = g_soundManager->GetDefaultSEVolume();
		//�|�C���g���C�g������
		g_renderingEngine->UnUsePointLight();

		//�������C�g�̗L����
		g_renderingEngine->UseLimLight();

		
	}
}

void EntryBoss::Render(RenderContext& rc)
{
	m_model.Draw(rc);

	if (m_drawBOSSTextFlag == true)
	{
		m_bossTextRender.Draw(rc);
	}
	else
	{
		m_PressAButton.Draw(rc);
	}

	//�\���J�n�t���O�������Ă��Ȃ��Ȃ�
	if (m_GoTextFlag != true)
	{
		return;
	}

	if (m_summonerTextCount >= S)
	{
		m_summonerCharInfo[S].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= U)
	{
		m_summonerCharInfo[U].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= M_1)
	{
		m_summonerCharInfo[M_1].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= M_2)
	{
		m_summonerCharInfo[M_2].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= O)
	{
		m_summonerCharInfo[O].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= N)
	{
		m_summonerCharInfo[N].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= E)
	{
		m_summonerCharInfo[E].m_CharRender.Draw(rc);
	}
	if (m_summonerTextCount >= R)
	{
		m_summonerCharInfo[R].m_CharRender.Draw(rc);
	}
}
