#pragma once
//#include "Component.h"

#include "IComponent.h"

/// <summary>
/// キャラクター移動用コンポーネント
/// </summary>
class CharacterMovement : public IComponent
{
public:



	void UpdateComponent() override;


};

