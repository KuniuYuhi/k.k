#pragma once

class Game;

class GameCamera:public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();


private:
	Game* game = nullptr;

	Vector3				m_toCameraPos = Vector3::Zero;		//�J�����ʒu���璍���_�Ɍ������x�N�g��
	Vector3				m_position = Vector3::Zero;		//�J�������W
	Vector3				m_target = Vector3::Zero;		//�J���������_
	Quaternion			m_rotation = Quaternion::Identity;	//��]
};

