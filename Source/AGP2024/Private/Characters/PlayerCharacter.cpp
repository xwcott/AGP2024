#include "Characters/PlayerCharacter.h"
#include "InteractionSystem/CharacterInteractionComponent.h"
#include "Player/CustomCharacterMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CombatSystem/CombatComponent.h"
#include "Components/InputComponent.h"

const FName NAME_WeaponSocket(TEXT("weapon_R"));

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super
(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	CustomCharacterMovement = Cast<UCustomCharacterMovement>(GetCharacterMovement());
	
	PrimaryActorTick.bCanEverTick = false;
	
	// Create ArmsMesh
	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ArmsMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmsMesh->SetupAttachment(GetMesh());
	ArmsMesh->CastShadow = false;

	// Create WeaponMesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetupAttachment(ArmsMesh, NAME_WeaponSocket);
	WeaponMesh->CastShadow = false;

	// Create CharacterInteractionComponent
	CharacterInteractionComponent = CreateDefaultSubobject<UCharacterInteractionComponent>(TEXT("InteractionComponent"));

	// Create CombatComponent
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CustomCharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
}

void APlayerCharacter::Damage()
{
	CombatComponent->GameOver();
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Add mapping context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerCharacterMappingContext, 0);
		}
		
		UCustomCharacterMovement* CustomMovementTemp = GetComponentByClass<UCustomCharacterMovement>();
		check(CustomMovementTemp)
		UCharacterInteractionComponent* CharacterInteractionTemp = GetComponentByClass<UCharacterInteractionComponent>();
		check(CharacterInteractionTemp)
		UCombatComponent* CombatCompTemp = GetComponentByClass<UCombatComponent>();
		check(CombatCompTemp)
		
		// Bind functions to input actions
		if (UEnhancedInputComponent* EnhancedInputComponent =
			CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
		{
			EnhancedInputComponent->BindAction(
				MoveAction,
				ETriggerEvent::Triggered,
				CustomMovementTemp,
				&UCustomCharacterMovement::OnMoveInputReceived);
		
			EnhancedInputComponent->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				CustomMovementTemp,
				&UCustomCharacterMovement::OnLookInputReceived);
			
			EnhancedInputComponent->BindAction(
				JumpAction,
				ETriggerEvent::Triggered,
				CustomMovementTemp,
				&UCustomCharacterMovement::OnJumpInputReceived);
			
			EnhancedInputComponent->BindAction(
				CrouchAction,
				ETriggerEvent::Triggered,
				CustomMovementTemp,
				&UCustomCharacterMovement::OnCrouchInputReceived);
			
			EnhancedInputComponent->BindAction(
				SprintAction,
				ETriggerEvent::Triggered,
				CustomMovementTemp,
				&UCustomCharacterMovement::OnRunInputReceived);
			
			EnhancedInputComponent->BindAction(
				InteractAction,
				ETriggerEvent::Triggered,
				CharacterInteractionTemp,
				&UCharacterInteractionComponent::OnInteractInputReceived);
			
			EnhancedInputComponent->BindAction(
				AttackAction,
				ETriggerEvent::Triggered,
				CombatCompTemp,
				&UCombatComponent::OnAttackInputReceived);
		}
	}
}