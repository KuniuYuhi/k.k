#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"



class Status_Player;
class PlayerMovement;
class PlayerController;


class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	virtual void Damage() override;

	/// <summary>
	/// ���S������
	/// </summary>
	virtual void Dead() override;

	/// <summary>
	/// ��]���邽�߂̕����x�N�g����ݒ�
	/// </summary>
	/// <param name="rotateDirection"></param>
	void SetRotateDirection(Vector3 rotateDirection)
	{
		m_rotateDirection = rotateDirection;
	}


protected:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void SettingDefaultComponent() override;


	/// <summary>
	/// �ړ�����
	/// </summary>
	void Movement();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();

private:

	Status_Player m_status;

	

	PlayerMovement* m_playerMovement = nullptr;

	PlayerController* m_playerContoller = nullptr;

};

