// Shoot then up game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUPlayerHudWidget.generated.h"

/**
 *
 */
UCLASS()
class SHOOTTHEMUP_API USTUPlayerHudWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
     bool GetWeaponUIData(FWeaponUIData& UIData) const;
};
