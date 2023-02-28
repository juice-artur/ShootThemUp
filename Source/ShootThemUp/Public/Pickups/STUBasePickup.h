// Shoot them up game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ASTUBasePickup();

protected:
    UPROPERTY(VisibleAnyWhere, Category = "Pickup")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnyWhere, Category = "Pickup")
    float RespawnTime = 5.0f;

    virtual void BeginPlay() override;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    float RotatioYaw = 0.0f;

    UPROPERTY()
    TArray<APawn*> OverlappingPawns;

    virtual bool GivePickupTo(APawn* PlayerPawn);
    void PickupWasTaken();
    void Respawn();

    void GenerateRotatioYaw();
};
