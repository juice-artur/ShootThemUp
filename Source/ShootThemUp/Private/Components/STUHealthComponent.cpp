// Shoot then up game


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    Health = MaxHealth;

	AActor* ComponentOwner = GetOwner();

	if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}


void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    Health -= Damage;
}

