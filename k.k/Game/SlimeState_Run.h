#pragma once
#include "IMobEnemyState.h"
#include "SlimeStateContext.h"

class Slime;
class SlimeStateContext;


class SlimeState_Run : public IMobEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="slime">�X���C���̃C���X�^���X</param>
	/// <param name="stateContest">�X�e�[�g�R���e�L�X�g</param>
	SlimeState_Run(Slime* slime, SlimeStateContext* stateContest)
	{
		m_slime = slime;
		m_stateCotext = stateContest;
	}

	~SlimeState_Run();

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// ���������̏���
	/// </summary>
	void Entry() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Ubdate() override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	void Exit() override;


private:
	Slime* m_slime = nullptr;
	SlimeStateContext* m_stateCotext = nullptr;
};

