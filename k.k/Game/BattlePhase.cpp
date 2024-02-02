#include "stdafx.h"
#include "BattlePhase.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "Boss.h"
#include "Player.h"

#include "InitEffect.h"

//�������郂���X�^�[�̃w�b�_�[�t�@�C��
#include "Slime.h"
#include "TurtleShell.h"
#include "Mushroom.h"
#include "Cactus.h"


namespace {
	//const int SUMMON_MONSTERS_CONUT = 10;

	const int SUMMMON_RADIUS = 900.0;

	const float SUMMON_POS_DISTANCE = 320.0f;

	const float SUMMON_POS_DISTANCE_FOR_BOSS = 200.0f;
	const float SUMMON_POS_DISTANCE_FOR_PLAYER = 100.0f;

	const float INCANTATION_TIME_LIMMIT = 2.0f;

	const float CIRCLE_EFFECT_SIZE = 15.0F;

}

BattlePhase::BattlePhase()
{
}

BattlePhase::~BattlePhase()
{
	//todo �G�t�F�N�g�̍폜
}

bool BattlePhase::Start()
{
	//�������������B
	srand((unsigned)time(NULL));

	for (int num = 0; num < SUMMON_MONSTERS_CONUT; num++)
	{
		model[num].Init("Assets/modelData/character/Slime/slime.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS");
		model[num].Update();
	}

	return true;
}

void BattlePhase::Update()
{
	//���s�����܂����珈�����~�߂�
	if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		return;
	}

	//�t�F�[�Y���ς�����Ƃ��Ƀ��u�����X�^�[�𐶐�
	//�����X�^�[�𐶐��X�e�[�g�Ȃ�
	if (GameManager::GetInstance()->GetDAndCMonstersState() == 
		GameManager::enMonsters_Create)
	{
		//�����X�^�[����������
		ProcessSummonMonsters();
	}

	//�����X�^�[���폜�X�e�[�g�Ȃ�
	if (GameManager::GetInstance()->GetDAndCMonstersState() ==
		GameManager::enMonsters_Delete)
	{
		//���݂̃t�F�[�Y�̃��u�����X�^�[���폜
		DeleteMobMonsters();
		//�폜�����̂ŃQ�[���}�l�[�W���[�̃X�e�[�g���Ȃ��ɂ���
		GameManager::GetInstance()->
			SetDAndCMonstersState(GameManager::enMonsters_None);
	}
}

void BattlePhase::ProcessSummonMonsters()
{
	switch (m_enSummonMonsterStep)
	{
	case BattlePhase::enSummonMonsterStep_SettingMagicCircle:
		//����������W�̐ݒ�
		SetSummonRandomPosition(SUMMMON_RADIUS);
		//�����w�̐ݒu
		CreateMagicCircleEffect(SUMMON_MONSTERS_CONUT);
		//���̃X�e�b�v�ɐi��
		m_enSummonMonsterStep = enSummonMonsterStep_SummonMonsters;
		break;
	case BattlePhase::enSummonMonsterStep_SummonMonsters:
		//��莞�ԑ҂�
		if (IsIncantation() == true)
		{
			//�����X�^�[�𐶐�
			CreateMobMonsters(SUMMON_MONSTERS_CONUT);
			//���������̂ŃQ�[���}�l�[�W���[�̃X�e�[�g���Ȃ��ɂ���
			GameManager::GetInstance()->
				SetDAndCMonstersState(GameManager::enMonsters_None);
			//�X�e�b�v�����Z�b�g
			m_enSummonMonsterStep = enSummonMonsterStep_SettingMagicCircle;
		}
		break;
	default:
		break;
	}

	


}

void BattlePhase::CreateMobMonsters(const int createCount)
{
	//�t�F�[�Y�ɂ���ēG�̎�ނ�ς���
	AdjustDifficultyByPhase(createCount);

}

void BattlePhase::DeleteMobMonsters()
{
	//��������O�ɗv�f��S�č폜
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		monster->ProcessDead(false);
		DeleteGO(monster);
	}
	//���X�g���N���A
	CharactersInfoManager::GetInstance()->GetMobMonsters().clear();
}

void BattlePhase::SetSummonRandomPosition(const int radius)
{
	//���S
	Vector3 Center = g_vec3Zero;
	//����������W
	for (int num = 0; num < SUMMON_MONSTERS_CONUT; num++)
	{
		m_createPos[num] = g_vec3Zero;
	}

	//X���W��Z���W
	float X;
	float Z;
	//�z��̃J�E���g
	int count = 0;
	//�����_���Ȓl�̍ő�l�̌v�Z
	int max = radius * 2 + 1;
	//����������W�����܂�܂Ń��[�v
	while (count< SUMMON_MONSTERS_CONUT)
	{
		//�~�͈͓̔��Ń����_���Ȓl��ݒ�
		X = rand() % max - radius;
		Z = rand() % max - radius;
		//����������W
		m_createPos[count] = { X,0.0f,Z };
		//����������W�����a��菬�����Ȃ�A���W������
		if (m_createPos[count].Length() < radius)
		{
			//���̏���������W�ƈ��̋������󂢂Ă��邩����
			if (IsWithInDistances(count, SUMMON_POS_DISTANCE) == true)
			{
				//���̔z��ɐi�߂�
				count++;
			}
		}
	}
}

