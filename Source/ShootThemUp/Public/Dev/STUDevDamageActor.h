// Shoot then up game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUDevDamageActor.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUDevDamageActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ASTUDevDamageActor();

    UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
    USceneComponent* SceneComponent;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    float Radius = 300.0f;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    FColor SphereColor = FColor::Red;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    float Damage = 10.0f;

    UPROPERTY(EditAnyWhere, BlueprintReadWrite)
    bool DoFullDamage = false;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
