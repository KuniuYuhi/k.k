#include "stdafx.h"
#include "EntryBoss.h"
#include "Game.h"
#include "InitEffect.h"

namespace {
	//�X�J�C�L���[�u�̏����̖��邳
	const float START_SKY_CUBE_LMINANCE = 0.9f;
	const float END_SKY_CUBE_LMINANCE = 0.2f;

	//���f���̃T�C�Y
	const float SCALE_UP = 4.0f;
	//���f����Y���W�Ɖ�]
	const float DEFAULT_MODEL_POS_Y = 380.0f;
	const float DEFAULT__ROT_Y = 180.0f;
	//Y���W�̉��Z��
	const float ADD_MODEL_Y_UP = 45.0f;

	//L�AI�AC�AH�A�̕\���^�C�~���O
	const float LICH_SPRITE_DRAW_TIMING = -20.0f;

	//L�AI�AC�AH�A�̏������W�ƍŏI�I�ȍ��W
	const Vector3 L_START_POS = { -707.0f,430.0f,0.0f };
	const Vector3 L_END_POS = { -407.0f,-268.0f,0.0f };
	const Vector3 I_START_POS = { -488.0f,430.0f,0.0f };
	const Vector3 I_END_POS = { -188.0f,-268.0f,0.0f };
	const Vector3 C_START_POS = { 361.0f,430.0f,0.0f };
	const Vector3 C_END_POS = { 61.0f,-268.0f,0.0f };
	const Vector3 H_START_POS = { 676.0f,430.0f,0.0f };
	const Vector3 H_END_POS = { 376.0f,-268.0f,0.0f };
	//LICH�摜�̃T�C�Y
	const Vector3 START_SCALE = { 4.0f,4.0f,4.0f };
	const Vector3 END_SCALE = g_vec3One;

	//BOSS�e�L�X�g�摜�̍��W
	const Vector3 BOSS_TEXT_POS = { 0.0f,-98.0f,0.0f };

	//A�{�^���������ƃX�L�b�v�̉摜�̍��W�ƃT�C�Y
	const Vector3 PRESS_A_POS = { 723.0f,-501.0f ,0.0f };
	const Vector3 PRESS_A_SCALE = { 0.6f,0.6f ,0.6f };

	//���@�w��Y���W�ƃT�C�Y
	const float ADD_CIRCLE_POS_Y = 5.0f;
	const float CIRICLE_EFFECT_SIZE = 15.0f;

	//�����̏����J���[�ƍŏI�I�ȃJ���[
	const float START_AMBIENT_COLOR = 0.8f;
	const float END_AMBIENT_COLOR = 0.1f;

	//�|�C���g���C�g
	const Vector3 ADD_POINT_LIGHT_POS = { 0.0f,50.0f,-140.0f };
	const Vector3 POINT_LIGHT_COLOR = { 14.0f, 7.0f, 12.0f };
	const Vector3 POINT_LIGHT_RANGE = { 1500.0f, 4.0f, 0.0f };

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.8f, 0.8f, 0.8f);

}

EntryBoss::EntryBoss()
{
}

EntryBoss::~EntryBoss()
{
	//�G�t�F�N�g�̒�~
	if (m_CircleEffect != nullptr)
	{
		m_CircleEffect->Stop();
	}
	
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
}

bool EntryBoss::Start()
{
	//m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/deleteAtk.tka");
	//m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	//m_animationClip[enAnimClip_Victory].Load("Assets/animData/character/Lich/deleteAtk.tka");
	//m_animationClip[enAnimClip_Victory].SetLoopFlag(true);
	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_FogRemoval].Load("Assets/animData/character/Lich/FogRemoval.tka");
	m_animationClip[enAnimClip_FogRemoval].SetLoopFlag(false);

	m_model.Init(
		"Assets/modelData/character/Lich/Lich_real.tkm",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	Vector3 pos = m_position;
	pos.y += ADD_CIRCLE_POS_Y;
	m_CircleEffect = NewGO<EffectEmitter>(0);
	m_CircleEffect->Init(InitEffect::enEffect_Boss_Summon_Circle);
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

	//�X�J�C�L���[�u�����邳���擾
	m_skyLuminance = m_skyCube->GetLuminance();
	
	/*g_renderingEngine->SetSpotLight(
		pointLightPos,
		Vector3(100.0f,0.0f,100.0f),
		Vector3(500.0f, 10.0f, 0.0f),
		Vector3(1.0f,-1.0f,0.0f),
		Vector3(90.0f, 40.0f, 0.0f)
	);*/

	
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//�摜�̓ǂݍ���
	InitSprite();
	//���`��ԂŎg���n�_�ƏI�_�̏�����
	InitLerpPosition();


	//����
	g_renderingEngine->SetAmbient(g_vec3One);
	//�����̊����̃J���[��ݒ�
	m_ambientColor = START_AMBIENT_COLOR;

	return true;
}

