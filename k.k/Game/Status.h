#pragma once
struct Status
{
	int maxHp = 0;
	int hp = 0;
	int defaultAtk = 0;
	int atk = 0;
	float speed = 0.0f;
	float defaultSpeed = 0.0f;


	void InitStatus(int MaxHp,int Atk, float Speed)
	{
		maxHp = MaxHp;
		hp = maxHp;
		defaultAtk = Atk;
		atk = defaultAtk;
		defaultSpeed = Speed;
		speed = defaultSpeed;
	}

	//csvは使わない
	std::string name;          //名前

static std::unordered_map<std::string, Status> StatusDataMAP;
static const std::string csvFilePath;
static bool IsLoadedCSVFile;

void Init(const std::string& name);

private:
	void LoadCSV(const std::string& name);
};