#include "stdafx.h"
#include "FireBall.h"
//#include "Wizard.h"

FireBall::FireBall()
{
}

FireBall::~FireBall()
{
    if (m_BallCollision != nullptr)
    {
        DeleteGO(m_BallCollision);
    }
}

bool FireBall::Start()
{
    //�E�B�U�[�h�p�̐ݒ�
    if (m_wizard != nullptr)
    {
       m_model.Init("Assets/modelData/character/Wizard/Effect/FireBall.tkm",nullptr ,0 ,enModelUpAxisZ ,false,false,false);
        SetForWizard(
            m_fireBall,
            m_speed,
            m_distance,
            20,
            15,
            m_forWizardBallScale
        );
        //�U���͂̐ݒ�
        m_atk = m_wizardAttack;
        //�^�C�}�[�̐ݒ�
        m_limitTimer = m_forWizardLimitTimer;
    }
    //���b�`�p�̐ݒ�
    else if (m_lich != nullptr)
    {
       m_model.Init("Assets/modelData/character/Lich/Effect/DarkBall.tkm", nullptr, 0, enModelUpAxisZ, false,false,false);
        SetForLich(
            m_darkBall,
            350,
            100,
            60,
            45,
            m_forLichBallScale
        );
        //�U���͂̐ݒ�
        m_atk = m_lichAttack;
        //�^�C�}�[�̐ݒ�
        m_limitTimer = m_forLichLimitTimer;
    }
    else
    {
        //�ł����L�����N�^�[���N��������Ȃ�������N���b�V��������
        std::abort();
    }

	return true;
}

void FireBall::Update()
{
    //�G�ɂԂ������玩�g������
    if (m_hitEnemeyFlag == true)
    {
        DeleteGO(this);
    }

    Timer();

    Move();

}

void FireBall::Timer()
{
    if (m_limitTimer < m_moveTime)
    {
        DeleteGO(this);
    }
    else
    {
        m_moveTime += g_gameTime->GetFrameDeltaTime();
    }
}

void FireBall::Move()
{
    m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
    m_collisionPosition = m_position;
    m_collisionPosition.y += 20.0f;

    m_model.SetPosition(m_position);
    m_model.Update();
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::Render(RenderContext& rc)
{
    m_model.Draw(rc);
}

void FireBall::SetForWizard(const char* collisionname, float speed, 
    float distance, float y_up, float collisionradius, Vector3 scale)
{
    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);
    //����������W�̌���
    m_position += m_moveSpeed * distance;
    //���x�����߂�
    m_moveSpeed *= speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        scale
    );

    m_model.Update();

    //�����蔻��̍��W�̐ݒ�
    m_collisionPosition = m_position;

    //�����蔻��쐬
    m_BallCollision = NewGO<CollisionObject>(0, collisionname);
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        collisionradius
    );
    //���蔲����悤�ɂ���
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += y_up;
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}

void FireBall::SetForLich(const char* collisionname, float speed, 
    float distance, float y_up, float collisionradius, Vector3 scale)
{
    m_moveSpeed = Vector3::AxisZ;
    m_rotation.Apply(m_moveSpeed);
    //����������W�̌���
    m_position += m_moveSpeed * distance;
    //���x�����߂�
    m_moveSpeed *= speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        scale
    );

    m_model.Update();

    //�����蔻��̍��W�̐ݒ�
    m_collisionPosition = m_position;

    //�����蔻��쐬
    m_BallCollision = NewGO<CollisionObject>(0, collisionname);
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        collisionradius
    );
    //���蔲����悤�ɂ���
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += y_up;
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}
