// Shoot them up game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUAmmoPickup.h"
#include "STUHealthComponent.h"
#include "STUUtils.h"
#include "STUHealthPickup.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUAmmoPickup
{
    GENERATED_BODY()
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float HealthAmount = 100.0f;

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override
    {
        const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
        if (!HealthComponent)
        {
            return false;
        }
        return HealthComponent->TryToAddHealth(HealthAmount);
    }
};
