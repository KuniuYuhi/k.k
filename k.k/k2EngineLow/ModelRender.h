#pragma once

namespace nsK2EngineLow {
	class ModelRender
	{
	public:
		
		/// <summary>
		/// 通常のモデルの初期化
		/// </summary>
		/// <param name="tkmFilepath"></param>
		void Init(const char* tkmFilepath);

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc)
		{
			g_renderingEngine->AddModelList(this);
		}

		/// <summary>
		/// モデルを描画する
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

	private:
		Model								m_model;					//モデルクラス
		ModelInitData						m_modelInitData;					//モデルを初期化するための情報を設定するクラス
	};
}

