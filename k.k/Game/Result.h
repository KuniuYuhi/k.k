#pragma once
class ResultSeen:public IGameObject
{
public:
	ResultSeen();
	~ResultSeen();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitSprite();

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �������̏���
	/// </summary>
	void WinState();
	/// <summary>
	/// �s�k���̏���
	/// </summary>
	void LoseState();

	void ResultEnd();

	/// <summary>
	/// �X�v���C�g�̍��W�A�T�C�Y�A��]��ݒ�
	/// </summary>
	/// <param name="spriterender">�X�v���C�g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

	/// <summary>
	/// �~�`���C�v�I���t���O���擾
	/// </summary>
	/// <returns></returns>
	bool GetRoundWipeEndFlag()
	{
		return m_roundWipeEndFlag;
	}

	/// <summary>
	/// ���U���g�̏������I��������̃t���O���擾
	/// </summary>
	/// <param name="flag"></param>
	/// <returns></returns>
	const bool& GetResultEndFlag() const
	{
		return m_resultEndFlag;
	}

	//���s
	enum EnOutcomeState
	{
		enOutcomeNone,
		enOutcome_Win,
		enOutcome_Lose
	};
	/// <summary>
	/// ���s��ݒ肷��
	/// </summary>
	/// <param name="outcom"></param>
	void SetOutcome(EnOutcomeState outcom)
	{
		m_enOutComeState = outcom;
	}

	/// <summary>
	/// ���U���g�ł̏����X�e�b�v
	/// </summary>
	enum EnResultStep
	{
		enResultStep_main,
		enResultStep_end,
	};

	/// <summary>
	/// �Q�[���N���A�̏���
	/// </summary>
	void OnProcessGameClearMainTranstion();
	/// <summary>
	/// �Q�[���I�[�o�[�̏���
	/// </summary>
	void OnProcessGameOverMainTranstion();
	/// <summary>
	/// �I�����̏���
	/// </summary>
	void OnProcessEndTranstion();

private:

	SpriteRender m_winTextRender;
	SpriteRender m_rightRender;

	SpriteRender m_loseTextRender;

	SpriteRender m_goToTitleRender;




	SpriteRender m_resultSprite;


	EnOutcomeState m_enOutComeState = enOutcomeNone;
	EnResultStep m_enResultStep = enResultStep_main;

	float m_wipeSize = 1200.0f;
	bool m_roundWipeEndFlag = false;


	Quaternion m_rightRotation = g_quatIdentity;

	bool m_resultEndFlag = false;
	bool m_roundWipeStartFlag = false;


	float m_goToTitleTime = 1.0f;

	bool m_possibleGoToTitleFlag = false;

};

