#pragma once


/// <summary>
/// キャラクターの基底クラス
/// </summary>
class ICharacter:public IGameObject
{
public:
	virtual ~ICharacter() = default;



	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}


	Vector3& GetPosition()
	{
		return m_position;
	}

	const Vector3& GetForward() const
	{
		return m_forward;
	}

	const Vector3& GetScale() const
	{
		return m_scale;
	}

	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	ModelRender& GetModelRender()
	{
		return m_modelRender;
	}

protected:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_forward = Vector3::Front;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

	ModelRender m_modelRender;

};

