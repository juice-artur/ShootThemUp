// Shoot them up game

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUAmmoPickup.h"
#include "STUHealthPickup.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTUHealthPickup : public ASTUAmmoPickup
{
    GENERATED_BODY()

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};
