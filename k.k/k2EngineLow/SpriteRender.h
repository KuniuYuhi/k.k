#pragma once

namespace nsK2EngineLow {

	struct SpriteCB
	{
		Vector2 RoundWipeStartPosition = Vector2(0.0f, 0.0f);		//円形ワイプを行う始点
		bool grayScale = false;					//グレースケールをするかどうか
		float pad1;
		Vector2 wipeDirection = Vector2(0.0f, 0.0f);		//ワイプする方向
		bool simpleWipe = false;				//単純なリニアワイプをするかどうか
		float wipeSize = 0;						//ワイプサイズ
		bool RoundWipe = false;					//円形ワイプを使用するかどうか
		bool WipeWithDirection = false;			//方向を指定するリニアワイプをするかどうか
	};

	class SpriteRender
	{
	public:

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="width">画像の横幅</param>
		/// <param name="height">画像の縦幅</param>
		/// <param name="alphaBlendMode">デフォルトは半透明合成</param>
		void Init(
			const char* filePath,
			const float width,
			const float height,
			AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans
		);

		/// <summary>
		/// 座標の設定(zは0.0fにする)
		/// </summary>
		/// <param name="position">座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/// <summary>
		/// 座標の設定(zは0.0fにする)
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetPosition(float x, float y, float z = 0.0f)
		{
			SetPosition({ x,y,z });
		}

		/// <summary>
		/// 座標を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/// <summary>
		/// 大きさを設定する(Zは1.0fにする)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさを設定する(zは1.0fにする)
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetScale(float x, float y, float z = 1.0f)
		{
			SetScale({ x, y, z });
		}

		/// <summary>
		/// 大きさを取得する
		/// </summary>
		/// <returns>大きさ</returns>
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 回転を取得する
		/// </summary>
		/// <returns>回転</returns>
		const Quaternion& GetRotation() const
		{
			return m_rotation;
		}

		/// <summary>
		/// ピボットを設定する
		/// </summary>
		/// <param name="pivot"></param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// ピボットを取得する
		/// </summary>
		/// <returns>ピボット</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}

		/// <summary>
		/// 乗算カラーを設定する
		/// </summary>
		/// <param name="mulColor">乗算カラー</param>
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}
		/// <summary>
		/// 乗算カラーを取得する
		/// </summary>
		/// <returns>乗算カラー</returns>
		const Vector4& GetMulColor() const
		{
			return m_sprite.GetMulColor();
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update()
		{
			m_sprite.Update(
				m_position,
				m_rotation,
				m_scale,
				m_pivot
			);
		}

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// 画像を描画する(RenderingEngineで描画するときに呼び出す)
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void OnRenderSprite(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

		bool IsInit()
		{
			if (m_isInit)	return true;
			else			return false;
		}

		/// <summary>
		/// グレースケールを設定する
		/// </summary>
		/// <param name="flag">trueの場合グレースケールを行う</param>
		void SetGrayScale(bool flag)
		{
			m_spriteCB.grayScale = flag;
		}
		/// <summary>
		/// グレースケール中か判定
		/// </summary>
		/// <returns>trueの場合グレースケール中</returns>
		bool IsGrayScale()
		{
			return m_spriteCB.grayScale;
		}

		/// <summary>
		/// 単純なリニアワイプを設定する
		/// </summary>
		/// <param name="flag"></param>
		void SetSimpleWipe(bool flag)
		{
			m_spriteCB.simpleWipe = flag;
		}

		/// <summary>
		/// 単純なリニアワイプしているか判定
		/// </summary>
		/// <returns></returns>
		bool GetSimpleWipe()
		{
			return m_spriteCB.simpleWipe;
		}
		
		/// <summary>
		/// ワイプサイズを指定する
		/// </summary>
		/// <param name="wipesize"></param>
		void SetWipeSize(float wipesize)
		{
			m_spriteCB.wipeSize = wipesize;
		}

		/// <summary>
		/// ワイプサイズを取得する
		/// </summary>
		/// <returns></returns>
		float GetWipeSize()
		{
			return m_spriteCB.wipeSize;
		}

		/// <summary>
		/// ワイプ方向を指定して正規化する
		/// </summary>
		void SetWipeDirection(Vector2 wipedir)
		{
			m_spriteCB.wipeDirection = wipedir;
			m_spriteCB.wipeDirection.Normalize();

		}

		/// <summary>
		/// ワイプ方向を指定する
		/// </summary>
		void SetDirection(float x, float y)
		{
			SetWipeDirection(Vector2(x, y));
		}

		/// <summary>
		/// ワイプ方向を取得
		/// </summary>
		/// <returns></returns>
		Vector2 GetWipeDir()
		{
			return m_spriteCB.wipeDirection;
		}

		/// <summary>
		/// 方向を指定するリニアワイプを設定する
		/// </summary>
		/// <param name="flag"></param>
		void SetWipeWithDirection(bool flag)
		{
			m_spriteCB.WipeWithDirection = flag;
		}

		/// <summary>
		/// 方向を指定するリニアワイプを取得する
		/// </summary>
		/// <returns></returns>
		bool GetWipeWithDirection()
		{
			return m_spriteCB.WipeWithDirection;
		}

		/// <summary>
		/// 円形ワイプを行う始点の座標を指定する
		/// </summary>
		/// <param name="startPosition"></param>
		void SetRoundWipeStartPosition(Vector2 startPosition)
		{
			m_spriteCB.RoundWipeStartPosition = startPosition;
		}

		/// <summary>
		/// 円形ワイプを行う始点の座標を指定する
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		void SetRoundWipeStartPosition(float x, float y)
		{
			SetRoundWipeStartPosition(Vector2(x, y));
		}

		/// <summary>
		/// 円形ワイプを設定する
		/// </summary>
		/// <param name="flag"></param>
		void SetRoundWipe(bool flag)
		{
			m_spriteCB.RoundWipe = flag;
		}

		/// <summary>
		/// 円形ワイプを取得する
		/// </summary>
		/// <returns></returns>
		bool GetRoundWipe()
		{
			return m_spriteCB.RoundWipe;
		}

	private:
		SpriteCB	m_spriteCB;
		Sprite		m_sprite;								//Spriteクラス

		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;		//ピボット
		Vector3		m_position = Vector3::Zero;				//座標
		Vector3		m_scale = Vector3::One;					//大きさ
		Quaternion	m_rotation = Quaternion::Identity;	//回転
		bool		m_isInit = false;
	};
}
