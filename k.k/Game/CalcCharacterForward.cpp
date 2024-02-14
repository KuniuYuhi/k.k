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
    //�O�����̐��K��
    forward.Normalize();
    //�����X�^�[�̍��W�����X�g�ɒǉ�
    PushBackMonsterPositionList();

    //�{�X�̍��W���擾
    Vector3 bossPos = CharactersInfoManager::GetInstance()->
        GetBossInstance()->GetPosition();
   
    //�L�����N�^�[����{�X�Ɍ������x�N�g�����擾
    Vector3 bossToCharacterVector = GetCreateVector(charPosition, bossPos,false);

    //moveSpeed�ɑ������̍����x�N�g����ۑ�����x�N�g��
    Vector3 nearMoveSpeedVector = moveSpeed;

    //�L�����N�^�[����ړ������Ɍ������x�N�g�����쐬
    Vector3 newMoveSpeed = moveSpeed;
    newMoveSpeed.y = 0.0f;
    newMoveSpeed.Add(moveSpeed);
    //�L�����N�^�[����ړ������Ɍ������x�N�g�����擾
    Vector3 moveSpeedToCharacterVector = GetCreateVector(charPosition, newMoveSpeed);    

    //�L�����N�^�[���烂���X�^�[�Ɍ������x�N�g���B�����l�͈ړ�����
    Vector3 monsterToCharcterVector = forward;
    //�ǐՋ������̓G�̐��̃J�E���g
    int count = 0;
    //��ԋ߂��L�����N�^�[���烂���X�^�[�Ɍ������x�N�g���𒲂ׂ�
    for (Vector3 monsterPosition : m_monstersPosition)
    {
        //�L�����N�^�[���烂���X�^�[�Ɍ������x�N�g�����v�Z
        Vector3 diff = monsterPosition - charPosition;
        //�x�N�g���̒������ǐՋ�������������
        if (diff.Length() < chaseDistance)
        {
            //��ԋ߂������X�^�[�Ɍ������x�N�g���ɂȂ�
            monsterToCharcterVector = diff;
            //���K��
            diff.Normalize();

            //�ړ������ƓG�Ɍ������x�N�g���̑������𒲂ׂ�
            float d = Dot(moveSpeedToCharacterVector,diff);
            //��������0.5�ȏ�Ȃ�
            if (d >= 0.5f)
            {
                //�ړ������Ƒ������̍����x�N�g���ɐݒ�
                nearMoveSpeedVector = diff;
            }
            //�ǐՋ������̓G�̐��̃J�E���g���Z
            count++;
        }
    }

    //�O�����x�N�g���̐ݒ�
    Vector3 newForward = charPosition;
    newForward.y = 0.0f;
    newForward.Add(forward * 20.0f);
    //�L�����N�^�[����O�����Ɍ������x�N�g�����擾
    Vector3 forwardToCharacterVector = GetCreateVector(charPosition, newForward);
  
    //�O�����ƓG�Ɍ������O�����̓��ς����߂�
    float t1 = Dot(monsterToCharcterVector, forwardToCharacterVector);
   
    //�O�����ƈړ������̓��ς����߂�
    float t2 = Dot(moveSpeedToCharacterVector, forwardToCharacterVector);

    //�G�Ɍ������x�N�g���ƈړ������̓��ς����߂�
    float t3 = Dot(nearMoveSpeedVector, moveSpeedToCharacterVector);

    //�ړ������̓��͂��Ȃ��Ȃ�G�̕�����D�悷��
    if (fabsf(moveSpeed.x) < 0.1f && fabsf(moveSpeed.z) < 0.1f)
    {
        //���͂��Ȃ����^���܂łɓG��������
        // �O������G�Ɍ������x�N�g���ɕύX
        if (t1 >= 0.0f)
        {
            //�{�X�Ɍ������x�N�g���̒������ǐՋ������Ȃ�
            if (bossToCharacterVector.Length() < chaseDistance)
            {
                //���K��
                bossToCharacterVector.Normalize();
                //�O�������{�X�Ɍ������x�N�g���ɐݒ�
                forward = bossToCharacterVector;
            }
            else
            {
                //�����X�^�[�Ɍ������x�N�g���𐳋K��
                monsterToCharcterVector.Normalize();
                //�O�����������X�^�[�Ɍ������x�N�g���ɐݒ�B
                //�����X�^�[���ǐՋ������ɂ��Ȃ�����������̑O�����ɂȂ�
                forward = monsterToCharcterVector;
            }
        }
        //�I��
        return;
    }

    //��ԋ߂��G�Ɍ������x�N�g���ƈړ������̓��ς����Ȃ莗�Ă��邩��
    //�G���߂��ɂ���J�E���g���P�ȏ�Ȃ�
    //�O������moveSpeed�ɑ������̍����x�N�g���ɏ���������
    if (t3 >= 0.75 && count > 0)
    {
        //���K��
        nearMoveSpeedVector.Normalize();
        //�O������moveSpeed�ɑ������̍����x�N�g���ɐݒ�
        forward = nearMoveSpeedVector;
        //�I��
        return;
    }
    else
    {
        //���Ă��Ȃ�����������̈ړ�������O�����ɐݒ�
        forward = moveSpeed;
        forward.Normalize();
        //�I��
        return;
    }
}

void CalcCharacterForward::PushBackMonsterPositionList()
{
    //�܂������X�^�[�̃��X�g�N���A
    m_monstersPosition.clear();
    //�����X�^�[�̍��W�����X�g�ɒǉ�
    for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
    {
        //���̃����X�^�[������Ă������΂�
        if (monster->GetDeadFlag() == true)
        {
            continue;
        }
        //�����X�^�[�̍��W���擾
        Vector3 monsterPos = monster->GetPosition();
        //���X�g�ɒǉ�
        m_monstersPosition.emplace_back(monsterPos);
    }
}

Vector3 CalcCharacterForward::GetCreateVector(
    Vector3 startPosition, Vector3 endPosition, bool isNormalizeFlag)
{
    //�n�_����I�_�Ɍ������x�N�g�����v�Z
    Vector3 diff = endPosition - startPosition;

    //���K���t���O���Z�b�g����Ă���Ȃ�
    if (isNormalizeFlag == true)
    {
        //���K��
        diff.Normalize();
    }

    return diff;
}
