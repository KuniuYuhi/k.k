#pragma once
namespace nsK2EngineLow {

	class IComponent {
	public:

		virtual ~IComponent() = default;

		/// <summary>
		/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
		/// </summary>
		virtual void UpdateComponent() {}

	};


}

