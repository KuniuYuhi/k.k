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
    //ウィザード用の設定
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
        //攻撃力の設定
        m_atk = m_wizardAttack;
        //タイマーの設定
        m_limitTimer = m_forWizardLimitTimer;
    }
    //リッチ用の設定
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
        //攻撃力の設定
        m_atk = m_lichAttack;
        //タイマーの設定
        m_limitTimer = m_forLichLimitTimer;
    }
    else
    {
        //打ったキャラクターが誰か分からなかったらクラッシュさせる
        std::abort();
    }

	return true;
}

void FireBall::Update()
{
    //敵にぶつかったら自身を消す
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
    //生成する座標の決定
    m_position += m_moveSpeed * distance;
    //速度を決める
    m_moveSpeed *= speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        scale
    );

    m_model.Update();

    //当たり判定の座標の設定
    m_collisionPosition = m_position;

    //当たり判定作成
    m_BallCollision = NewGO<CollisionObject>(0, collisionname);
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        collisionradius
    );
    //すり抜けるようにする
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
    //生成する座標の決定
    m_position += m_moveSpeed * distance;
    //速度を決める
    m_moveSpeed *= speed;

    m_model.SetTransform(
        m_position,
        m_rotation,
        scale
    );

    m_model.Update();

    //当たり判定の座標の設定
    m_collisionPosition = m_position;

    //当たり判定作成
    m_BallCollision = NewGO<CollisionObject>(0, collisionname);
    m_BallCollision->CreateSphere(
        m_collisionPosition,
        Quaternion::Identity,
        collisionradius
    );
    //すり抜けるようにする
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y += y_up;
    m_BallCollision->SetPosition(m_collisionPosition);
    m_BallCollision->Update();
}
