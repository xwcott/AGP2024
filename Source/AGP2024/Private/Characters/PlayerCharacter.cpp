#include "Characters/PlayerCharacter.h"

#include "Player/CharacterInputManager.h"
#include "InteractionSystem/CharacterInteractionComponent.h"
#include "Player/CustomCharacterMovement.h"

const FName NAME_WeaponSocket(TEXT("weapon"));

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super
(ObjectInitializer.SetDefaultSubobjectClass<UCustomCharacterMovement>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Create ArmsMesh
	ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
	ArmsMesh->SetupAttachment(GetMesh());
	ArmsMesh->CastShadow = false;

	// Create WeaponMesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(ArmsMesh);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Create CharacterInputManager
	CharacterInputManager = CreateDefaultSubobject<UCharacterInputManager>(TEXT("InputManager"));

	// Create CharacterInteractionComponent
	CharacterInteractionComponent = CreateDefaultSubobject<UCharacterInteractionComponent>(TEXT("InteractionComponent"));

	CustomCharacterMovement->bCanWalkOffLedgesWhenCrouching = true;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		CharacterInputManager->SetupCharacterInput(PlayerInputComponent, PlayerController);
	}
}