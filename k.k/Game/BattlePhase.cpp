#include "stdafx.h"
#include "BattlePhase.h"

#include "MobMonstersList.h"

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

#include "MobMonster.h"

using namespace MobMonstersList;

namespace {
	const int DEFAULT_SUMMON_MOB_MONSTERS_CONUT = 7;
	const int MAX_SUMMON_MOB_MONSTERS_CONUT = 15;

	const int ADD_SUMMON_MONSTER_COUNT = 2;
	const int SUB_SUMMON_MONSTER_COUNT = 5;

	const int SUMMMON_RADIUS = 900.0;

	const float SUMMON_POS_DISTANCE = 320.0f;

	const float SUMMON_POS_DISTANCE_FOR_BOSS = 200.0f;
	const float SUMMON_POS_DISTANCE_FOR_PLAYER = 100.0f;

	const float INCANTATION_TIME_LIMMIT = 2.0f;

	const float CIRCLE_EFFECT_SIZE = 15.0f;

	const int NONE_PRIORITY = -1;

}

BattlePhase::BattlePhase()
{
}

BattlePhase::~BattlePhase()
{
}

bool BattlePhase::Start()
{
	//�������������B
	srand((unsigned)time(NULL));

	//�ŏ��ɏ����������郂���X�^�[�̐���ݒ�
	m_mobMonsterSpawnCount = DEFAULT_SUMMON_MOB_MONSTERS_CONUT;

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
		//�������郂���X�^�[�̐������߂�
		 SetMobMonsterSpawnCount();
		//����������W�̐ݒ�
		SetSummonRandomPosition(SUMMMON_RADIUS);
		//�����w�̐ݒu
		CreateMagicCircleEffect(m_mobMonsterSpawnCount);
		//���̃X�e�b�v�ɐi��
		m_enSummonMonsterStep = enSummonMonsterStep_SummonMonsters;
		break;
	case BattlePhase::enSummonMonsterStep_SummonMonsters:
		//��莞�ԑ҂�
		if (IsIncantation() == true)
		{
			//�����X�^�[�𐶐�
			CreateMobMonsters();
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

void BattlePhase::SetMobMonsterSpawnCount()
{
	//���݂̃t�F�[�Y��1�����ȏ�t�F�[�Y�����[�v���Ă���Ȃ�
	//�������郂���X�^�[�̐����������炷
	if (GameManager::GetInstance()->GetPhaseLoopCount() != 0 &&
		GameManager::GetInstance()->GetNowPhaseState() ==
		GameManager::EnPhaseState_Phase1)
	{
		m_mobMonsterSpawnCount -= SUB_SUMMON_MONSTER_COUNT;
	}

	//�����X�^�[��|�������ő��₵�Ă��悢����
	m_mobMonsterSpawnCount += ADD_SUMMON_MONSTER_COUNT;


	//�����X�^�[�̐��������������Ȃ��悤�ɂ���
	if (m_mobMonsterSpawnCount > MAX_SUMMON_MOB_MONSTERS_CONUT)
	{
		m_mobMonsterSpawnCount = MAX_SUMMON_MOB_MONSTERS_CONUT;
	}

}

void BattlePhase::CreateMobMonsters()
{
	//�t�F�[�Y�ɂ���ēG�̎�ނ�ς���
	AdjustDifficultyByPhase(m_mobMonsterSpawnCount);

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
	//�O��̏����Ŋi�[�������W���X�g���N���A
	m_createPositions.clear();
	//���S
	Vector3 Center = g_vec3Zero;
	//�������郂���X�^�[�̐��J��Ԃ�
	for (int num = 0; num < m_mobMonsterSpawnCount; num++)
	{
		//���W�����X�g�Ɋi�[
		m_createPositions.emplace_back(g_vec3Zero);
	}

	//X���W��Z���W
	float X;
	float Z;
	//�z��̃J�E���g
	int count = 0;
	//�����_���Ȓl�̍ő�l�̌v�Z
	int max = radius * 2 + 1;
	//����������W�����܂�܂Ń��[�v
	while (count< m_mobMonsterSpawnCount)
	{
		//�~�͈͓̔��Ń����_���Ȓl��ݒ�
		X = rand() % max - radius;
		Z = rand() % max - radius;
		//����������W
		m_createPositions[count] = { X,0.0f,Z };
		//����������W�����a��菬�����Ȃ�A���W������
		if (m_createPositions[count].Length() < radius)
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
	Vector3 diff = m_createPositions[count] -
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition();
	//�{�X�Ƃ̊Ԋu�����������珈�����~�߂�(���W�ݒ��蒼��)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_BOSS)
	{
		return false;
	}
	//�v���C���[�Ƃ̊Ԋu�𒲂ׂ�
	diff = m_createPositions[count] -
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
		float length = (m_createPositions[count] - m_createPositions[i]).Length();
		//����������Ă�����A����������W�Ɍ���
		if (length > distance)
		{
			distanceCount++;
		}
	}
	//�S�Ă̍��W�Ƌ������󂢂Ă�����
	if (distanceCount == count)
	{
		//�󂢂Ă���
		return true;
	}
	else
	{
		//�󂢂Ă��Ȃ�
		return false;
	}
	
}

void BattlePhase::AdjustDifficultyByPhase(const int summonMonstersCount)
{
	//��Փx�ɂ���Đ�������郂���X�^�[�̎�ނ𐧌�

	//�D�悷�郂�u�����X�^�[��ݒ肷�邩���߂�
	//�D�悷�郂�u�����X�^�[�̔ԍ����擾
	int priority = GetMobMonsterPriority();
	
	for (int num = 0; num < summonMonstersCount; num++)
	{
		//�����X�^�[�𐶐�����
		MobMonster* monster = GenerateMobMonster(priority);
		//���W��ݒ�
		monster->SetPosition(m_createPositions[num]);
		//���u�����X�^�[���X�g�Ɋi�[
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(monster);
		//�������̌��̃G�t�F�N�g�̍Đ�
		CreateSummonLightEffect(m_createPositions[num]);
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
		Vector3 Pos = m_createPositions[num];
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

const int BattlePhase::GetMobMonsterPriority()
{
	//�����l�͗D�悷�郂�u�����X�^�[�Ȃ�
	int num = NONE_PRIORITY;

	//�t�F�[�Y����x�����[�v���Ă��Ȃ��Ȃ�D�悷�郂�u�����X�^�[�����߂Ȃ�
	if (GameManager::GetInstance()->GetPhaseLoopCount() == 0)
	{
		return num;
	}

	//�D��x�����߂邩�̔��f
	int judgeValue = rand() % 21-10;
	//judgeValue�����l��艺�Ȃ�D��x�����߂Ȃ�
	if (judgeValue < 5)
	{
		return num;
	}

	//�D�悷�郂�u�����X�^�[�������_���Ɍ��߂�
	num = rand()% (int)enMobMonsterList_Num;

	return num;
}

MobMonster* BattlePhase::GenerateMobMonster(int priorityMobMonsterNumber)
{
	//�t�F�[�Y�ɂ���Đ������Ȃ������N�̃����X�^�[�����߂�

	//���u�����X�^�[�̃C���X�^���X
	MobMonster* monster = nullptr;
	//�������郂�u�����X�^�[�̔ԍ���������
	int num = 0;

	//�D�悵�������u�����X�^�[�̔ԍ�������Ȃ炻�����̔ԍ���D�悷��
	if (priorityMobMonsterNumber >= 0)
	{
		num = priorityMobMonsterNumber;
	}
	else
	{
		//�����X�^�[�̎�ނ��������_���ɑI��
		num = rand() % (int)enMobMonsterList_Num;
	}
	
	//�ԍ��ɑΉ����������X�^�[�𐶐�
	switch (num)
	{
	case enMobMonsterList_Slime:
		monster = NewGO<Slime>(0,"slime");
		break;
	case enMobMonsterList_Cactus:
		monster = NewGO<Cactus>(0, "cactus");
		break;
	case enMobMonsterList_Mushroom:
		monster = NewGO<Mushroom>(0, "mushroom");
		break;
	case enMobMonsterList_Turtleshell:
		monster = NewGO<TurtleShell>(0, "turtleshell");
		break;
	default:
		std::abort();
		break;
	}


	return monster;
}


