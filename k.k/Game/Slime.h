#pragma once
#include "MobEnemyBase.h"



/// <summary>
/// ���u�G�l�~�[�F�X���C���N���X
/// </summary>
class Slime : public MobEnemyBase
{
public:
	~Slime();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;


	/// <summary>
	/// �X�V����
	/// </summary>
	void UpdateCharacter() override;



private:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void SettingDefaultComponent() override;



private:



};