void EntryBoss::Update()
{
	//��ʂ����X�ɈÂ�����
	slowlyDarkScreen();

	//�o�ꃀ�[�r�[���������������
	if (m_completeFlag == true)
	{
		//���΂炭�Î~���Ă���I���
		CompleteTime();
	}
	else
	{
		//�{�X�̍��W���グ��
		positionUp();
		//0�ɂȂ邿��ƑO����摜������
		if (m_GoTextFlag == true)
		{
			SpriteMove();
		}
	}

	//�������G�t�F�N�g��nullpty�łȂ��Ȃ�
	if (m_FogRemovalEffect != nullptr)
	{
		SlowlyBrightScreen();

		//�������G�t�F�N�g�����������
		if (m_FogRemovalEffect->IsPlay() == false)
		{
			//��邱�ƏI���
			m_completeFlag = true;
		}
	}

	MamageState();
	Animation();
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
	m_lichCharInfo[L].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/L.DDS", 243, 290);
	SettingSpriteRender(m_lichCharInfo[L].m_CharRender, L_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[I].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/I.DDS", 167, 290);
	SettingSpriteRender(m_lichCharInfo[I].m_CharRender, I_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[C].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/C.DDS", 280, 290);
	SettingSpriteRender(m_lichCharInfo[C].m_CharRender, C_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[H].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/H.DDS", 307, 290);
	SettingSpriteRender(m_lichCharInfo[H].m_CharRender, H_START_POS, START_SCALE, g_quatIdentity);
	
	m_bossTextRender.Init("Assets/sprite/InGame/BossAppearance/Boss.DDS", 284, 84);
	SettingSpriteRender(m_bossTextRender, BOSS_TEXT_POS, g_vec3One, g_quatIdentity);

	m_PressAButton.Init("Assets/sprite/InGame/BossAppearance/SkipText.DDS", 720, 76);
	SettingSpriteRender(m_PressAButton, PRESS_A_POS, PRESS_A_SCALE, g_quatIdentity);
}

void EntryBoss::InitLerpPosition()
{
	//�uL�v�̎n�_�ƏI�_
	m_lichCharInfo[L].m_startPosition = L_START_POS;
	m_lichCharInfo[L].m_endPosition = L_END_POS;
	//�uI�v�̎n�_�ƏI�_
	m_lichCharInfo[I].m_startPosition = I_START_POS;
	m_lichCharInfo[I].m_endPosition = I_END_POS;
	//�uC�v�̎n�_�ƏI�_
	m_lichCharInfo[C].m_startPosition = C_START_POS;
	m_lichCharInfo[C].m_endPosition = C_END_POS;
	//�uI�v�̎n�_�ƏI�_
	m_lichCharInfo[H].m_startPosition = H_START_POS;
	m_lichCharInfo[H].m_endPosition = H_END_POS;
}

void EntryBoss::SpriteMove()
{
	if (m_lichTextCount == END)
	{
		m_drawBOSSTextFlag = true;
		return;
	}
	//��ԗ��̌v�Z
	m_time += g_gameTime->GetFrameDeltaTime() * 3.8f;

	//�����̍��W�̐��`���
	Vector3 LerpPos;
	LerpPos.Lerp(m_time, m_lichCharInfo[m_lichTextCount].m_startPosition, m_lichCharInfo[m_lichTextCount].m_endPosition);
	//�����̃T�C�Y�̐��`���
	Vector3 size;
	size.Lerp(m_time, START_SCALE, END_SCALE);
	//��]
	Quaternion rot = g_quatIdentity;

	//�X�V
	m_lichCharInfo[m_lichTextCount].m_CharRender.SetTransForm(LerpPos, size, rot);
	m_lichCharInfo[m_lichTextCount].m_CharRender.Update();


	//��b�������玟�̕����ɐ؂�ւ���
	if (m_time >= 1.0f)
	{
		m_lichTextCount++;
		m_time = 0.0f;
	}
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

	//����Â�����
	m_skyLuminance = Math::Lerp(m_lightTimer, m_skyLuminance, END_SKY_CUBE_LMINANCE);
	m_skyCube->SetLuminance(m_skyLuminance);
	m_skyCube->Update();

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

	m_lightTimer += g_gameTime->GetFrameDeltaTime() * 0.03f;

	//��ʂ𖾂邭����
	m_ambientColor = Math::Lerp(m_lightTimer, m_ambientColor, START_AMBIENT_COLOR);
	m_finalAmbientColor = g_vec3One * m_ambientColor;
	g_renderingEngine->SetAmbient(m_finalAmbientColor);

	//��𖾂邭����
	m_skyLuminance = Math::Lerp(m_lightTimer, m_skyLuminance, START_SKY_CUBE_LMINANCE);
	m_skyCube->SetLuminance(m_skyLuminance);
	m_skyCube->Update();

	//�������ő�܂ŏ������Ȃ�����
	if (m_ambientColor >= START_AMBIENT_COLOR)
	{
		m_SlowlyBrightScreenEndFlag = true;
	}
}

void EntryBoss::CompleteTime()
{
	//��������������炵�΂炭��~���Ă���
	if (m_endTime < m_endTimer)
	{
		//�o�ꃀ�[�r�[���I��������Ƃ�`����
		m_game->SetBossMovieFlag(true);
	}
	else
	{
		m_endTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void EntryBoss::MamageState()
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

void EntryBoss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�������G�t�F�N�g�̍Đ�
	if (wcscmp(eventName, L"FogRemoval") == 0)
	{
		m_FogRemovalEffect = NewGO<EffectEmitter>(0);
		m_FogRemovalEffect->Init(InitEffect::enEffect_Boss_Summon_FogRemoval);
		m_FogRemovalEffect->Play();
		m_FogRemovalEffect->SetPosition(m_position);
		m_FogRemovalEffect->SetScale({ 15.0f,40.0f,15.0f });
		m_FogRemovalEffect->Update();

		//�|�C���g���C�g������
		g_renderingEngine->UnUsePointLight();
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

	if (m_lichTextCount >= L)
	{
		m_lichCharInfo[L].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= I)
	{
		m_lichCharInfo[I].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= C)
	{
		m_lichCharInfo[C].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= H)
	{
		m_lichCharInfo[H].m_CharRender.Draw(rc);
	}
}
