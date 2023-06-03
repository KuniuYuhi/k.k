#pragma once

namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		
		/// <summary>
		/// �ʏ�̃��f���̏�����
		/// </summary>
		/// <param name="tkmFilepath"></param>
		void Init(const char* tkmFilepath);

		/// <summary>
		/// �`�揈��
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc)
		{
			g_renderingEngine->AddModelList(this);
		}

		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

	private:
		Model								m_model;					//���f���N���X
		ModelInitData						m_modelInitData;					//���f�������������邽�߂̏���ݒ肷��N���X
	};
}

