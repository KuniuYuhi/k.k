#pragma once
#include "MobEnemyBase.h"
#include "CactusStateContext.h"
#include "CactusInfo.h"


class CactusStateContext;

using namespace CactusStates;
using namespace CactusAnimationClips;
using namespace KnockBackInfo;

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



private:

	std::unique_ptr<CactusStateContext> m_cactusContext = nullptr;	//�X�e�[�g�R���e�L�X�g

	AnimationClip m_animationClip[enCactusAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//�����蔻��p�R���W����





	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;

	




	float count = 0.0f;

};

