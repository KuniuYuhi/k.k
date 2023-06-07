#pragma once
namespace nsK2EngineLow {

	const int MAM_DIRECTIONAL_LIGHT = 4;   //�f�B���N�V�������C�g�̍ő吔

	struct DirectionalLight
	{
		Vector3 lightDirection;		//���C�g�̕���
		float pad0;					//�p�e�B���O
		Vector3 color;				//���C�g�̃J���[
		float pad1;					//�p�e�B���O
		
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight;
		float pad0;
		float pad1;
		Matrix mLVP;
		Vector3 cameraEyePos;
	};


	class SceneLight
	{
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="LVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void SetmLVP(Matrix LVP)
		{
			m_light.mLVP = LVP;
		}
		//////////////////////////////////////////////////////////////////////
		///�f�B���N�V�������C�g�̊֐�
		////////////////////////////////////////////////////////////////////// 
		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			SetDirLightDirection(direction);
			SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_light.directionalLight.lightDirection = direction;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_light.directionalLight.color = color;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_light.directionalLight.lightDirection;
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_light.directionalLight.color;
		}

		////////////////////////////////////////////////////////
		///�J�����̈ʒu�̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_light.cameraEyePos = eyePos;
		}

	private:
		Light m_light;
	};
}



