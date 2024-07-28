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

protected:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_forward = Vector3::Front;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rotation = Quaternion::Identity;

};

