#pragma once

#include "CoreMinimal.h"
#include "CombatSystem/DamageableInterface.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class ULevelManagerWorldSub;
class UCustomCharacterMovement;
class UCharacterInteractionComponent;
class UInputAction;
class UInputMappingContext;
class UPlayerCombatComponent;

const FName NAME_CameraBone(TEXT("camera"));

UCLASS()
class AGP2024_API APlayerCharacter : public ACharacter, public IDamageableInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);
	
	FVector GetCameraTargetLocation() const { return ArmsMesh->GetSocketLocation(NAME_CameraBone); }
	FRotator GetCameraTargetRotation() const { return ArmsMesh->GetSocketRotation(NAME_CameraBone); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkeletalMeshComponent* GetArmsMesh() const { return ArmsMesh; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCharacterInteractionComponent* GetCharacterInteraction() const { return CharacterInteractionComponent; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CharacterInputEnabled() const { return bCharacterInputEnabled; };
	
	virtual void Damage() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCustomCharacterMovement> CustomCharacterMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> ArmsMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCharacterInteractionComponent> CharacterInteractionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UPlayerCombatComponent> PlayerCombatComponent;

	UFUNCTION()
	void EnableCharacterInput(bool EnableInput);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Properties")
	bool bIsImmuneToDamage = false;

	bool bCharacterInputEnabled = false;

private:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputMappingContext* PlayerCharacterMappingContext;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* SprintAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "PlayerInput", meta=(AllowPrivateAccess=true))
	UInputAction* AttackAction;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};