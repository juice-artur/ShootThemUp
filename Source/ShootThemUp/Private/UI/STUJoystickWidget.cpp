#include "UI/STUJoystickWidget.h"
#include "Components/Image.h"
#include "Math/Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/STUPlayerCharacter.h"



void USTUJoystickWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (JoystickBackgroundImage == nullptr || JoystickHandleImage == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Joystick images not set in UVirtualJoystickWidget!"));
        return;
    }

    FVector2D BackgroundSize = JoystickBackgroundImage->GetBrush().GetImageSize();
    JoystickRadius = FMath::Min(BackgroundSize.X, BackgroundSize.Y) * 0.5f;
    InitialHandlePosition = JoystickHandleImage->RenderTransform.Translation;
}

void USTUJoystickWidget::UpdateJoystickPosition(FVector2D NewPosition)
{
    JoystickPosition += NewPosition;
    if (GetJoystickValues().Length() > 1)
    {
        JoystickPosition = FVector2D::ZeroVector;
    }

    UpdateJoystickVisuals();

}

FVector2D USTUJoystickWidget::GetJoystickValues() const
{
    return JoystickPosition / JoystickRadius;
}

void USTUJoystickWidget::UpdateJoystickVisuals()
{
    if (JoystickHandleImage != nullptr)
    {
        FVector2D HandlePosition = JoystickPosition;
        JoystickHandleImage->SetRenderTranslation(HandlePosition);
    }
}

FReply USTUJoystickWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
    InitialHandlePosition = InGestureEvent.GetScreenSpacePosition();
    return FReply::Handled();
}

FReply USTUJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
    JoystickPosition = FVector2D::ZeroVector;
    UpdateJoystickVisuals();
    return FReply::Handled();
}

FReply USTUJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
    FVector2D Delta = InGestureEvent.GetScreenSpacePosition() - InitialHandlePosition;

    UpdateJoystickPosition(InGestureEvent.GetCursorDelta());

    InitialHandlePosition = InGestureEvent.GetScreenSpacePosition();

    
    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
            FString::Printf(TEXT("%s::::::%s"), *InitialHandlePosition.ToString(), *GetJoystickValues().ToString()));
    return FReply::Handled();

}


void USTUJoystickWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}