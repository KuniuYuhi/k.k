#include "stdafx.h"
#include "Brave.h"

////////////////////////////////////////
//�R���|�[�l���g
///////////////////////////////////////
#include "PlayerMovement.h"
#include "PlayerController.h"

//���f���̃A�j���[�V�����ǂݍ��݃t�@�C����邱��

///////////////////////////////////////
//
///////////////////////////////////////





Brave::~Brave()
{
}

bool Brave::Start()
{
	m_modelRender.Init("Assets/modelData/character/Player/NewHero/Hero_Smile_Selllook.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS");

	m_modelRender.SetTransform(m_position, m_rotation, m_scale);


	m_charaCon.Init(20.0f, 50.0f, m_position);

	//�X�e�[�^�X��������
	m_status.InitPlayerStatus("Brave");
	
	//�e��R���|�[�l���g�̃Z�b�e�B���O
	SettingDefaultComponent();

	return true;
}

void Brave::Update()
{

	//����̏���

	//�ړ�����
	Movement();


	//��]���Ō�
	Rotation();

	//m_playerMovement->UpdateComponent();


	//���f���̍X�V����
	m_modelRender.SetTransform(m_position, m_rotation, m_scale);
	m_modelRender.Update();
}

void Brave::Damage()
{
}

void Brave::Dead()
{
}

void Brave::SettingDefaultComponent()
{
	//�v���C���[�ړ��R���|�[�l���g���擾
	AddComponent<PlayerMovement>();
	m_playerMovement = GetComponent<PlayerMovement>();

	//�R���g���[���[�R���|�[�l���g���擾
	AddComponent<PlayerController>();
	m_playerContoller = GetComponent<PlayerController>();

}

void Brave::Movement()
{
	//�R���g���[���[�̓��͂��󂯕t���Ȃ��Ȃ�ړ��������Ȃ�
	if (!m_playerContoller->IsControllerInputEnabledFlag()) return;

	//�ړ��ʂ��擾
	m_moveSpeed = m_playerMovement->CalcSimpleMovementVerocity(
		m_status, m_moveSpeed, m_playerContoller->GetLStickInput()
	);

	//���̃W�����v����
	if (m_playerContoller->IsButtonTrigger(enButtonA) && m_charaCon.IsOnGround())
	{
		m_moveSpeed.y = 400.0f;
	}

	//�d�͂̌v�Z
	m_moveSpeed.y -= 980.0f * g_gameTime->GetFrameDeltaTime();

	//�L�����R�����g���č��W���ړ�
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}

	//��]�p�x�N�g���Ɉړ��ʂ�ۑ�
	m_rotateDirection = m_moveSpeed;
}

void Brave::Rotation()
{
	if (fabsf(m_rotateDirection.x) < 0.001f
		&& fabsf(m_rotateDirection.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}

	m_rotation.SetRotationYFromDirectionXZ(m_rotateDirection);

}

void Brave::Attack()
{
}




void Brave::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
