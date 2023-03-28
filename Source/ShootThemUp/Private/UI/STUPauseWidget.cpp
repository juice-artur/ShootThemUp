// Shoot them up game

#include "STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"

void USTUPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ClearPauseButton)
    {
        ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
    }
}

void USTUPauseWidget::OnClearPause()
{
    if (!GetWorld() || !GetWorld()->GetAuthGameMode())
    {
        return;
    }

    GetWorld()->GetAuthGameMode()->ClearPause();
}
