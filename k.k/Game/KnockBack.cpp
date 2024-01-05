#include "stdafx.h"
#include "KnockBack.h"

const Vector3 SetKnockBackDirection(
	Vector3 startPosition, Vector3 endPosition,float knockBackPower)
{
	//�n�_����I�_�Ɍ������x�N�g�����v�Z
	Vector3 finalPos;
	finalPos = startPosition - endPosition;



	//���K��
	finalPos.y = 0.0f;
	finalPos.Normalize();
	//�m�b�N�o�b�N�p���[���|����
	//�p���[���傫���قǐ������
	finalPos *= knockBackPower;

	return finalPos;
}

bool IsKnockingBack(
	Vector3& moveSpeed,
	float& timer, 
	const float timerLimmit
)
{
	if (timer >= timerLimmit)
	{
		//�^�C�}�[�����Z�b�g
		timer = 0.0f;
		//�L�����̌����Ă�������ƃx�N�g�����t�Ȃ̂�
		//-1���|���Ė߂�
		moveSpeed *= -1.0f;
		return false;
	}

	//�^�C�}�[�����Z
	timer += g_gameTime->GetFrameDeltaTime()*2.0f;

	return true;
}
