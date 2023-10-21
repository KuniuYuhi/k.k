#include "k2EngineLowPreCompile.h"
#include "CascadeShadowMapMatrix.h"

void nsK2EngineLow::CascadeShadowMapMatrix::CalcLightViewProjectionCropMatrix(
	Camera& lightCamera,
	float cascadeAreaRateTbl[NUM_SHADOW_MAP]
)
{
	const auto& lvpMatrix = lightCamera.GetViewProjectionMatrix();

	//�J�����̑O�����A������A�E���������߂�	
	//�O�����ƉE�����͊��Ɍv�Z�ς݂Ȃ̂ŁA��������������Ă���
	const auto& cameraForward = g_camera3D->GetForward();
	const auto& cameraRight = g_camera3D->GetRight();

	//�J�����̏�����͑O�����ƉE�����̊O�ςŋ��߂�
	Vector3 cameraUp;
	cameraUp.Cross(cameraForward, cameraRight);

	// nearDepth�̓G���A�̍ŏ��[�x�l��\��
	// ��ԋ߂��G���A�̍ŏ��[�x�l�̓J�����̃j�A�N���b�v
	float nearDepth = g_camera3D->GetNear();

	for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
	{
		//�G���A�����鎋����̂W���_�����߂�
		//�G���A�̋ߕ��ʂ̒��S����̏�ʁA���ʂ܂ł̋��������߂�
		float nearY = tanf(g_camera3D->GetViewAngle() * 0.5f) * nearDepth;

		// �G���A�̋ߕ��ʂ̒��S����̉E�ʁA���ʂ܂ł̋��������߂�
		float nearX = nearY * g_camera3D->GetAspect();

		// �G���A�̉����ʂ̒��S����̏�ʁA���ʂ܂ł̋��������߂�
		float farY = tanf(g_camera3D->GetViewAngle() * 0.5f) * cascadeAreaRateTbl[areaNo];

		// �G���A�̉����ʂ̒��S����̉E�ʁA���ʂ܂ł̋��������߂�
		float farX = farY * g_camera3D->GetAspect();

		// �G���A�̋ߕ��ʂ̒��S���W�����߂�
		Vector3 nearPos = g_camera3D->GetPosition() + cameraForward * nearDepth;

		// �G���A�̉����ʂ̒��S���W�����߂�
		Vector3 farPos = g_camera3D->GetPosition() + cameraForward * cascadeAreaRateTbl[areaNo];

		// 8���_�����߂�
		Vector3 vertex[8];

		// �ߕ��ʂ̉E��̒��_
		vertex[0] += nearPos + cameraUp * nearY + cameraRight * nearX;

		// �ߕ��ʂ̍���̒��_
		vertex[1] += nearPos + cameraUp * nearY + cameraRight * -nearX;

		// �ߕ��ʂ̉E���̒��_
		vertex[2] += nearPos + cameraUp * -nearY + cameraRight * nearX;

		// �ߕ��ʂ̍����̒��_
		vertex[3] += nearPos + cameraUp * -nearY + cameraRight * -nearX;

		// �����ʂ̉E��̒��_
		vertex[4] += farPos + cameraUp * farY + cameraRight * farX;

		// �����ʂ̍���̒��_
		vertex[5] += farPos + cameraUp * farY + cameraRight * -farX;

		// �����ʂ̉E���̒��_
		vertex[6] += farPos + cameraUp * -farY + cameraRight * farX;

		// �����ʂ̍����̒��_
		vertex[7] += farPos + cameraUp * -farY + cameraRight * -farX;

		// 8���_�����C�g�r���[�v���W�F�N�V������Ԃɕϊ����āA
		// 8���_�̍ő�l�A�ŏ��l�����߂�
		Vector3 vMax, vMin;
		vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
		vMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };
		for (auto v : vertex)
		{
			lvpMatrix.Apply(v);
			vMax.Max(v);
			vMin.Min(v);
		}

		//�N���b�v�s������߂�
		//���K���X�N���[�����W�n�̕��ƍ�����2
		//AABB�����K���X�N���[�����W�n�Ƀt�B�b�g����悤�Ȋg�嗦�����Ƃ߂�
		float xScale = 2.0f / (vMax.x - vMin.x);
		float yScale = 2.0f / (vMax.y - vMin.y);
		//���W��-1����1�Ɏ��܂�悤�ɂ��炷
		float xOffset = (vMax.x + vMin.x) * -0.5f * xScale;
		float yOffset = (vMax.y + vMin.y) * -0.5f * yScale;
		Matrix clopMatrix;
		clopMatrix.m[0][0] = xScale;
		clopMatrix.m[1][1] = yScale;
		clopMatrix.m[3][0] = xOffset;
		clopMatrix.m[3][1] = yOffset;

		// ���C�g�r���[�v���W�F�N�V�����s��ɃN���b�v�s�����Z����
		m_lvpcMatrix[areaNo] = lvpMatrix * clopMatrix;

		// ���̃G���A�̋ߕ��ʂ܂ł̋�����������
		nearDepth = cascadeAreaRateTbl[areaNo];
	}


}

