// Shoot them up game

#include "Components/STUAIPerceptionComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "STUHealthComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
    TArray<AActor*> PersiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PersiveActors);

    if (PersiveActors.Num() == 0)
    {
        return nullptr;
    }

    const auto Controller = Cast<AAIController>(GetOwner());

    if (!Controller)
    {
        return nullptr;
    }

    const auto Pawn = Controller->GetPawn();

    if (!Pawn)
    {
        return nullptr;
    }

    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;

    for (const auto PersiveActor : PersiveActors)
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PersiveActor);

        const auto PercievePawn = Cast<APawn>(PersiveActor);
        const auto AreEnemies = PercievePawn && STUUtils::AreEnemies(Controller, PercievePawn->Controller);

        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const auto CurrentDistance = (PersiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();

            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PersiveActor;
            }
        }
    }

    return BestPawn;
}
