#pragma once
#include "IComponent.h"

#include "KnockBackInfo.h"
#include "WeaponInfo.h"



class CharacterBase;
class IMagicBall;

using namespace KnockBackInfo;
using namespace WeaponAttribute;

/// <summary>
/// �_���[�W�v���o�C�_�[�̍\����
/// </summary>
namespace DamageInformaiton {

	struct DamageInfo
	{
		
		int attackId = -1;						//�U��ID
		int attackPower = 0;					//�U����
		float knockBackTimeScale = 0.0f;			//�m�b�N�o�b�N�̎��ԊԊu
		EnKnockBackPattern knockBackPattern;							//�m�b�N�o�b�N�p�^�[��
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None;		//����̑���
	};
}

using namespace DamageInformaiton;


/// <summary>
/// �_���[�W�Ɋւ������񋟂���N���X�B�R���|�[�l���g
/// </summary>
class DamageProvider:public IComponent
{
public:

	DamageProvider();
	~DamageProvider();

	/// <summary>
	/// �R���|�[�l���g��ǉ��������̍ŏ��̏���
	/// </summary>
	void Start() override;

	/// <summary>
	/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
	/// </summary>
	void UpdateComponent() override;

	/// <summary>
	/// �񋟎҂̃L�����N�^�[�̃C���X�^���X��ݒ�
	/// </summary>
	/// <param name="characterBase"></param>
	void SetProviderCharacterInstance(CharacterBase* characterBase)
	{
		m_characterBase = characterBase;
	}
	/// <summary>
	/// �񋟎҂̖��@���̃C���X�^���X��ݒ�
	/// </summary>
	/// <param name="magicBall"></param>
	void SetProviderMagicBallInstance(IMagicBall* magicBall)
	{
		m_magicBall = magicBall;
	}


	/// <summary>
	/// �_���[�W�񋟎҂̍��W���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetProviderPostion();

	/// <summary>
	/// �񋟎҂̃_���[�W�����擾
	/// </summary>
	/// <returns></returns>
	DamageInfo GetProviderDamageInfo()
	{
		return m_damageInfo;
	}

	/// <summary>
	/// �U��ID��ݒ�
	/// </summary>
	/// <param name="idNum"></param>
	void SetAttackId(int  idNum)
	{
		m_damageInfo.attackId = idNum;
	}
	/// <summary>
	/// �U��ID���擾
	/// </summary>
	/// <returns></returns>
	int GetAttackId()
	{
		return m_damageInfo.attackId;
	}

	/// <summary>
	/// �����_���ɒ�����̃_���[�W���擾
	/// </summary>
	/// <returns></returns>
	int GetAdjustedDamage()
	{
		AdjustPowerRandomly();
		return m_damageInfo.attackPower;
	}


	/// <summary>
	/// �_���[�W����ݒ�
	/// </summary>
	/// <param name="attackId"></param>
	/// <param name="attackPower"></param>
	/// <param name="knockBackPattern"></param>
	/// <param name="weaponAttribute"></param>
	void SetDamageInfo(
		int attackId,
		int attackPower,
		float knockBackTimeScale,
		EnKnockBackPattern knockBackPattern,
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None
	)
	{
		m_damageInfo.attackId = attackId;
		//�����_���ɕς���
		m_damageInfo.attackPower = attackPower;
		//AdjustPowerRandomly();

		m_damageInfo.knockBackTimeScale = knockBackTimeScale;
		m_damageInfo.knockBackPattern = knockBackPattern;
		m_damageInfo.weaponAttribute = weaponAttribute;
	}

	/// <summary>
	/// �U���͂������_���ɒ���
	/// </summary>
	void AdjustPowerRandomly();

	/// <summary>
	/// �ŏI�I�ȃm�b�N�o�b�N�p�^�[�����v�Z
	/// </summary>
	/// <param name="knockBackToleranceLevel">�m�b�N�o�b�N�ϐ����x��</param>
	void CalcFinalKnockBackPattern(int knockBackToleranceLevel);

	/// <summary>
	/// �q�b�g����
	/// </summary>
	void Hit()
	{
		m_isHit = true;
	}

	/// <summary>
	/// �q�b�g�������H
	/// </summary>
	/// <returns></returns>
	const bool IsHit() const
	{
		return m_isHit;
	}
	

	//todo�@�@�����\���̂ɂ��ēn������
	//��_���[�W�ʁA�U��ID�A�m�b�N�o�b�N�f�[�^

protected:
	
	CharacterBase* m_characterBase = nullptr;	//���̃R���|�[�l���g�������Ă���L�����N�^�[

	IMagicBall* m_magicBall = nullptr;		//���̃R���|�[�l���g�������Ă���}�W�b�N�{�[��

	DamageInfo m_damageInfo;


	bool m_isHit = false;

};

