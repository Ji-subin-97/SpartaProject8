// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpartaAttributeSet.h"

USpartaAttributeSet::USpartaAttributeSet()
{
	// 기본 체력 설정
	Health.SetBaseValue(100.f);
	Health.SetCurrentValue(100.f);

	MaxHealth.SetBaseValue(Health.GetBaseValue());
	MaxHealth.SetCurrentValue(Health.GetCurrentValue());
}
