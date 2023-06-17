#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"

namespace {
	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(1.0f, 1.0f, 1.0f);

	const Vector3 SPOT_LIGHT_COLOR = Vector3(40.0f, 10.0f, 10.0f);
}

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{

	Vector3 directionLightDir = Vector3{ 1.0f,-1.0f,-1.0f };
	directionLightDir.Normalize();
	Vector3 directionLightColor = DIRECTION_RIGHT_COLOR;
	//�f�B���N�V�������C�g
	g_renderingEngine->SetDerectionLight(0, directionLightDir, directionLightColor);
	//����
	g_renderingEngine->SetAmbient(Vector3(0.5f, 0.5f, 0.5f));
	//�|�C���g���C�g
	/*g_renderingEngine->SetPointLight(
		Vector3(200.0f, 50.0f, 200.0f),
		Vector3(15.0f, 10.0f, 12.0f),
		Vector3(500.0f, 3.0f, 0.0f)
	);*/
	//�X�|�b�g���C�g
	spPosition.y = 4.0f;
	spPosition.z = 100.0f;
	spDirection = Vector3(1.0f, -1.0f, 1.0f);
	spDirection.Normalize();

	/*g_renderingEngine->SetSpotLight(
		spPosition,
		SPOT_LIGHT_COLOR,
		Vector3(200.0f, 3.0f, 0.0f),
		spDirection,
		Vector3(90.0f, 40.0f, 0.0f)
	);*/

	fontTest.SetText(L"��");
	fontTest.SetPosition(Vector3(50.0f, 0.0f, 0.0f));
	fontTest.SetColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	fontTest.SetOffset(Vector2(20.0f, -20.0f));

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

	spriteTest.Init("Assets/sprite/titleBack.DDS", 1920.0f, 1080.0f);
	spriteTest.SetPosition(m_position);
	//spriteTest.SetGrayScale(true);
	spriteTest.SetWipeSize(wipSize);
	spriteTest.Update();

	//Assets/animData/character/Player/Idle_Normal.tka
	//Assets/animData/character/Player/SprintFWD_Battle.tka
	// 
	// "Assets/animData/character/BadPerson/Idle_Normal1.tka"
	// "Assets/animData/character/BadPerson/Idle_Battle1.tka"
	// 
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/character/BadPerson/Idle_Normal00.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/character/BadPerson/Idle_Battle1.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	//"Assets/modelData/character/Player/Hero.tkm",
	//"Assets/modelData/character/BadPerson/BadPerson_Boy.tkm"
	model.Init(
		"Assets/modelData/character/BadPerson/BadPerson_Boy.tkm",
		m_animationClipArray, 
		enAnimClip_Num, 
		enModelUpAxisZ
		);
	//model.SetRotation(m_rotation);
	model.Update();
	
	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	//���x��
	levelbg.Init(
		"Assets/level3D/stadium05Level.tkl",
		[&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"stadium05_ground")==true) {
				backGround.InitShadow("Assets/modelData/BackGround/grass01.tkm");
				backGround.SetPosition(objData.position);
				backGround.SetRotation(objData.rotation);
				BGPhysicsStaticObject.CreateFromModel(
					backGround.GetModel(),
					backGround.GetModel().GetWorldMatrix()
				);
				return true;
			}

			return false;
		});

	//backGround.SetRotation(Quaternion(0.0f, 180.0f, 0.0f, 1.0f));
	backGround.Update();
	
	GameCamera* gameCamera = NewGO<GameCamera>(0, "gameCamera");

	//�����蔻��̉���
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}

void Game::Update()
{
	PlayAnim();
	//SpriteTransform();
	Move();

	Spotmove();

	if (g_pad[0]->IsPress(enButtonStart))
	{
		spriteTest.SetWipeSize(wipSize);
		wipSize += 5.0f;
		//if (g_renderingEngine->HemiLightIsUse() == false)
		//{
		//	//g_renderingEngine->UnUseHemiLight();

		//	g_renderingEngine->UseHemiLight();
		//}
		//else
		//g_renderingEngine->UnUseHemiLight();
		
		//g_renderingEngine->UseHemiLight();
	}
}

void Game::Move()
{
	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * -300.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * -300.0f;
	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());


	//// ���X�e�B�b�N(�L�[�{�[�h�FWASD)�ŕ��s�ړ��B
	/*m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();*/

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


	model.SetPosition(m_position);
	model.SetRotation(m_rotation);
	model.SetScale(m_scale);
	model.Update();
}

void Game::SpriteTransform()
{
	//// ���X�e�B�b�N(�L�[�{�[�h�FWASD)�ŕ��s�ړ��B
	m_position.x += g_pad[0]->GetLStickXF();
	//m_position.y += g_pad[0]->GetLStickYF();

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
		model.PlayAnimation(enAnimClip_Idle, 0.6f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		model.PlayAnimation(enAnimClip_Run, 0.2f);
	}

	
}

void Game::Spotmove()
{
	/*spDirection.x -= g_pad[0]->GetLStickXF()*0.5f;
	spDirection.Normalize();
	g_renderingEngine->SetDirLightDirection(spDirection);*/


	//���̃A�i���O�X�e�B�b�N�œ�����
	spPosition.x -= g_pad[0]->GetLStickXF();
	if (g_pad[0]->IsPress(enButtonB))
	{
		//B�{�^�����ꏏ�ɉ�����Ă�����Y�������ɓ�����
		spPosition.y += g_pad[0]->GetLStickYF();
	}
	else
	{
		//Z�����ɓ�����
		spPosition.z -= g_pad[0]->GetLStickYF();
	}

	// step-4 �R���g���[���[�E�X�e�B�b�N�ŃX�|�b�g���C�g����]������
	   //Y�����̃N�H�[�^�j�I�����v�Z����
	Quaternion qRotY;
	qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
	//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕�������
	qRotY.Apply(spDirection);

	//X�����̃N�H�[�^�j�I�����v�Z����
	Vector3 rotAxis;
	rotAxis.Cross(g_vec3AxisY, spDirection);
	Quaternion qRotX;
	qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);
	//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕�������
	qRotX.Apply(spDirection);

	spDirection.Normalize();
	/*g_renderingEngine->SetSpotLightPosition(spPosition);
	g_renderingEngine->SetSpotLightDirection(spDirection);*/
	
}

void Game::Render(RenderContext& rc)
{
	model.Draw(rc);
	backGround.Draw(rc);
	//spriteTest.Draw(rc);
	//fontTest.Draw(rc);
}
