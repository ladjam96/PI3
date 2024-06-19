#include "EnemyHealthBar.h"

void UEnemyHealthBar::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		float HealthPercentage = FMath::Clamp(CurrentHealth / MaxHealth, 0.0f, 1.0f);
		HealthBar->SetPercent(HealthPercentage);
	}
}