// Shoot then up game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"


class ASTUProjectile;

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapom")
    TSubclassOf<ASTUProjectile> ProjectileClass;

    virtual void MakeShot() override;
};