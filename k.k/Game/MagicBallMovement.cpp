#include "stdafx.h"
#include "MagicBallMovement.h"
#include "IMagicBall.h"


namespace {
	float MUL_STRAGHT_SPEED = 10.0f;
	float MUL_CHASE_SPEED = 4.0f;
}

MagicBallMovement::MagicBallMovement()
{
}

MagicBallMovement::~MagicBallMovement()
{
}

void MagicBallMovement::Start()
{
}

void MagicBallMovement::UpdateComponent()
{
}

void MagicBallMovement::MoveStraight(float yUp, bool setYUp)
{
	//前方向を取得
	Vector3 moveSpeed = m_magicBall->GetForward();
	moveSpeed.Normalize();

	moveSpeed *= MUL_STRAGHT_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter()*g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	if(setYUp) currentPos.y = yUp;

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);

}

void MagicBallMovement::MoveFall()
{
	//前方向を取得
	Vector3 moveSpeed = g_vec3Down;

	moveSpeed *= MUL_STRAGHT_SPEED;

	moveSpeed *= m_magicBall->GetSpeedParameter() * g_gameTime->GetFrameDeltaTime();
	m_magicBall->SetMoveSpeed(moveSpeed);

	Vector3 currentPos = m_magicBall->GetPosition();

	currentPos += moveSpeed;

	m_magicBall->SetPosition(currentPos);
}

void MagicBallMovement::MoveChase(
	Vector3 targetPosition, float radius, float yUp, bool setYUp)
{

	// プレイヤーの位置に向かうベクトル
	Vector3 directionToPlayer = targetPosition - m_magicBall->GetPosition();
	//長さを計算
	float length = directionToPlayer.Length();
	// 現在の弾の移動ベクトル
	Vector3 currentDirection = m_magicBall->GetMoveSpeed();
	currentDirection.Normalize();

	//ベクトルの長さが範囲内だったら
	if (length < radius || m_isNearPlayerDistance == true)
	{
		m_isNearPlayerDistance = true;
		//追尾せず前方向を使う
		currentDirection = m_magicBall->GetForward();
		directionToPlayer = m_magicBall->GetForward();
	}
	directionToPlayer.Normalize();

	// プレイヤー方向と現在の弾の方向の補間 (追尾速度を調整)
	float homingSpeed = 0.6f; // 追尾の速さ
	Vector3 newDirection = 
		currentDirection * (1.0f - homingSpeed) + directionToPlayer * homingSpeed;

	newDirection.Normalize();

	Vector3 moveSpeed = 
		(newDirection * MUL_CHASE_SPEED) * m_magicBall->GetSpeedParameter() * g_gameTime->GetFrameDeltaTime();

	// 新しい速度ベクトルを設定
	//移動速度設定
	m_magicBall->SetMoveSpeed(moveSpeed);

	//前方向を設定する
	moveSpeed.Normalize();
	m_magicBall->SetForward(moveSpeed);

	//
	Vector3 currentPos = m_magicBall->GetPosition();

	if (setYUp) currentPos.y = yUp;

	currentPos += m_magicBall->GetMoveSpeed();

	m_magicBall->SetPosition(currentPos);


}
