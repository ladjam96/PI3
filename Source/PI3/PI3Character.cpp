#include "PI3Character.h"
#include "BlackHoleAbility.h"
#include "CustomPlayerController.h"
#include "ShockwaveAbility.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameOverMenu.h"
#include "InputActionValue.h"
#include "PlayerHUD.h"

DEFINE_LOG_CATEGORY_STATIC(LogPI3Character, Log, All);

API3Character::API3Character()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 500.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    // FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
    FollowCamera->SetRelativeRotation(FRotator(-75.f, 0.f, 0.f));
    FollowCamera->SetOrthoWidth(3500.f);

    BlackHoleAttack = CreateDefaultSubobject<UBlackHoleAbility>(TEXT("BlackHoleAttack"));
    ShockwaveAttack = CreateDefaultSubobject<UShockwaveAbility>(TEXT("Shockwave"));
    BaseAttack = CreateDefaultSubobject<UBaseAttack>(TEXT("BaseAttack"));

    IsDead = false;
    
    MaxHealth = 500.f;

    CurrentLevel = 1;
    CurrentExperience = 0.f;
    ExperienceToNextLevel = CalculateExperienceToNextLevel();

    Damage = 20.f;
}

void API3Character::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

    if (PlayerHUDClass)
    {
        PlayerHUDInstance = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDClass);
        if (PlayerHUDInstance)
        {
            PlayerHUDInstance->AddToViewport();
            PlayerHUDInstance->UpdateHealthBar(CurrentHealth, MaxHealth);
            PlayerHUDInstance->UpdateHealthText(CurrentHealth, MaxHealth);
            PlayerHUDInstance->UpdateExpBar(CurrentExperience, ExperienceToNextLevel);
            PlayerHUDInstance->UpdateExpText(CurrentExperience, ExperienceToNextLevel);
            PlayerHUDInstance->UpdateLevelText(CurrentLevel);
        }
    }

    if (GameOverMenuClass)
    {
        GameOverMenuInstance = CreateWidget<UGameOverMenu>(GetWorld(), GameOverMenuClass);
    }

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // FTimerHandle HealthDecreaseTimerHandle;
    // GetWorldTimerManager().SetTimer(HealthDecreaseTimerHandle, this, &API3Character::DecreaseHealth, 1.0f, true, 0.0f);
    //
    // FTimerHandle ExpIncreaseTimerHandle;
    // GetWorldTimerManager().SetTimer(ExpIncreaseTimerHandle, this, &API3Character::IncreaseExp, 1.0f, true, 0.0f);    
}

void API3Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //Camera Rotation & Location
    FollowCamera->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));
    FollowCamera->SetRelativeLocation(FVector(-100.0f, 0.0f, 370.0f)); 
    CameraBoom->SetRelativeLocation(FVector(0, 0 , 500.f));

    if (BlackHoleAttack)
    {
        BlackHoleAttack->UpdateCooldown(DeltaTime);

        float RemainingCooldownPercentage = 100.f - (BlackHoleAttack->CurrentCooldown / BlackHoleAttack->Cooldown) * 100.f;
        PlayerHUDInstance->UpdateBlackholeBar(RemainingCooldownPercentage);
    }

    if (ShockwaveAttack)
    {
        ShockwaveAttack->UpdateCooldown(DeltaTime);

        float RemainingCooldownPercentage = 100.f - (ShockwaveAttack->CurrentCooldown / ShockwaveAttack->Cooldown) * 100.f;
        PlayerHUDInstance->UpdateShockwaveBar(RemainingCooldownPercentage);
    }

    if(BaseAttack)
    {
        BaseAttack->UpdateCooldown(DeltaTime);
    }

    CheckIfDead();

    if(IsDead)
    {
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &API3Character::ShowGOMenu, 3.0f, false);
    }
}

void API3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &API3Character::Move);
        EnhancedInputComponent->BindAction(BlackHoleAction, ETriggerEvent::Triggered, this, &API3Character::UseBlackHole);
        EnhancedInputComponent->BindAction(ShockwaveAction, ETriggerEvent::Triggered, this, &API3Character::UseShockwave);
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &API3Character::UseBasicAttack);
    }
    else
    {
        UE_LOG(LogPI3Character, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
    }
}

