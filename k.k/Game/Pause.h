#pragma once



class Pause:public IGameObject
{
public:
	Pause();
	~Pause();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


	/// <summary>
	/// �Q�[���I���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetGameExitFlag() const
	{
		return m_gameExitFlag;
	}

private:
	/// <summary>
	/// �摜�̏�����
	/// </summary>
	void InitSprite();
	/// <summary>
	/// �I��
	/// </summary>
	void Select();

private:
	SpriteRender m_bgRender;		//�w�i
	
	bool m_gameExitFlag = false;
};

