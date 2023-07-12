#pragma once
struct Status
{
	int maxHp = 0;
	int hp = 0;
	int maxMp = 0;
	float mp = 0;
	int defaultAtk = 0;
	int atk = 0;
	float dashSpeed = 0.0f;
	float defaultSpeed = 0.0f;
	const char* characterName = nullptr;


	void InitStatus(int MaxHp,int MaxMp,int Atk, float Speed,const char* name)
	{
		maxHp = MaxHp;
		hp = maxHp;
		maxMp = MaxMp;
		mp = maxMp;
		defaultAtk = Atk;
		atk = defaultAtk;
		defaultSpeed = Speed;
		dashSpeed = defaultSpeed;
		characterName = name;
	}

	//csv‚ÍŽg‚í‚È‚¢
	std::string name;          //–¼‘O

static std::unordered_map<std::string, Status> StatusDataMAP;
static const std::string csvFilePath;
static bool IsLoadedCSVFile;

void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);
};