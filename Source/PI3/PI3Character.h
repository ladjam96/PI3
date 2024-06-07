#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PI3Character.generated.h"

UCLASS(config=Game)
class API3Character : public ACharacter
{
	GENERATED_BODY()

public:
	API3Character();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);

	UFUNCTION(BlueprintPure, Category = "Movement")
	FVector GetCharacterVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Movement")
	FRotator GetCharacterDirection() const;

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	
	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector2D MovementVector;
	
	UPROPERTY(BlueprintReadOnly, Category = "Rotation")
	FVector2D RotationVector;

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};
