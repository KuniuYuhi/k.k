#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
	//�f�B���N�V�������C�g�̃J���[
	m_light.directionalLight.color.x = 0.5f;
	m_light.directionalLight.color.y = 0.5f;
	m_light.directionalLight.color.z = 0.5f;
	//�f�B���N�V�������C�g�̕���
	m_light.directionalLight.lightDirection.x = 1.0f;
	m_light.directionalLight.lightDirection.y = -1.0f;
	m_light.directionalLight.lightDirection.z = -1.0f;
	m_light.directionalLight.lightDirection.Normalize();
	//����
	m_light.ambientLight.x = 0.3f;
	m_light.ambientLight.y = 0.3f;
	m_light.ambientLight.z = 0.3f;
	//�������C�g�̒n�ʐF
	m_light.hemiSphereLight.groundColor = Vector3(0.5f,0.5f,0.5f);
	//�������C�g�̓V���F
	m_light.hemiSphereLight.skyColor = Vector3(0.5f, 0.5f, 0.5f);
	//�������C�g�̒n�ʂ̖@��
	m_light.hemiSphereLight.groundNormal = Vector3(0.0f, 1.0f, 0.0f);

	m_light.cameraEyePos = g_camera3D->GetPosition();
}
