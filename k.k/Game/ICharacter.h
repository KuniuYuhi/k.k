#pragma once


/// <summary>
/// キャラクターの基底クラス
/// </summary>
class ICharacter:public IGameObject
{
public:
	virtual ~ICharacter() = default;

	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Vector3& GetScale() const
	{
		return m_scale;
	}

	const Vector3& GetPosition() const
	{
		return m_position;
	}

protected:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

};

