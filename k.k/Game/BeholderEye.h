#pragma once
#include "MobEnemyBase.h"

#include "BeholderEyeStateContext.h"
#include "BeholderEyeInfo.h"


class BeholderEyeStateContext;

using namespace BeholderEyeStates;
using namespace BeholderEyeAnimationClips;
using namespace KnockBackInfo;


/// <summary>
/// ���u�G�l�~�[�F�ڋʃN���X
/// </summary>
class BeholderEye : public MobEnemyBase
{
public:
	~BeholderEye();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	/// <summary>
	/// ���ʃX�e�[�g�̏���
	/// </summary>
	void ProcessCommonTranstion();

	/// <summary>
	/// �v���C���[�̕�������
	/// </summary>
	void TurnToPlayer();

	/// <summary>
	/// �U���A�N�V�������n�߂�Ƃ��̏���
	/// </summary>
	void EntryAttackActionProcess();
	/// <summary>
	/// �U���A�N�V�����̍X�V����
	/// </summary>
	void UpdateAttackActionProcess();
	/// <summary>
	/// �U���A�N�V�������I��鎞�̏���
	/// </summary>
	void ExitAttackActionProcess();


	/// <summary>
	/// �q�b�g�A�N�V�����ɓ��������̏���
	/// </summary>
	void EntryHitActionProcess();
	/// <summary>
	/// �q�b�g�A�N�V�������̏���
	/// </summary>
	void UpdateHitActionProcess();
	/// <summary>
	/// �q�b�g�A�N�V�����I�����̏���
	/// </summary>
	void ExitHitActionProcess();

	/// <summary>
	/// ���S���̏���
	/// </summary>
	void DieProcess();

	/// <summary>
	/// �������̏���
	/// </summary>
	void WinProcess() override;

	/// <summary>
	/// �O������폜
	/// </summary>
	void DieFlomOutside(bool isPlayEffect = false) override;

private:

	/// <summary>
	/// �A�j���[�V�����N���b�v��ǂݍ���
	/// </summary>
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

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	void ProcessHit(DamageInfo damageInfo) override;



	/// <summary>
	/// �U������
	/// </summary>
	void Attack();

	/// <summary>
	/// ���@�������
	/// </summary>
	void ShotNormalBall();

private:

	std::unique_ptr<BeholderEyeStateContext> m_beholderEyeContext = nullptr;	//�X�e�[�g�R���e�L�X�g

	AnimationClip m_animationClip[enBeholderEyeAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//�����蔻��p�R���W����





	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;


	int m_eyeBallCount = 0;

	float count = 0.0f;

};

