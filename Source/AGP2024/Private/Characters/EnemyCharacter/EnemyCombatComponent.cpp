#include "Characters/EnemyCharacter/EnemyCombatComponent.h"

#include "Characters/EnemyCharacter/EnemyCharacter.h"
#include "CombatSystem/ArrowShooterComponent.h"

UEnemyCombatComponent::UEnemyCombatComponent()
{
	AttackCooldown = 2.5f;
	bAttackContinuously = false;
}

void UEnemyCombatComponent::Attack()
{
	Super::Attack();

	if(OwningEnemyCharacter && OwningEnemyCharacter->EnemyShooterComponent && TargetPawn)
	{
		FVector NewShotDirection = TargetPawn->GetActorLocation() - OwningEnemyCharacter->EnemyShooterComponent->GetComponentLocation();
		NewShotDirection.Normalize();
		OwningEnemyCharacter->EnemyShooterComponent->SetShotDirection(NewShotDirection);
		OwningEnemyCharacter->EnemyShooterComponent->Shoot();
	}
}

void UEnemyCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningEnemyCharacter = CastChecked<AEnemyCharacter>(GetOwner());
	
	if (OwningEnemyCharacter->GetMesh())
	{
		EnemyCharacterMeshAnimInstance = OwningEnemyCharacter->GetMesh()->GetAnimInstance();
		check(EnemyCharacterMeshAnimInstance)
	}
}

void UEnemyCombatComponent::AttemptAttack(APawn* PawnToAttack)
{
	if(AttackIsOnCooldown()) return;

	if(TargetPawn != PawnToAttack) TargetPawn = PawnToAttack;
	if(EnemyCharacterMeshAnimInstance && AttackMontage)
	{
		if(!EnemyCharacterMeshAnimInstance->Montage_IsPlaying(NULL))
		{
			EnemyCharacterMeshAnimInstance->Montage_Play(AttackMontage);
			
			if(const UWorld* World = GetWorld())
			{
				LastAttackTime = World->GetTimeSeconds();
			}
		}
	}
}