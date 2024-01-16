#pragma once
class Loading:public IGameObject
{
public:
	Loading();
	~Loading();

	/// <summary>
	/// ���[�f�B���O�̕��@
	/// </summary>
	enum EnMethodLoading
	{
		enMethodLoading_None,
		enMethodLoading_Fade,
		enMethodLoading_DirectionWipe,
		enMethodLoading_RoundWipe
	};

	/// <summary>
	/// ���[�f�B���O�X�e�[�g
	/// </summary>
	enum EnLoadingState
	{
		enLoadingState_None,
		enLoadingState_In,
		enLoadingState_Out
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���[�f�B���O���n�߂�
	/// </summary>
	/// <param name="methodLoading"></param>
	void StartLoading(EnMethodLoading methodLoading);

	/// <summary>
	/// ���[�f�B���O���I���
	/// </summary>
	/// <param name="methodLoading"></param>
	void EndLoading(EnMethodLoading methodLoading);

	/// <summary>
	/// ���݂̃��[�f�B���O�̕��@���擾
	/// </summary>
	/// <returns></returns>
	EnMethodLoading& GetCurrentMethodLoading()
	{
		return m_enMethodLoading;
	}

	/// <summary>
	/// ��ԊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// 
	/// </summary>
	void ProcessInLoadingStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void ProcessOutLoadingStateTransition();

	/// <summary>
	/// �t�F�[�h����
	/// </summary>
	/// <param name="loadingState">�X�e�[�g</param>
	void ProcessFade(EnLoadingState loadingState);

	/// <summary>
	/// �������C�v����
	/// </summary>
	/// <param name="loadingState">�X�e�[�g</param>
	void ProcessDirectionWipe(EnLoadingState loadingState);

	/// <summary>
	/// �~�`���C�v����
	/// </summary>
	/// <param name="loadingState">�X�e�[�g</param>
	void ProcessRoundWipe(EnLoadingState loadingState);


private:
	SpriteRender m_backSprite;
	SpriteRender m_loadTextSprite;

	EnLoadingState m_enLoadingState = enLoadingState_None;
	EnMethodLoading m_enMethodLoading = enMethodLoading_None;

	float m_currentAlpha = 0.0f;

	bool m_drawLoadSpriteFlag = false;

};

