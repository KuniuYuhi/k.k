#pragma once
#include "MobEnemyBase.h"
#include "SlimeStateContext.h"
#include "SlimeInfo.h"

class SlimeStateContext;


using namespace SlimeAnimationClips;

/// <summary>
/// ���u�G�l�~�[�F�X���C���N���X
/// </summary>
class Slime : public MobEnemyBase
{
public:
	~Slime();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;


private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	void LoadAnimationClip();

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

	std::unique_ptr<SlimeStateContext> m_slimeContext = nullptr;	//�X�e�[�g�R���e�L�X�g

	AnimationClip m_animationClip[enSlimeAnimClip_num];


	bool m_isSettingComponents = false;


};

