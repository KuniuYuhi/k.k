#pragma once

class Meteo;

class DarkMeteo:public IGameObject
{
public:
	DarkMeteo();
	~DarkMeteo();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���e�I������A�̏����̗���
	/// </summary>
	enum EnDarkMeteoState
	{
		enDarkMeteoState_Charge,			//���e�I���`���[�W����(�傫������)
		enDarkMeteoState_ShotMeteo,			//���e�I������
		enDarkMeteoState_ShotDarkMeteo,		//����̏󋵂Ń`���[�W�����_�[�N���e�I������
		enDarkMeteoState_End				//�����I���
	};

	/// <summary>
	/// �`���[�W�����t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetChargeCompleteFlag() const
	{
		return m_chargeCompleteFlag;
	}

	const bool& GetEndFlag() const
	{
		return m_endFlag;
	}

	/// <summary>
	/// �_�[�N���e�I�������̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetShotDarkMeteoFlag(bool flag)
	{
		m_shotDarkMeteoFlag = flag;
	}

	/// <summary>
	/// �_�[�N���e�I�̍��W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetDarkMeteoPosition(Vector3 position)
	{
		m_darkMeteoPositon = position;
	}

	/// <summary>
	/// ���̃��e�I�X�e�[�g��ݒ�
	/// </summary>
	/// <param name="nextDarkMeteoState"></param>
	void SetNextDarkMeteoState(EnDarkMeteoState nextDarkMeteoState)
	{
		m_enDarkMeteoState = nextDarkMeteoState;
	}

private:

	/// <summary>
	/// �_�[�N���e�I�̏�����
	/// </summary>
	void InitDarkMeteoEffect();

	/// <summary>
	/// ���e�I�̐���
	/// </summary>
	/// <param name="endPosition">�I�_</param>
	void CreateMeteo(Vector3 endPosition);

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();

	void OnProcessDarkMeteoChargeTranstion();
	void OnProcessShotMeteoTranstion();
	void OnProcessShotDarkMeteoTranstion();
	void OnProcessEndTranstion();

	/// <summary>
	/// ���e�I���ǂɃq�b�g���邩
	/// </summary>
	/// <returns></returns>
	bool IsMeteoHitWall(Vector3 endPosition);

	/// <summary>
	/// ���e�I�������Ă��悢��
	/// </summary>
	/// <returns></returns>
	bool IsShotMeteo();

private:

	ModelRender model[5];

	std::vector<Meteo*>				m_meteoList;
	
	EnDarkMeteoState m_enDarkMeteoState = enDarkMeteoState_Charge;


	Vector3 m_darkMeteoPositon = g_vec3Zero;
	Vector3 m_darkMeteoScale = g_vec3One;

	EffectEmitter* m_darkMeteoriteEffect;//�_�[�N���e�I�{�̂̃G�t�F�N�g

	float m_mulScaleDarkMeteo = 1.0f;	//�_�[�N���e�I�̃X�P�[���̊g�嗦

	bool m_chargeCompleteFlag = false;

	bool m_shotDarkMeteoFlag = false;

	bool m_endFlag = false;


	float m_shotMeteoTimer = 0.0f;

	int m_shotMeteoCounter = 0;

};

