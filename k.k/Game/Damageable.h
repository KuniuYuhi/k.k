#pragma once

////���̃R���W������������A�N�^�[������
// �N���G�C�^�[���擾
//Damageable = FindGO<Damageable>(knightcollision->GetCreatorName());
//Damageable�������Ă�����_���[�W����


/// <summary>
/// ��_���[�W����L�����N�^�[�ɒǉ�����B�C���^�[�t�F�[�X�̎���
/// </summary>
class Damageable
{
public:

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	virtual void Damage() = 0;

	/// <summary>
	/// ���S������
	/// </summary>
	virtual void Dead() = 0;
};

