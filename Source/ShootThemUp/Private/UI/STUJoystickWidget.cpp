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
    //APlayerController* PC = GetOwningPlayer();
    //if (PC)
    //{
    //    UEnhancedInputLocalPlayerSubsystem* Subsystem =
    //        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

    //    if (Subsystem)
    //    {
    //        UEnhancedPlayerInput* PlayerInput = Subsystem->GetPlayerInput();
    //        // Now you can use PlayerInput as needed
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem"));
    //    }
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Error, TEXT("Failed to get Owning PlayerController"));
    //}

//    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//
//    if (PlayerController)
//    {
//        PlayerCharacter = Cast<ASTUPlayerCharacter>(PlayerController->GetPawn());
//        UE_LOG(LogTemp, Error, TEXT("Failed to cast to ASTUPlayerCharacter"));
//    }
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
    InitialHandlePosition = InGestureEvent.GetScreenSpacePosition();
    return FReply::Handled();
}

FReply USTUJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
    JoystickPosition = FVector2D::ZeroVector;
    UpdateJoystickVisuals();
    return FReply::Handled();
}

FReply USTUJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
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