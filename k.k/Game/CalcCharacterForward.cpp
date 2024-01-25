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
    
    forward.Normalize();

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

    //�����X�^�[�Ɍ������O�����B�����l�͈ړ�����
    Vector3 monsterForward = forward;
    //moveSpeed�ɑ������̍����x�N�g��
    Vector3 NearmoveSpeedMonsterForward = moveSpeed;

    //�ړ������Ɍ������x�N�g��
    Vector3 moveSpeedVector = charPosition;
    moveSpeedVector.y = 0.0f;
    moveSpeedVector.Add(moveSpeed);
    //���g�̍��W����ړ������Ɍ������x�N�g��
    Vector3 moveSpeedDiff = moveSpeedVector - charPosition;
    //���K��
    moveSpeedDiff.Normalize();

    //�ǐՋ������J�E���g
    int count = 0;

    for (Vector3 monsterPosition : m_monstersPosition)
    {
        //�v���C���[���烂���X�^�[�Ɍ������x�N�g�����v�Z
        Vector3 diff = monsterPosition - charPosition;
        //�x�N�g���̒������ǐՋ�������������
        if (diff.Length() < chaseDistance)
        {
            //��ԋ߂��G�Ɍ������x�N�g���ɂȂ�
            monsterForward = diff;

            //���K��
            diff.Normalize();
            //�ړ������ƓG�Ɍ������x�N�g���̑������𒲂ׂ�
            float d = Dot(moveSpeedDiff,diff);
            //��������0.5�ȏ�Ȃ�
            if (d >= 0.5f)
            {
                //�x�N�g����ݒ�
                NearmoveSpeedMonsterForward = diff;
            }
            //�J�E���g���Z
            count++;
        }
    }

    //�G�Ɍ������x�N�g���𐳋K��
    monsterForward.Normalize();

    //�O�����x�N�g���̐ݒ�
    Vector3 forwardVector = charPosition;
    forwardVector.y = 0.0f;
    forwardVector.Add(forward * 20.0f);
    //���g�̍��W����O�����Ɍ������x�N�g��
    Vector3 forwardDiff = forwardVector - charPosition;
    //���K��
    forwardDiff.Normalize();

    
   

    //�O�����ƓG�Ɍ������O�����̓��ς����߂�
    float t1 = Dot(monsterForward, forwardDiff);
   
    //�O�����ƈړ������̓��ς����߂�
    float t2 = Dot(moveSpeedDiff, forwardDiff);

    //�G�Ɍ������x�N�g���ƈړ������̓��ς����߂�
    float t3 = Dot(NearmoveSpeedMonsterForward, moveSpeedDiff);


    //�G�̕����𒲂ׂ�
    //�O�����𒲂ׂ�
    // 
    //�ړ������𒲂ׂ�  
    //�ړ������ƑO�����̓��ς̑���0.0�ȏ�Ȃ�ړ�������O�����ɂ���
    // 
    // �ړ������ƓG�̕��������Ȃ莗�Ă�����A�O������G�̕����ɏ���������
    // 
    //�G�̕����ƑO�����̓��ς̑�����0.0�ȏ�Ȃ�O������ύX�I�I

    //�ړ������̓��͂��Ȃ��Ȃ�G�̕�����D�悷��
    if (fabsf(moveSpeed.x) < 0.1f && fabsf(moveSpeed.z) < 0.1f)
    {
        //���͂��Ȃ����^���܂łɓG��������
        // �O������G�Ɍ������x�N�g���ɕύX
        if (t1 >= 0.0f)
        {
            //�����X�^�[�Ɍ������O�����𐳋K��
            monsterForward.Normalize();
            //�O��������
            forward = monsterForward;
        }
        return;
    }
    //�ړ������̓��ςƓG�Ɍ��������ς����Ȃ莗�Ă��邩��
    //�G���߂��ɂ���J�E���g
    //�O������G�Ɍ������x�N�g���ɏ���������
    if (t3 >= 0.5 && count > 0)
    {
        //�O�����𐳋K��
        NearmoveSpeedMonsterForward.Normalize();
        forward = NearmoveSpeedMonsterForward;
        return;
    }
    else
    {
        //���Ă��Ȃ�����������̈ړ�������O�����ɐݒ�
        forward = moveSpeed;
        forward.Normalize();
        return;
    }
    //�ړ��̓��͂���������

    //�P�ʃx�N�g�����m�̓��ς�-1����1�͈̔́B1�ɂȂ�قǎ��Ă���
    //���ς�0.0f�ȏ�Ȃ�O������ύX����
    
    //�O�����ƈړ������̃x�N�g���̑�������0.0�����傫���Ȃ�
    if (t2 >= 0.0f)
    {
        
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

       
        //�o���ĂȂ�
        forward = rightangleX;
        forward.Normalize();


        //���Ă��Ȃ�����������̈ړ�������O�����ɐݒ�
        forward = moveSpeed;
        forward.Normalize();

        return;
    }

  
   

  

}
