#pragma once

#include "IWeapon.h"

class BigSword:public IWeapon
{
public:
	BigSword();
	~BigSword();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;


private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;

	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;

	/// <summary>
	/// �E��(�v���C���[)�̃A�j���[�V�����N���b�v�����g�̕���̂��̂ɕύX
	/// </summary>
	/// <param name="modelrender"></param>
	void SetBraveAnimationClip(ModelRender* modelrender) override;


private:
	ModelRender m_modelBigSword;		//�����f��

	Vector3 m_swordPos = g_vec3Zero;

	Matrix m_swordMatrix = g_matIdentity;

	//������������鎞�̃{�[��ID
	int m_armedSwordBoonId = -1;

};

