#pragma once
#include "MobEnemyBase.h"
#include "SlimeStateContext.h"
#include "SlimeInfo.h"

//#include "KnockBackInfo.h"


class SlimeStateContext;

using namespace SlimeStates;
using namespace SlimeAnimationClips;
using namespace KnockBackInfo;


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

	void InitModel() override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	/// <summary>
	/// ���ʃX�e�[�g�̏���
	/// </summary>
	void ProcessCommonTranstion();


	/// <summary>
	/// �^�[�Q�b�g�̂ق��ɐU�����
	/// </summary>
	void TrunToTarget();




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
	/// 
	/// </summary>
	void EntryHitActionProcess();
	/// <summary>
	/// 
	/// </summary>
	void UpdateHitActionProcess();

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
	/// �U���\���H
	/// </summary>
	/// <returns>�\�Ȃ�true</returns>
	bool IsAttackable();

private:

	std::unique_ptr<SlimeStateContext> m_slimeContext = nullptr;	//�X�e�[�g�R���e�L�X�g

	AnimationClip m_animationClip[enSlimeAnimClip_num];

	CollisionObject* m_attackCollision = nullptr;		//�����蔻��p�R���W����

	



	bool m_isCreateAttackCollision = false;

	int m_headBoonId = -1;

	bool m_isSettingComponents = false;

	float m_attackIntarvalTimer = 0.0f;




	float count = 0.0f;

	
	
	

	

};

