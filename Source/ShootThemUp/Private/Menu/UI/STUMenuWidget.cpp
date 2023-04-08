// Shoot them up game

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    if (!GetWorld())
    {
        return;
    }

    const auto STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>();
    if (!STUGameInstance)
    {
        return;
    }

    if (STUGameInstance->GetStartupLevelName().IsNone())
    {
        UE_LOG(LogSTUMenuWidget, Error, TEXT("Level name is NONE"));
        return;
    }

    UGameplayStatics::OpenLevel(this, STUGameInstance->GetStartupLevelName());
}

void USTUMenuWidget::OnStartGame()
{
    const FName StartupLevelName = "TestLevel";
    UGameplayStatics::OpenLevel(this, StartupLevelName);
}
