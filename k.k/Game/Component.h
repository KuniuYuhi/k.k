#pragma once

namespace nsK2EngineLow {

	class Component {
	public:

		virtual ~Component() = default;

		/// <summary>
		/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
		/// </summary>
		virtual void UpdateComponent() {}

	};


}

