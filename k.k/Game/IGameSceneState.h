#pragma once

/// <summary>
/// �Q�[���V�[���X�e�[�g�̊��N���X
/// </summary>
class IGameSceneState
{
public:

	//�f�X�g���N�^
	virtual ~IGameSceneState(){}

	/// <summary>
	/// �X�e�[�g�J�n���̏���
	/// </summary>
	virtual void Entry() {}

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// �X�e�[�g�I�����̏���
	/// </summary>
	virtual void Exit() {}

protected:

};