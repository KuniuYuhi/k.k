#include "k2EngineLowPreCompile.h"
#include "Light.h"

void nsK2EngineLow::SceneLight::Init()
{
	//ディレクションライトのカラー
	m_light.directionalLight.color.x = 0.5f;
	m_light.directionalLight.color.y = 0.5f;
	m_light.directionalLight.color.z = 0.5f;
	//ディレクションライトの方向
	m_light.directionalLight.lightDirection.x = 1.0f;
	m_light.directionalLight.lightDirection.y = -1.0f;
	m_light.directionalLight.lightDirection.z = -1.0f;
	m_light.directionalLight.lightDirection.Normalize();
	//環境光
	m_light.ambientLight.x = 0.3f;
	m_light.ambientLight.y = 0.3f;
	m_light.ambientLight.z = 0.3f;
	//半球ライトの地面色
	m_light.hemiSphereLight.groundColor = Vector3(0.5f,0.5f,0.5f);
	//半球ライトの天球色
	m_light.hemiSphereLight.skyColor = Vector3(0.5f, 0.5f, 0.5f);
	//半球ライトの地面の法線
	m_light.hemiSphereLight.groundNormal = Vector3(0.0f, 1.0f, 0.0f);

	m_light.cameraEyePos = g_camera3D->GetPosition();
}
