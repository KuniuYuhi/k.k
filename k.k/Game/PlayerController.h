#pragma once
#include "IComponent.h"

class PlayerController:public IComponent
{
public:

	PlayerController();

	~PlayerController();

	/// <summary>
	/// �R���|�[�l���g��ǉ��������̍ŏ��̏���
	/// </summary>
	void Start() override;

	/// <summary>
	/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
	/// </summary>
	void UpdateComponent() override;





	/// <summary>
	/// �R���g���[���[�̓��͂��󂯕t���邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetControllerInputEnabledFlag(bool flag)
	{
		m_isControllerInputEnabled = flag;
	}

	/// <summary>
	/// �R���g���[���[�̓��͂��󂯕t���邩�̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& IsControllerInputEnabledFlag() const
	{
		return m_isControllerInputEnabled;
	}



	/// <summary>
	/// �ʏ�U���{�^������������
	/// </summary>
	/// <returns></returns>
	bool IsTriggerNromalAttackButton();

	/// <summary>
	/// ����A�h��{�^������������
	/// </summary>
	/// <returns></returns>
	bool IsTriggerDefensiveActionButton();

	/// <summary>
	/// ����A�h��{�^�������������Ă��邩
	/// </summary>
	/// <returns></returns>
	bool IsPressDefensiveActionButton();

	/// <summary>
	/// ����̃{�^���̃g���K�[����
	/// </summary>
	/// <param name="button">���肵�����{�^��</param>
	/// <returns></returns>
	bool IsButtonTrigger(EnButton button);

	/// <summary>
	/// ����̃{�^���̃v���X����
	/// </summary>
	/// <param name="button">���肵�����{�^��</param>
	/// <returns></returns>
	bool IsButtonPress(EnButton button);

	/// <summary>
	/// ���X�e�B�b�N�̓��͗ʂ��擾
	/// </summary>
	/// <returns>���͗ʂ�-1�`1�͈̔͂Ŏ擾�B���͂��Ă��Ȃ��Ƃ���0���擾</returns>
	Vector3 GetLStickInput();

	/// <summary>
	/// �E�X�e�B�b�N�̓��͗ʂ��擾
	/// </summary>
	/// <returns>���͗ʂ�-1�`1�͈̔͂Ŏ擾�B���͂��Ă��Ȃ��Ƃ���0���擾</returns>
	Vector3 GetRStickInput();




private:

	bool m_isControllerInputEnabled = true;		//�R���g���[���[�̓��͂��󂯕t���邩

};