bool BattlePhase::IsWithInDistances(int count, float distance)
{
	//�{�X�Ƃ̊Ԋu�𒲂ׂ�
	Vector3 diff = m_createPos[count] -
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition();
	//�{�X�Ƃ̊Ԋu�����������珈�����~�߂�(���W�ݒ��蒼��)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_BOSS)
	{
		return false;
	}
	//�v���C���[�Ƃ̊Ԋu�𒲂ׂ�
	diff = m_createPos[count] -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//�v���C���[�Ƃ̊Ԋu�����������珈�����~�߂�(���W�ݒ��蒼��)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_PLAYER)
	{
		return false;
	}

	int distanceCount = 0;
	//���̐���������W�ƈ��̊Ԋu���J����
	for (int i = 0; i < count; i++)
	{
		//����������W���m�̋������v�Z
		float length = (m_createPos[count] - m_createPos[i]).Length();
		//����������Ă�����A����������W�Ɍ���
		if (length > distance)
		{
			distanceCount++;
		}
	}
	//�S�Ă̍��W�Ƌ������󂢂Ă�����
	if (distanceCount == count)
	{
		//
		return true;
	}
	else
	{
		return false;
	}
	
}

void BattlePhase::AdjustDifficultyByPhase(const int summonMonstersCount)
{
	//todo ���^Ai�ł���Ă݂���
	switch (GameManager::GetInstance()->GetNowPhaseState())
	{
		//��Փx�F�ȒP
	case GameManager::EnPhaseState_Phase1:
		processEasyDifficulty(summonMonstersCount);
		break;
		//��Փx�F����
	case GameManager::EnPhaseState_Phase2:
		processNormalDifficulty(summonMonstersCount);
		break;
		//��Փx�F���
	case GameManager::EnPhaseState_Phase3:
		processHardDifficulty(summonMonstersCount);
		break;
	default:
		break;
	}



}

void BattlePhase::processEasyDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		Slime* slime = NewGO<Slime>(0, "slime");
		slime->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(slime);
		//�������̌��̃G�t�F�N�g�̍Đ�
		CreateSummonLightEffect(m_createPos[num]);
	}
}

void BattlePhase::processNormalDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		Cactus* cactus = NewGO<Cactus>(0, "cactus");
		cactus->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(cactus);
		//�������̌��̃G�t�F�N�g�̍Đ�
		CreateSummonLightEffect(m_createPos[num]);
	}
}

void BattlePhase::processHardDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		//Cactus* cactus = NewGO<Cactus>(0, "cactus");
		//cactus->SetPosition(m_createPos[num]);
		//CharactersInfoManager::GetInstance()->AddMobMonsterFromList(cactus);
		////�������̌��̃G�t�F�N�g�̍Đ�
		//CreateSummonLightEffect(m_createPos[num]);

		Mushroom* mushroom = NewGO<Mushroom>(0, "mushroom");
		mushroom->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(mushroom);
		//�������̌��̃G�t�F�N�g�̍Đ�
		CreateSummonLightEffect(m_createPos[num]);
	}
}

bool BattlePhase::IsIncantation()
{
	if (m_incantationTimer > INCANTATION_TIME_LIMMIT)
	{
		m_incantationTimer = 0.0f;
		return true;
	}
	else
	{
		m_incantationTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
	
}

void BattlePhase::CreateMagicCircleEffect(const int monsterCount)
{
	//�����X�^�[�𐶐�������W�ɖ��@�w�G�t�F�N�g���Đ�
	for (int num = 0; num < monsterCount; num++)
	{
		Vector3 Pos = m_createPos[num];
		Pos.y += 5.0f;
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Circle);
		effectEmitter->Play();
		effectEmitter->SetPosition(Pos);
		effectEmitter->SetScale(g_vec3One * CIRCLE_EFFECT_SIZE);
		effectEmitter->Update();
	}
}

void BattlePhase::CreateSummonLightEffect(Vector3 createPos)
{
	//�������ꂽ�Ƃ��̌��̃G�t�F�N�g�̍Đ�
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Right);
	effectEmitter->Play();
	effectEmitter->SetPosition(createPos);
	effectEmitter->SetScale(g_vec3One * CIRCLE_EFFECT_SIZE);
	effectEmitter->Update();
}


