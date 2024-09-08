#pragma once

class Summoner;
class SummonerStateContext;

/// <summary>
/// �T���i�[�̃X�e�[�g�̊��N���X
/// </summary>
class ISummonerState
{
public:

	ISummonerState(Summoner* summoner, SummonerStateContext* context)
	{
		m_summoner = summoner;
		m_summonerStateContext = context;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ISummonerState() {}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// ���������̏���
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	virtual void Exit() = 0;

protected:
	//�T���i�[�̃C���X�^���X
	Summoner* m_summoner = nullptr;
	SummonerStateContext* m_summonerStateContext = nullptr;

};

