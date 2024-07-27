#pragma once


class WaveTimer
{
public:
	WaveTimer(){}
	~WaveTimer(){}




	/// <summary>
	/// �t�F�[�Y���ł̏����̃X�e�b�v
	/// </summary>
	enum EnWaveStep
	{
		enPhaseStep_WaveProgress,	//�E�F�[�u�i�s
		enPhaseStep_ShortBreak,		//���x�e
	};

	/// <summary>
	/// ����������
	/// </summary>
	void Init();




private:



private:

	float m_waveTimer = 0.0f;		//�E�F�[�u�Ǘ�����^�C�}�[

	int m_nowWaveCounter = 0;			//���݂̃E�F�[�u�̒i�K

	int m_brakeTimeWaveCount = 3;		//���̃J�E���g�ŋx�e�^�C���ɂ��邩

};

