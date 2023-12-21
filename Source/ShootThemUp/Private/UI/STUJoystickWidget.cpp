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
    FReply Reply =  Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
    InitialHandlePosition = InGestureEvent.GetScreenSpacePosition();
    return Reply;
}

FReply USTUJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    FReply Reply = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
    JoystickPosition = FVector2D::ZeroVector;
    UpdateJoystickVisuals();
    return Reply;
}

FReply USTUJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    FReply Reply =  Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
    if (!IsHovered())
    {
        // Reset joystick position if the finger is outside
        JoystickPosition = FVector2D::ZeroVector;
    }
    else
    {
        FVector2D Delta = InGestureEvent.GetScreenSpacePosition() - InitialHandlePosition;
        UpdateJoystickPosition(InGestureEvent.GetCursorDelta());
        InitialHandlePosition = InGestureEvent.GetScreenSpacePosition();
    }

    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
            FString::Printf(TEXT("%s::::::%s"), *InitialHandlePosition.ToString(), *GetJoystickValues().ToString()));
    }

    return Reply;

}


void USTUJoystickWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}