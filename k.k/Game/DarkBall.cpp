#include "stdafx.h"
#include "DarkBall.h"
#include "InitEffect.h"

#include "Player.h"

namespace {
    const int ADD_CREATE_POS = 100;
    const float SPEED = 600.0f;
    const float RADIUS = 45.0f;
    const float Y_UP = 60.0f;

    const float EXPLOSION_SIZE = 9.0f;

    const float DARK_BALL_SIZE = 12.0f;

    const float DELETE_LENGTH = 1200.0f;

    const float HIT_LENGTH = 80.0f;

    const int ATTACK = 20;

}

DarkBall::DarkBall()
{
    m_speed = SPEED;
}

DarkBall::~DarkBall()
{
    if (m_BallCollision != nullptr)
    {
        DeleteGO(m_BallCollision);
    }

    if (m_darkBallEffect != nullptr)
    {
        m_darkBallEffect->Stop();
    }
}

bool DarkBall::Start()
{
    //�v���C���[�̃C���X�^���X��T��
    m_player = FindGO<Player>("player");

    //�ړ������̐ݒ�
    SetMoveSpeed();
    //����������W�̌���
    m_position += m_moveSpeed * ADD_CREATE_POS;
    //���x�����߂�
    m_moveSpeed *= SPEED;
    //�����J�n���W��ݒ�
    SetStartPosition(m_position);

    //�����蔻��̐ݒ�
    SettingCollision();
    //�G�t�F�N�g�Đ�
    PlayDarkBallEffect();
    
    //�_�[�N�{�[���������Đ�
    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall, g_soundManager->GetSEVolume());

	return true;
}

void DarkBall::Update()
{
    //�q�b�g�����甚������
    if (m_hitFlag == true)
    {
        //��������
        Explosion();
        return;
    }

    //�폜���鋗���ɒB���ĂȂ��Ȃ�
    if (IsForceDelete(m_position, DELETE_LENGTH) != true)
    {
        //�ړ�����
        Move();
        //�v���C���[�Ƀq�b�g������(��������)
        if (IsHit() == true)
        {
            //�q�b�g����
            m_hitFlag = true;
        }
    }
    else
    {
        //�폜����
        //��������
        Explosion();
    }
}

void DarkBall::Move()
{
    //���i����
    MoveStraight();
  
    //�����蔻����I�u�W�F�N�g�Ɠ������W�Ɉړ�
    m_position.y = Y_UP;

    //�G�t�F�N�g�̈ړ��̐ݒ�ƍX�V
    m_darkBallEffect->SetPosition(m_position);
    m_darkBallEffect->Update();
    //�����蔻��̈ړ��̐ݒ�ƍX�V
    m_BallCollision->SetPosition(m_position);
    m_BallCollision->Update();
}

void DarkBall::SettingCollision()
{
    //�����蔻��̍��W�̐ݒ�
    //m_collisionPosition = m_position;

    //�����蔻��쐬
    m_BallCollision = NewGO<CollisionObject>(0, GetCollisionName());
    m_BallCollision->CreateSphere(
        m_position,
        Quaternion::Identity,
        RADIUS
    );
    //�����蔻��𐶐������쐬�҂̐ݒ�
    m_BallCollision->SetCreatorName(GetName());
    //�����ō폜���Ȃ��悤�ɂ���
    m_BallCollision->SetIsEnableAutoDelete(false);

    m_collisionPosition.y = Y_UP;
    m_BallCollision->SetPosition(m_position);
    m_BallCollision->Update();
}

void DarkBall::Explosion()
{
    //�_�[�N�{�[���̃G�t�F�N�g��~
    m_darkBallEffect->Stop();
    //�������Đ�
    g_soundManager->InitAndPlaySoundSource(enSoundName_Boss_DarkBall_Explosion, g_soundManager->GetSEVolume());

    //�����G�t�F�N�g�Đ�
    EffectEmitter* explosionEffect = NewGO<EffectEmitter>(0);
    explosionEffect->Init(InitEffect::enEffect_DarkBall_Explosion);
    explosionEffect->Play();
    explosionEffect->SetScale(g_vec3One * EXPLOSION_SIZE);
    explosionEffect->SetPosition(m_position);
    explosionEffect->Update();
    //���g���폜
    DeleteGO(this);
}

void DarkBall::PlayDarkBallEffect()
{
    //�G�t�F�N�g�̐ݒ�
    m_darkBallEffect = NewGO<EffectEmitter>(0, "DarkBall");
    m_darkBallEffect->Init(InitEffect::enEffect_DarkBall);
    m_darkBallEffect->Play();
    m_darkBallEffect->SetScale(g_vec3One * DARK_BALL_SIZE);
    m_darkBallEffect->SetPosition(m_position);
    m_darkBallEffect->SetRotation(m_rotation);
    m_darkBallEffect->Update();
}

bool DarkBall::IsHit()
{
    //���g�̍��W����v���C���[���������x�N�g�����v�Z
    Vector3 diff = m_player->GetPosition() - m_position;
    //�x�N�g���̒��������l�ȉ��Ȃ�
    if (diff.Length() < HIT_LENGTH)
    {
        //��������
        return true;
    }
    //������Ȃ�����
    return false;
}
