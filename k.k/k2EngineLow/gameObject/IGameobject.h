/*!
 *@brief	�Q�[���I�u�W�F�N�g�̊��N���X�B
 */

#pragma once


#include "IComponent.h"
#include <list>
#include <string>



namespace nsK2EngineLow {
	class RenderContext;

	/*!
		*@brief	�Q�[���I�u�W�F�N�g�B
		*/
	class IGameObject : public Noncopyable {
	public:
		/*!
			*@brief	�f�X�g���N�^
			*/
		virtual ~IGameObject()
		{
			//�R���|�[�l���g���X�g���폜
			for (IComponent* component : m_components) {
				delete component;
			}
		}
	public:


		/*!
		*@brief	Update�̒��O�ŌĂ΂��J�n�����B
		*@details
		*
		* �{�֐���true��Ԃ��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f�����</br>
		* Update�֐����Ă΂�o���܂��Btrue��Ԃ��Ĉȍ~��Start�֐��͌Ă΂�Ȃ��Ȃ�܂��B</br>
		* �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v�Ȃǂ��g����</br>
		* �K�؂ɏ��������s���Ă��������B
		*/
		virtual bool Start() { return true; }
		/*!
			*@brief	�X�V
			*/
		virtual void Update() {}
		/*!
		 *@brief	�`��
		*/
		virtual void Render(RenderContext& renderContext)
		{
			(void)renderContext;
		}

	public:


		/*!
		*@brief ���f���̓ǂݍ��݁B
		���f���̓ǂݍ��݂͎��Ԃ�������̂Ő�ɓǂݍ���ł��������Ƃ��ɌĂ�
		*/
		virtual void InitModel(){}

		/*!
		*@brief Start�֐������������H
		*/
		bool IsStart() const
		{
			return m_isStart;
		}
		/*!
		*@brief �A�N�e�B�u���ǂ����𔻒�B
		*/
		bool IsActive() const
		{
			return m_isActive;
		}
		/// <summary>
		/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���B
		/// </summary>
		void Deactivate()
		{
			m_isActive = false;
		}

		/// <summary>
		/// �X�^�[�g�t���O�����Z�b�g����
		/// </summary>
		void ResetStartFlag()
		{
			m_isStart = false;
		}

		/// <summary>
		/// ���S������B
		/// </summary>
		void Dead()
		{
			m_isDead = true;
		}
		/// <summary>
		/// ���S���Ă���H
		/// </summary>
		/// <returns>true���Ԃ��Ă����玀�S���Ă���</returns>
		bool IsDead() const
		{
			return m_isDead;
		}
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O��ݒ�B
		/// </summary>
		/// <param name="name">���O</param>
		void SetName(const char* name)
		{
			if (name != nullptr) {
				m_name = name;
			}
		}
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O��Ԃ��B
		/// </summary>
		/// <returns>���O</returns>
		const char* GetName() {
			return m_name.c_str();
		}
		/// <summary>
		/// �����œn���ꂽ���O���A���̃Q�[���I�u�W�F�N�g�̖��O�ƃ}�b�`���邩����B
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		bool IsMatchName(const char* name) const
		{
			if (strcmp(m_name.c_str(), name) == 0) {
				return true;
			}
			return false;
		}


		/// <summary>
		/// �R���|�[�l���g��ǉ�
		/// </summary>
		/// <typeparam name="T">�ǉ�����R���|�[�l���g</typeparam>
		template<typename T>
		void AddComponent() 
		{
			m_components.emplace_back(new T());
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="deleteComponent"></param>
		void DeleteComponent(IComponent* deleteComponent)
		{
			m_components.erase(
				std::remove(
					m_components.begin(), 
					m_components.end(), 
					deleteComponent), 
				m_components.end()
			);
		}

		/// <summary>
		/// �R���|�[�l���g��T��
		/// </summary>
		/// <typeparam name="T">�Q�b�g�������R���|�[�l���g</typeparam>
		/// <returns>����΃R���|�[�l���g��Ԃ��B�Ȃ����nullptr��Ԃ�</returns>
		template<typename T>
		T* GetComponent()
		{
			for (IComponent* component : m_components)
			{
				T* target = dynamic_cast<T*>(component);
				if (target != nullptr)
				{
					return target;
				}
			}
			return nullptr;
		}

	public:

		void RenderWrapper(RenderContext& renderContext)
		{
			if (m_isActive && m_isStart && !m_isDead) {
				Render(renderContext);
			}
		}
		void UpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead) {
				Update();
			}
		}
		void UpdateComponentsWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead) {

				for (IComponent* component : m_components)
				{
					//�X�^�[�g�֐����Ƃ����Ă�����

					component->UpdateComponent();
				}
			}
		}
		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead) {
				if (Start()) {
					//���������������B
					m_isStart = true;
				}
			}
		}

		

		friend class CGameObjectManager;
	protected:
		std::string m_name = "default";					//�Q�[���I�u�W�F�N�g�̖��O
		bool m_isStart = false;							//Start�̊J�n�t���O�B
		bool m_isDead = false;							//���S�t���O�B
		bool m_isRegistDeadList = false;				//���S���X�g�ɐς܂�Ă���B
		bool m_isNewFromGameObjectManager;	//GameObjectManager��new���ꂽ�B
		bool m_isRegist = false;							//GameObjectManager�ɓo�^����Ă���H
		bool m_isActive = true;							//Active�t���O�B

		std::vector<IComponent*> m_components;			//�R���|�[�l���g���X�g

	};
}