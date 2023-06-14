#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow {

	void Bloom::Init(RenderTarget& mainRenderTarget)
	{
		InitRenderTarget(mainRenderTarget);
		InitLuminanceSprite(mainRenderTarget);
		InitGaussianBlur();
		InitFinalSprite(mainRenderTarget);
	}

	void Bloom::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
	{
		//輝度抽出用のレンダリングターゲットに変更
		rc.WaitUntilToPossibleSetRenderTarget(m_luminanceRenderTarget);
		rc.SetRenderTargetAndViewport(m_luminanceRenderTarget);
		rc.ClearRenderTargetView(m_luminanceRenderTarget);

		//輝度抽出を行う
		m_luminanceSprite.Draw(rc);

		//レンダリングエンジンへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceRenderTarget);

		//ガウシアンブラーを実行する
		m_gaussianBlur[0].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[1].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[2].ExecuteOnGPU(rc, 10);
		m_gaussianBlur[3].ExecuteOnGPU(rc, 10);

		//ボケ画像をメインレンダリングターゲットに加算合成
		//レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
		//レンダリングターゲットを設定する
		rc.SetRenderTargetAndViewport(mainRenderTarget);

		//最終合成
		m_finalSprite.Draw(rc);

		//レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	}

	void Bloom::InitRenderTarget(RenderTarget& mainRenderTarget)
	{
		//輝度抽出用のレンダリングターゲット
		m_luminanceRenderTarget.Create(
			mainRenderTarget.GetWidth(),	//メインレンダリングターゲットと同じ
			mainRenderTarget.GetHeight(),	//メインレンダリングターゲットと同じ
			1,
			1,
			mainRenderTarget.GetColorBufferFormat(),	//メインレンダリングターゲットと同じ
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void Bloom::InitLuminanceSprite(RenderTarget& mainRenderTarget)
	{
		//初期化情報作成
		SpriteInitData luminanceSpriteInitData;
		//輝度摘出用のシェーダーのファイルを指定する
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/postEffect.fx";
		//頂点シェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//ピクセルシェーダーのエントリーポイントを指定する
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//スプライトの幅と高さはluminanceRenderTargetと同じ
		luminanceSpriteInitData.m_width = m_luminanceRenderTarget.GetWidth();
		luminanceSpriteInitData.m_height = m_luminanceRenderTarget.GetHeight();

		//テクスチャはメインレンダーターゲットのカラーバッファー
		luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
		//書き込むレンダリングターゲットのフォーマットを指定する
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		//作成した初期化情報を元にスプライトを初期化する
		m_luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitFinalSprite(RenderTarget& mainRenderTarget)
	{
		//初期化情報を設定する
		SpriteInitData finalSpriteInitData;
		finalSpriteInitData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();

		//mainRenderTargetとおなじ幅と高さ
		finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
		finalSpriteInitData.m_height = mainRenderTarget.GetHeight();

		//ボケ画像を合成するので専用シェーダーを使用する
		finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/postEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//ただし、加算合成で描画するので、アルファブレンディングモードを加算する
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//カラーバッファーのフォーマットは例によって、32ビット浮動小数点バッファー
		finalSpriteInitData.m_colorBufferFormat[0]= DXGI_FORMAT_R32G32B32A32_FLOAT;

		//スプライトを初期化
		m_finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::InitGaussianBlur()
	{
		//[0]は輝度テクスチャにガウシアンブラーをかける
		m_gaussianBlur[0].Init(&m_luminanceRenderTarget.GetRenderTargetTexture());
		//[1]は[0]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
		//[2]は[1]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
		//[3]は[2]のテクスチャにガウシアンブラーをかける
		m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());
	}


}
