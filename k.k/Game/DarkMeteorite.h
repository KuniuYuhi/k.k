#pragma once

class Meteo;
class Player;
class Lich;

class DarkMeteorite : public IGameObject
{
public:
	DarkMeteorite();
	~DarkMeteorite();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// �T�C�Y�A�b�v
	/// </summary>
	void SizeUp();

	/// <summary>
	/// �����ƂɊւ��鏈��
	/// </summary>
	void ProcessShot();
	/// <summary>
	/// ���e�I�����u�Ԃ̏���
	/// </summary>
	/// <returns>���e�I�𐶐�������true</returns>
	bool ShotMeteo();

	/// <summary>
	/// ���e�I�̃^�[�Q�b�g���W��ݒ�
	/// </summary>
	/// <returns></returns>
	Vector3 SetMeteoTargetPosition();

	/// <summary>
	///	�{�̂̓����蔻�萶��
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// ���e�I�̐���
	/// </summary>
	/// <param name="targetposition">�^�[�Q�b�g���W</param>
	void CreateMeteo(Vector3 targetposition);

	/// <summary>
	/// ���e�I�𐶐�����^�C�}�[
	/// </summary>
	/// <returns></returns>
	bool CreateMeteoTimer();

	/// <summary>
	/// �n�ʂɃq�b�g�������̔���
	/// </summary>
	/// <param name="targetposition">�q�b�g�������̔��肵�������W</param>
	/// <param name="up">targetposition��Y���W���ǂꂾ���グ�邩</param>
	/// <param name="down">targetposition��Y���W���ǂꂾ�������邩</param>
	/// <returns></returns>
	bool IsHitGround(Vector3 targetposition,float up,float down);

	/// <summary>
	/// �ǂɃq�b�g�������̔���
	/// </summary>
	/// <param name="pos1">�n�_</param>
	/// <param name="pos2">�I�_</param>
	/// <returns></returns>
	bool IsHitWall();

	/// <summary>
	/// �V���b�g�X�e�[�g�Ǘ�
	/// </summary>
	void ShotManageState();

	/// <summary>
	/// �_�[�N���e�I�̈ړ��X�e�[�g�Ǘ�
	/// </summary>
	void DarkMeteoMoveManageState();

	void OnProcessFallStateTransition();
	void OnProcessChaseStateTransition();
	void OnProcessStraightStateTransition();

	/// <summary>
	/// �^�[�Q�b�g�Ɍ������x�N�g����ݒ�
	/// </summary>
	void SetToTargetDirection();

	/// <summary>
	/// �v���C���[�܂ł̋������߂����Btrue�ŋ߂�
	/// </summary>
	bool IsNearDistanceToPlayer();

	/// <summary>
	/// 
	/// </summary>
	void SetTRS();

	void SetLich(Lich* lich)
	{
		m_lich = lich;
	}

	void SetTargetPosition(Vector3 targetposition)
	{
		m_targetPosition = targetposition;
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	void SetShotStartFlag(bool flag)
	{
		m_ShotStartFlag = flag;
	}

	const bool& GetSizeUpFlag() const
	{
		return m_sizeUpFlag;
	}

	const bool& GetShotEndFlag() const
	{
		return m_shotEndFlag;
	}

	/// <summary>
	/// ���e�I�������I�������ɂł������e�I�������ǂ����̃t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	/// <returns></returns>
	void SetmLastBigMeteoShotFlag(bool flag)
	{
		m_lastBigMeteoShotFlag = flag;
	}

	/// <summary>
	/// �r�b�O���e�I�̍U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	const int& GetAtk() const
	{
		return m_bigMeteoAttack;
	}

	void SetChaseFlag(bool flag)
	{
		m_chaseFlag = flag;
	}

	/// <summary>
	/// �Q�[�����I��鎞�Ɏ��M�������Ƃ��ɐݒ肷��
	/// </summary>
	/// <param name="flag"></param>
	void SetGameEndFlag(bool flag)
	{
		m_gameEndFlag = flag;
	}

private:

	//�V���b�g�X�e�[�g
	enum EnShotState
	{
		enShotState_Meteo,
		enShotState_BigMeteo,
		enShotState_End
	};
	EnShotState						m_enShotState = enShotState_Meteo;

	//�_�[�N���e�I�̈ړ��X�e�[�g
	enum EnDarkMeteoMoveState
	{
		enDarkMeteoMoveState_None,
		enDarkMeteoMoveState_fall,
		enDarkMeteoMoveState_Chase,
		enDarkMeteoMoveState_straight
	};
	EnDarkMeteoMoveState			m_darkMeteoMoveState = enDarkMeteoMoveState_fall;

	Player*							m_player = nullptr;
	Lich*							m_lich = nullptr;
	EffectEmitter*					m_darkMeteoriteEffect;
	EffectEmitter*					m_windEffect;

	std::vector<Meteo*>				m_meteos;								//�����������e�I���i�[���郊�X�g

	ModelRender						m_model;

	CollisionObject*				m_collision = nullptr;

	Vector3							m_targetPosition = g_vec3Zero;
	Vector3							m_position = g_vec3Zero;
	Quaternion						m_rotation = g_quatIdentity;
	Vector3							m_scale = g_vec3One;
	Vector3							m_moveSpeed = g_vec3Zero;

	const Vector3					m_maxScale = { 10.0f,10.0f,10.0f };

	bool							m_sizeUpFlag = false;

	bool							m_ShotStartFlag = false;

	bool							m_shotEndFlag = false;

	float							m_createTime = 1.0f;
	float							m_createTimer = 0.0f;

	const int						m_createMeteoCount = 5;					//���e�I�𐶐����鐔
	int								m_meteoCounter = 0;

	bool							m_lastBigMeteoShotFlag = false;

	bool							m_isBigMeteoYDownFlag = false;			//�r�b�O���e�I��Y���W�������邩

	float							m_bigMeteoMoveCount = 4.0f;				//�r�b�O���e�I���v���C���[�Ɍ������x�N�g���Ɉړ������


	const int						m_bigMeteoAttack = 80;

	bool							m_chaseFlag = true;						//�v���C���[��ǂ������邩�̃t���O�Btrue�Œǂ�������

	bool							m_gameEndFlag = false;

};

