#pragma once



class Pause:public IGameObject
{
public:
	Pause();
	~Pause();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


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
	/// 
	/// </summary>
	void Select();

private:
	SpriteRender m_bgRender;		//�w�i
	
	bool m_gameExitFlag = false;
};

