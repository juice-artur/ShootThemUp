// Shoot then up game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()
public:
    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float TimeBetweenShots = 0.1f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    virtual void MakeShot() override;

private:
    FTimerHandle ShotTimerHandle;
};
