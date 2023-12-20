#include "stdafx.h"
#include "TurtleShellStateSkill.h"
#include "TurtleShell.h"

void TurtleShellStateSkill::ManageState()
{
	//防御する時間が終わったらアニメーション終わり	
	if (m_turtleShell->IsDifenceEnd() == true)
	{
		//共通のステート遷移処理実行
		m_turtleShell->ProcessCommonStateTransition();
	}
}

void TurtleShellStateSkill::PlayAnimation()
{
	m_turtleShell->GetModelRender().PlayAnimation(enAnimationClip_Skill);
}
