#pragma once
#include "IAttackObject.h"


class DarkSpearObj:public IAttackObject
{
public:
	DarkSpearObj();
	~DarkSpearObj();

	enum EnMaxScaleSize
	{
		enMaxScaleSize_Small,
		enMaxScaleSize_Medium,
		enMaxScaleSize_Large
	};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// �_�[�N�X�s�A�̍ő�T�C�Y��ݒ�
	/// </summary>
	/// <param name="maxScaleSize">�T�C�Y</param>
	void SetMaxScale(EnMaxScaleSize maxScaleSize);

	/// <summary>
	/// �_�[�N�X�s�A�̍ő�T�C�Y��ݒ�
	/// </summary>
	/// <param name="sizeNumer">0����2�܂ŁA����������</param>
	void SetMaxScale(int sizeNumer);


private:

	/// <summary>
	/// �����蔻��̍쐬
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// �G�t�F�N�g�Đ�
	/// </summary>
	void PlayEffect();

	/// <summary>
	/// �_�[�N�X�s�A�̃T�C�Y��ύX����B�傫���Ȃ��ď������Ȃ�B
	/// </summary>
	void ChangeSizeDarkSpearObj();

private:

	CollisionObject* m_collision = nullptr;

	EffectEmitter* m_rangeEffect = nullptr;

	EffectEmitter* m_darkSpearEffect = nullptr;

	ModelRender m_darkSpearModel;

	Vector3 m_position = g_vec3Zero;

	Vector3 m_scale = g_vec3One;


	float m_deleteTimer = 0.0f;

	float m_maxScale = 1.0f;

	float m_oldMulSize = 0.0f;

	bool m_stopRangeEffectFlag = false;

	
};

