#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PI3Character.generated.h"

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
	void DecreaseHealth();
	void IncreaseExp();

	void Move(const FInputActionValue& Value);

	void UseBlackHole();
	void UseShockwave();

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;

	UFUNCTION(BlueprintCallable)
	void GainExperience(float ExperienceAmount);

	// UFUNCTION(BlueprintCallable)
	// void MakeDamage(float DamageAmount);
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float DamageAmount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackHoleAbility* BlackHoleAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UShockwaveAbility* ShockwaveAttack;

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
	float CalculateExperienceToNextLevel() const;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlackHoleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShockwaveAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPlayerHUD* PlayerHUDInstance;
	
	void UseAbility(UBaseAbility* Ability);
};
