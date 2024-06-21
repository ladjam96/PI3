#pragma once

#include "CoreMinimal.h"
#include "BaseAttack.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PI3Character.generated.h"

class AEnemyCharacter;
class ACustomPlayerController;
class UBaseAbility;
class UShockwaveAbility;
class UBlackHoleAbility;

UCLASS(config=Game)
class API3Character : public ACharacter
{
	GENERATED_BODY()

public:
	API3Character();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// void DecreaseHealth();

	void Move(const FInputActionValue& Value);

	void UseBlackHole();
	void UseShockwave();
	void UseBasicAttack();

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;

	UFUNCTION(BlueprintCallable)
	void GainExperience(float ExperienceAmount);
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	UFUNCTION()
	void ShowGOMenu();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameOverMenu> GameOverMenuClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> ImproveAbilitiesClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackHoleAbility* BlackHoleAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShockwaveAbility* ShockwaveAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBaseAttack* BaseAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 CurrentLevel = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentExperience = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExperienceToNextLevel = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 0.f;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasHit = false;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D MovementVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D RotationVector;

	UFUNCTION()
	void LevelUp();

	UFUNCTION()
	void ShowImproveAbilitiesMenu();

	UFUNCTION()
	float CalculateExperienceToNextLevel() const;

private:

	void CheckIfDead();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlackHoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShockwaveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY()
	UGameOverMenu* GameOverMenuInstance;

	UPROPERTY()
	UPlayerHUD* PlayerHUDInstance;

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter = nullptr;
	
	void UseAbility(UBaseAbility* Ability);
};
