// Shoot then up game


#include "Components/STUHealthComponent.h"

// Sets default values for this component's properties
USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    Health = MaxHealth;
}


