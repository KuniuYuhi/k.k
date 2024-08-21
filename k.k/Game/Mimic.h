#pragma once
#include "MobEnemyBase.h"

/// <summary>
/// ���u�G�l�~�[�F�~�~�b�N�N���X
/// </summary>
class Mimic : public MobEnemyBase
{
public:
	~Mimic();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;


private:


	/// <summary>
	/// �S�ẴR���|�[�l���g��������
	/// </summary>
	void InitComponents();


	/// <summary>
	/// ���g���I�u�W�F�N�g�v�[���ɖ߂�
	/// </summary>
	void ReleaseThis() override;

	/// <summary>
	/// ����ɒǉ�����R���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void AddMoreComponent() override;

	/// <summary>
	/// �R���W�������쐬����
	/// </summary>
	void CreateCollisionObject() override;

private:

	bool m_isSettingComponents = false;

};

