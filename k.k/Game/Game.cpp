#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	fontTest.SetText(L"��");
	fontTest.SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	fontTest.SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	fontTest.SetOffset(Vector2(20.0f, -20.0f));


	spriteTest.Init("Assets/sprite/pointer_black.DDS",220.0f,220.0f);
	spriteTest.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	//spriteTest.SetGrayScale(true);
	spriteTest.Update();


	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	model.Init(
		"Assets/modelData/unityChan.tkm", 
		m_animationClipArray, 
		enAnimClip_Num, 
		enModelUpAxisY);

	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	backGround.Init("Assets/modelData/BackGround/bg.tkm");
	BGPhysicsStaticObject.CreateFromModel(
		backGround.GetModel(),
		backGround.GetModel().GetWorldMatrix()
	);

	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	PlayAnim();
	SpriteTransform();
	Move();

	if (g_pad[0]->IsPress(enButtonStart))
	{
		m_fontScale += g_gameTime->GetFrameDeltaTime()*2.0f;
		fontTest.SetScale(m_fontScale);
	}

}

void Game::Move()
{
	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * 120.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * 120.0f;
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());


	//// ���X�e�B�b�N(�L�[�{�[�h�FWASD)�ŕ��s�ړ��B
	/*m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();*/

	// �E�X�e�B�b�N(�L�[�{�[�h�F�㉺���E)�ŉ�]�B
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	// �㉺���E�L�[(�L�[�{�[�h�F2, 4, 6, 8)�Ŋg��
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.SetScale(m_scale);
	model.Update();
}

void Game::SpriteTransform()
{
	//// ���X�e�B�b�N(�L�[�{�[�h�FWASD)�ŕ��s�ړ��B
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();

	// �E�X�e�B�b�N(�L�[�{�[�h�F�㉺���E)�ŉ�]�B
	/*m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);*/

	// �㉺���E�L�[(�L�[�{�[�h�F2, 4, 6, 8)�Ŋg��
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	spriteTest.SetPosition(m_position);
	//spriteTest.SetRotation(m_rotation);
	spriteTest.SetScale(m_scale);
	spriteTest.Update();
}

void Game::PlayAnim()
{
	// �A�j���[�V�����̐؂�ւ��B
	if (g_pad[0]->IsPress(enButtonA)) {
		model.PlayAnimation(enAnimClip_Idle, 0.2f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		model.PlayAnimation(enAnimClip_Run, 0.2f);
	}

	
}

void Game::Render(RenderContext& rc)
{
	model.Draw(rc);
	backGround.Draw(rc);
	spriteTest.Draw(rc);
	fontTest.Draw(rc);
}
