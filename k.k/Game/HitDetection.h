#pragma once

//����ɍU�����q�b�g�������邩�̔�����s��
//���i�q�b�g����U���Ŏg��
struct HitDetection
{
private:
	float timer = 0.0f;			//�^�C�}�[
	bool hittableFlag = false;

public:
	/// <summary>
	/// ���i�q�b�g�\��
	/// </summary>
	/// <param name="hittableTime">�q�b�g�\�ɂȂ鐧������</param>
	/// <param name="hittableFlag">�q�b�g�\�����ʂ��ق��񂷂�t���O</param>
	/// <returns>�q�b�g�\��true�A�s�\��false</returns>
	bool IsHittable(const float hittableTime);

	/// <summary>
	/// ���i�q�b�g�\���t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetHittableFlag(bool flag)
	{
		hittableFlag = flag;
	}
	/// <summary>
	/// ���i�q�b�g�\���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetHittableFlag() const
	{
		return hittableFlag;
	}
};

