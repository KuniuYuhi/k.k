#pragma once

#include "MyRenderer.h"

namespace nsK2EngineLow {

	
	/// <summary>
	/// �f�B���N�V�������C�g�\����
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;		//���C�g�̕���
		float pad0;					//�p�e�B���O
		Vector3 color;				//���C�g�̃J���[
		float pad1;					//�p�e�B���O
		
	};

	/// <summary>
	/// �|�C���g���C�g�\����
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//���W
		float pad0;				//�p�e�B���O0
		Vector3 color;			//�J���[
		float pad1;				//�p�e�B���O1
		Vector3 attn;			//x�ɉe���͈́Ay�ɉe�����ɗݏ悷��p�����[�^
		int	isUse = false;		//�g�p���t���O

	};

	/// <summary>
	/// �X�|�b�g���C�g�\����
	/// </summary>
	struct SpotLight
	{
		Vector3 position;		//���W
		float pad0;				//�p�e�B���O0
		Vector3 angle;			//x�Ɏˏo�p�x�Ay�ɉe���ɗݏ悷��p�����[�^
		float pad1;				//�p�e�B���O1
		Vector3 color;			//�J���[
		float pad2;				//�p�e�B���O2
		Vector3 attn;			//x�ɉe���͈́Ay�ɉe�����ɗݏ悷��p�����[�^
		int	isUse = false;		//�g�p���t���O
		Vector3 direction;		//�Ǝ˕���
		float pad3;				//�p�e�B���O3
	};

	/// <summary>
	/// �������C�g
	/// </summary>
	struct HemiSphereLight
	{
		Vector3 groundColor;		//�n�ʐF
		float pad0;					//�p�e�B���O0
		Vector3 skyColor;			//�V���F
		int isUse = false;			//�g�p�t���O
		Vector3 groundNormal;		//�n�ʂ̖@��
		float pad1;
	};

	/// <summary>
	/// ���C�g�\����
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight;		//�f�B���N�V�������C�g
		PointLight			pointLight;				//�|�C���g���C�g
		SpotLight			spotLight;				//�X�|�b�g���C�g				
		HemiSphereLight		hemiSphereLight;		//�������C�g
		Vector3             ambientLight;			//����(�A���r�G���g���C�g)
		float				pad1;					//�p�e�B���O�P
		Vector3				cameraEyePos;			//�J�����̍��W
		float				pad2;
		Matrix				mLVP[NUM_SHADOW_MAP];					//���C�g�r���[�v���W�F�N�V�����s��
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
			for (int mapNo = 0; mapNo < NUM_SHADOW_MAP; mapNo++)
			{
				m_light.mLVP[mapNo] = LVP;
			}
			
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
		///�����̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �����̐ݒ�
		/// </summary>
		/// <param name="ambient">�����̋���</param>
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

		////////////////////////////////////////////////////////
		///�|�C���g���C�g�̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �|�C���g���C�g�̐ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		/// <param name="color">�F</param>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 attn)
		{
			SetPointLightPosition(pos);
			SetPointLightColor(color);
			SetPointLightAttn(attn);
			UsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g�̍��W
		/// </summary>
		/// <param name="pos">���W</param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.position = pos;
		}

		/// <summary>
		/// �|�C���g���C�g�̐F
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(Vector3 color)
		{
			m_light.pointLight.color = color;
		}

		/// <summary>
		/// �e���͈͂Ɖe�����ɗݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_light.pointLight.attn = attn;
		}

		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_light.pointLight.isUse = true;
		}

		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_light.pointLight.isUse = false;
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_light.pointLight.position;
		}

		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetPointLightColor() const
		{
			return m_light.pointLight.color;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_light.pointLight.attn;
		}

		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse() const
		{
			return m_light.pointLight.isUse;
		}

		////////////////////////////////////////////////////////
		///�X�|�b�g���C�g�̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �X�|�b�g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">�ʒu</param>
		/// <param name="color">�F</param>
		/// <param name="range">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		/// <param name="direction">����</param>
		/// <param name="angle">�p�x</param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			SetSpotLightPosition(pos);
			SetSpotLightColor(color);
			SetSpotLightAttn(attn);
			SetSpotLightDirection(direction);
			SetSpotLightAngle(angle);
			UseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ�
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_light.spotLight.position = pos;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector3 color)
		{
			m_light.spotLight.color = color;
		}

		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_light.spotLight.attn = attn;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_light.spotLight.direction = direction;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_light.spotLight.angle = angle;
		}

		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_light.spotLight.isUse = true;
		}

		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_light.spotLight.isUse = false;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_light.spotLight.position;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetSpotLightColor() const
		{
			return m_light.spotLight.color;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_light.spotLight.attn;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_light.spotLight.direction;
		}

		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_light.spotLight.angle;
		}

		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse() const
		{
			return m_light.spotLight.isUse;
		}

		////////////////////////////////////////////////////////
		///�������C�g�̊֐�
		////////////////////////////////////////////////////////

		/// <summary>
		/// �������C�g��ݒ肷��
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		/// <param name="skyColor">�V���F</param>
		/// <param name="groundNormal">�n�ʂ̖@��</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF��ݒ�
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_light.hemiSphereLight.groundColor = groundColor;
		}

		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_light.hemiSphereLight.skyColor = skyColor;
		}

		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_light.hemiSphereLight.groundNormal = normal;
		}

		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_light.hemiSphereLight.isUse = true;
		}

		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_light.hemiSphereLight.isUse = false;
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_light.hemiSphereLight.groundColor;
		}

		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_light.hemiSphereLight.skyColor;
		}

		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_light.hemiSphereLight.groundNormal;
		}

		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse() const
		{
			return m_light.hemiSphereLight.isUse;
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



