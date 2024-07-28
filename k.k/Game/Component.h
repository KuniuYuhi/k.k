#pragma once

namespace nsK2EngineLow {

	class Component {
	public:

		virtual ~Component() = default;

		/// <summary>
		/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
		/// </summary>
		virtual void UpdateComponent() {}

	};


}

