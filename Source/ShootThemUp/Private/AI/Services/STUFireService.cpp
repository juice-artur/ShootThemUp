// Shoot them up game

#include "AI/Services/STUFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

USTUFireService::USTUFireService()
{
    NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboar = OwnerComp.GetBlackboardComponent();

    const auto HasAim = Blackboar && Blackboar->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent)
        {
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
