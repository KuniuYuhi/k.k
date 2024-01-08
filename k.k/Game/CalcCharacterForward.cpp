#include "stdafx.h"
#include "CalcCharacterForward.h"

#include "CharactersInfoManager.h"
#include "Boss.h"

void CalcCharacterForward::CalcForwardOfNearMonster(
    Vector3 charPosition, 
    Vector3& forward, 
    Vector3& moveSpeed, 
    float chaseDistance
)
{
    //�܂������X�^�[�̃��X�g�N���A
    m_monstersPosition.clear();

    //�����X�^�[�̍��W�����X�g�ɒǉ�
    for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
    {
        //�����X�^�[�̍��W���擾
        Vector3 monsterPos = monster->GetPosition();
        //���X�g�ɒǉ�
        m_monstersPosition.emplace_back(monsterPos);
    }

    //�{�X�̍��W�����X�g�ɒǉ�
    Vector3 bossPos = CharactersInfoManager::GetInstance()->
        GetBossInstance()->GetPosition();
    //���X�g�ɒǉ�
    m_monstersPosition.emplace_back(bossPos);

    //���̑O����
    Vector3 temporaryForward = forward;
    //�ǐՋ������J�E���g
    int count = 0;

    for (Vector3 monsterPosition : m_monstersPosition)
    {
        //�v���C���[���烂���X�^�[�Ɍ������x�N�g�����v�Z
        Vector3 diff = monsterPosition - charPosition;
        //�x�N�g���̒������ǐՋ�������������
        if (diff.Length() < chaseDistance)
        {
            temporaryForward = diff;
            count++;
        }
    }

    //���ɂ��Ȃ��Ȃ�forawrd������Ȃ�

    //�O�����𐳋K��
    temporaryForward.Normalize();

    Vector3 forwardVector = charPosition;
    forwardVector.y = 0.0f;
    forwardVector.Add(forward * 20.0f);
    //���g�̍��W����O�����Ɍ������x�N�g��
    Vector3 forwardDiff = forwardVector - charPosition;
    //���K��
    forwardDiff.Normalize();

    

    Vector3 moveSpeedVector = charPosition;
    moveSpeedVector.y = 0.0f;
    moveSpeedVector.Add(moveSpeed);
    //���g�̍��W����ړ������Ɍ������x�N�g��
    Vector3 moveSpeedDiff = moveSpeedVector - charPosition;
    //���K��
    moveSpeedDiff.Normalize();

    //�O�����ƓG�Ɍ������O�����̓��ς����߂�
    float t1 = Dot(temporaryForward, forwardDiff);
   
    //�O�����ƈړ������̓��ς����߂�
    float t2 = Dot(moveSpeedDiff, forwardDiff);



    //�G�̕����𒲂ׂ�
    //�O�����𒲂ׂ�
    // 
    //�ړ������𒲂ׂ�  
    //�ړ������ƑO�����̓��ς̑���0.0�ȏ�Ȃ�ړ�������O�����ɂ���
    // 
    // �ړ������ƓG�̕��������Ȃ莗�Ă�����A�O������G�̕����ɏ���������
    // 
    //�G�̕����ƑO�����̓��ς̑�����0.0�ȏ�Ȃ�O������ύX�I�I


    // //X�����Œ��p�̃x�N�g��
    //Vector3 rightangleX = forwardDiff;
    ////rightangleX *= -1.0f;
    //float X = rightangleX.x;
    //rightangleX.x = rightangleX.z;
    //rightangleX.z = X;
    //rightangleX.y = 0.0f;

    //rightangleX.x *= moveSpeedDiff.x;
   
    //rightangleX.z *= moveSpeedDiff.z;

    //forward = rightangleX;
    //forward.Normalize();

    //return;

    //�ړ������̓��͂��Ȃ��Ȃ�G�̕�����D�悷��
    if (fabsf(moveSpeed.x) < 0.1f && fabsf(moveSpeed.z) < 0.1f)
    {
        //�G�Ɍ������x�N�g�����^���܂łȂ�
        if (t1 >= 0.0f)
        {
            //�O�����𐳋K��
            temporaryForward.Normalize();
            forward = temporaryForward;
        }

        return;
    }

    //�ړ��̓��͂���������

    //�P�ʃx�N�g�����m�̓��ς�-1����1�͈̔́B1�ɂȂ�قǎ��Ă���
    //���ς�0.0f�ȏ�Ȃ�O������ύX����
    if (t2 >= 0.0f)
    {
        //�G����̂������Ă��Ȃ�������
        if (count == 0)
        {
            forward = moveSpeed;
            forward.Normalize();
            return;
        }

        //�ړ������̓��ςƓG�Ɍ��������ς����Ȃ莗�Ă�����A
        //�O������G�Ɍ������x�N�g���ɏ���������
        if (abs(t1 - t2) < 0.5)
        {
            //�O�����𐳋K��
            temporaryForward.Normalize();
            forward = temporaryForward;
        }
        else
        {
            //���Ă��Ȃ�����������̈ړ�������O�����ɐݒ�
            forward = moveSpeed;
            forward.Normalize();
            return;
        }
    }
    else
    {
        //�x�N�g�����G�Ɍ�����͈͂𒴂��Ă���̂�
        //�^���̃x�N�g���ɂ���

        

        //���̐��̎��͐���A���̎��͋t���]

         //�O����������͕����Ɍ�������90�x�̃x�N�g�����쐬
        Vector3 rightangleX = forwardDiff;
        //rightangleX *= -1.0f;

        float X = rightangleX.x;
        rightangleX.x = rightangleX.z;
        rightangleX.z = X;
        rightangleX.y = 0.0f;

        Quaternion rot90 = g_quatIdentity;

        rot90.SetRotationYFromDirectionXZ(forwardDiff);
        rot90.AddRotationDegY(90.0f);
        rot90.Apply(rightangleX);

        //�O�����Ɠ��͕����̓��ςłȂ��p�����߂�3.14��max
        float kakudo = Dot(forwardDiff, moveSpeedDiff);
        kakudo = acos(kakudo);
        //�}�C�i�X���v���X���Ŕ��f

        //if (kakudo > 2.0f)
        //{
        //    //���]������
        //    rightangleX *= -1.0f;
        //}


        //if (moveSpeedDiff.x > moveSpeedDiff.z)
        //{
        //    rightangleX.z *= -1.0f;
        //    //rightangleX.z *= moveSpeedDiff.z;
        //}
        //else
        //{
        //    rightangleX.z *= -1.0f;
        //    //rightangleX.x *= moveSpeedDiff.x;
        //}

       

        forward = rightangleX;
        forward.Normalize();

        return;
    }

  
   

  

}
