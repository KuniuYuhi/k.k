#pragma once
namespace nsK2EngineLow {

	class IComponent {
	public:

		virtual ~IComponent() = default;

		/// <summary>
		/// �R���|�[�l���g��ǉ��������̍ŏ��̏���
		/// </summary>
		virtual void Start(){}

		/// <summary>
		/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
		/// </summary>
		virtual void UpdateComponent() {}

	};


}

