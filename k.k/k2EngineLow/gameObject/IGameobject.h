/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#pragma once


#include "IComponent.h"
#include <list>
#include <string>



namespace nsK2EngineLow {
	class RenderContext;

	/*!
		*@brief	ゲームオブジェクト。
		*/
	class IGameObject : public Noncopyable {
	public:
		/*!
			*@brief	デストラクタ
			*/
		virtual ~IGameObject()
		{
			//コンポーネントリストを削除
			for (IComponent* component : m_components) {
				delete component;
			}
		}
	public:


		/*!
		*@brief	Updateの直前で呼ばれる開始処理。
		*@details
		*
		* 本関数がtrueを返すとゲームオブジェクトの準備が完了したと判断されて</br>
		* Update関数が呼ばれ出します。trueを返して以降はStart関数は呼ばれなくなります。</br>
		* ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップなどを使って</br>
		* 適切に初期化を行ってください。
		*/
		virtual bool Start() { return true; }
		/*!
			*@brief	更新
			*/
		virtual void Update() {}
		/*!
		 *@brief	描画
		*/
		virtual void Render(RenderContext& renderContext)
		{
			(void)renderContext;
		}

	public:


		/*!
		*@brief モデルの読み込み。
		モデルの読み込みは時間がかかるので先に読み込んでおきたいときに呼ぶ
		*/
		virtual void InitModel(){}

		/*!
		*@brief Start関数が完了した？
		*/
		bool IsStart() const
		{
			return m_isStart;
		}
		/*!
		*@brief アクティブかどうかを判定。
		*/
		bool IsActive() const
		{
			return m_isActive;
		}
		/// <summary>
		/// ゲームオブジェクトをアクティブにする。
		/// </summary>
		void Activate()
		{
			m_isActive = true;
		}
		/// <summary>
		/// ゲームオブジェクトを非アクティブにする。
		/// </summary>
		void Deactivate()
		{
			m_isActive = false;
		}

		/// <summary>
		/// スタートフラグをリセットする
		/// </summary>
		void ResetStartFlag()
		{
			m_isStart = false;
		}

		/// <summary>
		/// 死亡させる。
		/// </summary>
		void Dead()
		{
			m_isDead = true;
		}
		/// <summary>
		/// 死亡している？
		/// </summary>
		/// <returns>trueが返ってきたら死亡している</returns>
		bool IsDead() const
		{
			return m_isDead;
		}
		/// <summary>
		/// ゲームオブジェクトの名前を設定。
		/// </summary>
		/// <param name="name">名前</param>
		void SetName(const char* name)
		{
			if (name != nullptr) {
				m_name = name;
			}
		}
		/// <summary>
		/// ゲームオブジェクトの名前を返す。
		/// </summary>
		/// <returns>名前</returns>
		const char* GetName() {
			return m_name.c_str();
		}
		/// <summary>
		/// 引数で渡された名前が、このゲームオブジェクトの名前とマッチするか判定。
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
		/// コンポーネントを追加
		/// </summary>
		/// <typeparam name="T">追加するコンポーネント</typeparam>
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
		/// コンポーネントを探す
		/// </summary>
		/// <typeparam name="T">ゲットしたいコンポーネント</typeparam>
		/// <returns>あればコンポーネントを返す。なければnullptrを返す</returns>
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
					//スタート関数をとおっていたら

					component->UpdateComponent();
				}
			}
		}
		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead) {
				if (Start()) {
					//初期化処理完了。
					m_isStart = true;
				}
			}
		}

		

		friend class CGameObjectManager;
	protected:
		std::string m_name = "default";					//ゲームオブジェクトの名前
		bool m_isStart = false;							//Startの開始フラグ。
		bool m_isDead = false;							//死亡フラグ。
		bool m_isRegistDeadList = false;				//死亡リストに積まれている。
		bool m_isNewFromGameObjectManager;	//GameObjectManagerでnewされた。
		bool m_isRegist = false;							//GameObjectManagerに登録されている？
		bool m_isActive = true;							//Activeフラグ。

		std::vector<IComponent*> m_components;			//コンポーネントリスト

	};
}