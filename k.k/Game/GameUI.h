#pragma once
#include "MyWeapon.h"

class Game;

class PlayerGameUI;


class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();


	bool Start();
	void Update();
	void Render(RenderContext& rc);

	
private:
	
	/// <summary>
	/// �X�v���C�g�����_�[�̏������B
	/// </summary>
	/// <param name="spriterender">�������������X�v���C�g�����_�[</param>
	/// <param name="filePath">�摜�̃t�@�C���p�X</param>
	/// <param name="width">�摜�̕�</param>
	/// <param name="height">�摜�̍���</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void InitSpriteRender(
		SpriteRender& spriterender,
		const char* filePath,
		const float width, const float height,
		Vector3 position,
		Vector3 scale = g_vec3One,
		Quaternion rotation = g_quatIdentity
	);
	
	/// <summary>
	/// �t�H���g�����_�[�̏������B
	/// </summary>
	/// <param name="fontRender">�������������t�H���g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="color">�J���[</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="isShadowParam">�e(�֊s)�����邩</param>
	/// <param name="shadowOffset">�I�t�Z�b�g��</param>
	/// <param name="shadowColor">�e�̃J���[</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);
	
private:


	PlayerGameUI* m_playerGameUI = nullptr;







};

