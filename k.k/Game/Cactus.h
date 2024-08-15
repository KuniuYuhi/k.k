#pragma once
#include "MobEnemyBase.h"


/// <summary>
/// ���u�G�l�~�[�F�J�N�^�X�N���X
/// </summary>
class Cactus : public MobEnemyBase
{
public:
	~Cactus();

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



private:

	bool m_isSettingComponents = false;

};

