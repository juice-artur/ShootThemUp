// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Layout/Geometry.h"
#include "STUJoystickWidget.generated.h"

class UImage;
class ASTUPlayerCharacter;

UCLASS()
class SHOOTTHEMUP_API USTUJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Joystick")
    void UpdateJoystickPosition(FVector2D NewPosition);

    UFUNCTION(BlueprintCallable, Category = "Joystick")
    FVector2D GetJoystickValues() const;

    UPROPERTY(meta = (BindWidget))
    UImage* JoystickBackgroundImage;

    UPROPERTY(meta = (BindWidget))
    UImage* JoystickHandleImage;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


protected:


private:
    FVector2D InitialHandlePosition;

    FVector2D JoystickPosition;
    float JoystickRadius;

    ASTUPlayerCharacter* PlayerCharacter = nullptr;

    virtual void NativeConstruct() override;

    void UpdateJoystickVisuals();

    virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
    virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
    virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};
