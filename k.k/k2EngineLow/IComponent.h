#pragma once
namespace nsK2EngineLow {

	class IComponent {
	public:

		virtual ~IComponent() = default;

		/// <summary>
		/// コンポーネントを追加した時の最初の処理
		/// </summary>
		virtual void Start(){}

		/// <summary>
		/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
		/// </summary>
		virtual void UpdateComponent() {}

	};


}

