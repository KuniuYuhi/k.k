#pragma once


/// <summary>
/// オブジェクトプールの基底クラス
/// </summary>
/// 
/// 
/// これをエネミーのオブジェクトプールクラスとして作ればよくね？

//いくつか生成しておいて非アクティブ化しておく


class IObjectPool
{
public:

	virtual ~IObjectPool() = default;

	///// <summary>
	///// オブジェクトプールリストにペア型の変数を挿入
	///// </summary>
	///// <param name="insertPair"></param>
	//void Insert(std::pair<std::string, T*> insertPair)
	//{
	//	m_objectMap.insert(insertPair);
	//}


	/// <summary>
	/// オブジェクトを生成してキューに格納。オブジェクトは非アクティブ化されます。
	/// </summary>
	/// <typeparam name="T">オブジェクトのクラス</typeparam>
	/// <param name="saveKeyName">オブジェクトを保存するペアのキーの名前</param>
	template<typename T>
	void OnCreate(const char* saveKeyName)
	{
		//オブジェクトを生成
		auto* createobj = NewGO<T>(0, saveKeyName);
		
		//IGameObjectを継承しているなら取得
		IGameObject* targetObj = dynamic_cast<IGameObject*>(createobj);
		////IGameObjectが継承されていないならエラー
		if (targetObj == nullptr) std::abort();

		//非アクティブ化する
		targetObj->Deactivate();


		//オブジェクトプールの中に引数のオブジェクトの名前があるか調べる
		auto it = m_objectPoolQueue.find(saveKeyName);
		if (it != m_objectPoolQueue.end()) 
		{
			//キューにプッシュ
			it->second.push(createobj);
		}
	}


	/// <summary>
	/// キューからオブジェクトを取得する
	/// </summary>
	/// <param name="getKeyName">取得するオブジェクトが格納されているキー</param>
	/// <returns></returns>
	IGameObject* OnGet(const char* getKeyName)
	{
		std::queue<IGameObject*>& getQueue = m_objectPoolQueue.at(getKeyName);

		if (getQueue.empty())
		{
			OnCreate<IGameObject>(getKeyName);
		}

	}


protected:

	int m_initPoolSize = 10;

	std::map<const char*, std::queue<IGameObject*>> m_objectPoolQueue;	//オブジェクトプールのマップ

	

};