void API3Character::Move(const FInputActionValue& Value)
{
    MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

// void API3Character::DecreaseHealth()
// {
//     CurrentHealth -= 50.0f;
//     CurrentHealth = FMath::Max(CurrentHealth, 0.0f);
//
//     if (PlayerHUDClass && PlayerHUDInstance)
//     {
//         PlayerHUDInstance->UpdateHealthBar(CurrentHealth, MaxHealth);
//         PlayerHUDInstance->UpdateHealthText(CurrentHealth, MaxHealth);
//     }
//
//     if(CurrentHealth <= 0)
//     {
//         IsDead = true;
//
//         APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//         APawn* PlayerPawn = PlayerController->GetPawn(); 
//         
//         PlayerPawn->DisableInput(PlayerController);
//         
//         UE_LOG(LogTemp, Log, TEXT("Character has died!"));
//     }
// }

void API3Character::Attack(float DamageAmount)
{    

}

void API3Character::TakeDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;

    if(CurrentHealth <= 0)
    {
        IsDead = true;

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        APawn* PlayerPawn = PlayerController->GetPawn(); 
        
        PlayerPawn->DisableInput(PlayerController);
    }

    if (PlayerHUDInstance)
    {
        PlayerHUDInstance->UpdateHealthBar(CurrentHealth, MaxHealth);
        PlayerHUDInstance->UpdateHealthText(CurrentHealth, MaxHealth);
    }
}

// void API3Character::IncreaseExp()
// {
//     if (IsDead)
//     {
//         return;
//     }
//
//     CurrentExperience += 10.f;
//
//     if (PlayerHUDClass && PlayerHUDInstance)
//     {
//         PlayerHUDInstance->UpdateExpBar(CurrentExperience, ExperienceToNextLevel);
//         PlayerHUDInstance->UpdateExpText(CurrentExperience, ExperienceToNextLevel);
//     }
//
//     if (CurrentExperience >= ExperienceToNextLevel)
//     {
//         LevelUp();
//     }
// }

void API3Character::GainExperience(float ExperienceAmount)
{
    if (IsDead)
    {
        return;
    }

    CurrentExperience += ExperienceAmount;

    while (CurrentExperience >= ExperienceToNextLevel)
    {
        CurrentExperience -= ExperienceToNextLevel;
        LevelUp();
    }

    if (PlayerHUDInstance)
    {
        PlayerHUDInstance->UpdateExpBar(CurrentExperience, ExperienceToNextLevel);
        PlayerHUDInstance->UpdateExpText(CurrentExperience, ExperienceToNextLevel);
    }
}

void API3Character::LevelUp()
{
    CurrentLevel++;
    ExperienceToNextLevel = CalculateExperienceToNextLevel();
    CurrentExperience = 0.0f;
    UE_LOG(LogTemp, Log, TEXT("Leveled up to %d"), CurrentLevel);

    if (PlayerHUDInstance)
    {
        PlayerHUDInstance->UpdateLevelText(CurrentLevel);
        PlayerHUDInstance->UpdateExpBar(CurrentExperience, ExperienceToNextLevel);
        PlayerHUDInstance->UpdateExpText(CurrentExperience, ExperienceToNextLevel);
    }
}

float API3Character::CalculateExperienceToNextLevel() const
{
    return 100.0f * FMath::Pow(CurrentLevel, 1.5f);
}

void API3Character::UseAbility(UBaseAbility* Ability)
{
    if (Ability && !Ability->IsOnCooldown())
    {
        Ability->Activate();
    }
}

void API3Character::UseBlackHole()
{
    UseAbility(BlackHoleAttack);
}

void API3Character::UseShockwave()
{
    UseAbility(ShockwaveAttack);
}

void API3Character::UseBasicAttack()
{
    UseAbility(BaseAttack);
}


void API3Character::ShowGOMenu()
{
    if (GameOverMenuInstance)
    {
        GameOverMenuInstance->AddToViewport();

        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController)
        {
            PlayerController->SetPause(true);
            PlayerController->bShowMouseCursor = true;
            PlayerController->SetInputMode(FInputModeUIOnly());
        }
    }
}

void API3Character::CheckIfDead()
{
    if (IsDead && GameOverMenuClass && !GameOverMenuInstance)
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->SetPause(true);
        }
    }
}

FVector API3Character::GetCharacterVelocity() const
{
    return GetCharacterMovement()->Velocity;
}

FRotator API3Character::GetCharacterDirection() const
{
    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        return FRotator(0.0f, Rotation.Yaw, 0.0f);
    }
    return FRotator::ZeroRotator;
}
