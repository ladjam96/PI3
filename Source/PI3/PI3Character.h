#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PI3Character.generated.h"

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

	void Move(const FInputActionValue& Value);

	void UseBlackHole();
	void UseShockwave();

	UFUNCTION(BlueprintPure)
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure)
	FRotator GetCharacterDirection() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UBlackHoleAbility* BlackHoleAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	UShockwaveAbility* ShockwaveAttack;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D MovementVector;
	
	UPROPERTY(BlueprintReadOnly)
	FVector2D RotationVector;

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
