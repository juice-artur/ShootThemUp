// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h" 
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/STUJoystickWidget.h"
#include "UI/STUBaseWidget.h"

USTUJoystickWidget* ASTUPlayerCharacter::Instance = nullptr;

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
    CameraCollisionComponent->SetupAttachment(CameraComponent);
    CameraCollisionComponent->SetSphereRadius(10.0f);
    CameraCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CameraCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

   /* if (JoystickWidgetClass)
    {
        JoystickWidgetClass->RemoveFromParent();
    }*/



}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(CameraCollisionComponent);

    CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionBeginOverlap);
    CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ASTUPlayerCharacter::OnCameraCollisionEndOverlap);


        if (/*JoystickWidgetClass*/ !Instance)
        {
        Instance = Cast<USTUJoystickWidget>(JoystickWidgetClass);
        Instance->AddToViewport();
            //JoystickWidget = CreateWidget<UUserWidget>(GetWorld(), JoystickWidgetClass);

          //  if (JoystickWidget)
           // {
              //  JoystickWidget->AddToViewport();
              /*  JoystickWidgetClass->AddToViewport();*/
                //JoystickWidget->Show();
                UE_LOG(LogTemp, Warning, TEXT(" JoystickWidget->AddToViewport();."));
          //  }
          //  else
          //  {
            //    UE_LOG(LogTemp, Warning, TEXT("Failed to create Joystick Widget."));
           // }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Joystick Widget class not set."));
        }
}

 void ASTUPlayerCharacter::Tick(float DeltaTime) 
 {
        Super::Tick(DeltaTime); 

       // auto* ss = Cast<USTUJoystickWidget>(JoystickWidgetClass);
        FVector2D val = Instance->GetJoystickValues();
        MoveForward(-val.Y /* == 0 ? 0 : GetJoystickValues().Y > 0 ? -1 : 1*/);
        MoveRight(val.X /*== 0 ? 0 : GetJoystickValues().X > 0 ? 1 : -1*/);
 }

void ASTUPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::OnCameraCollisionEndOverlap(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    CheckCameraOverlap();
}

void ASTUPlayerCharacter::CheckCameraOverlap()
{
    const auto HideMesh = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
    GetMesh()->SetOwnerNoSee(HideMesh);

    TArray<USceneComponent*> MeshChildren;
    GetMesh()->GetChildrenComponents(true, MeshChildren);

    for (auto MeshChild : MeshChildren)
    {
        if (const auto MeshChildGeometry = Cast<UPrimitiveComponent>(MeshChild))
        {
            MeshChildGeometry->SetOwnerNoSee(HideMesh);
        }
    }
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    APlayerController* PlayerController = Cast<APlayerController>(GetController());

    UEnhancedInputLocalPlayerSubsystem* Subsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

    Subsystem->ClearAllMappings();
    Subsystem->AddMappingContext(PlayerMappingContext, 0);

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Move);
    EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ASTUPlayerCharacter::StartFire);
    EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::StopFire);
    EnhancedInputComponent->BindAction(IA_TurnAround, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::TurnAround);
    EnhancedInputComponent->BindAction(IA_LookUp, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::LookUp);
    EnhancedInputComponent->BindAction(IA_NextWeapon, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::NextWeapon);
    EnhancedInputComponent->BindAction(IA_MobileCameraMove, ETriggerEvent::Started, this, &ASTUPlayerCharacter::OnTouchStarted);
    EnhancedInputComponent->BindAction(IA_MobileCameraMove, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::UpdateCameraPosition);
    
    //  PlayerInputComponent->BindAxis("MoveForward", this, &ASTUPlayerCharacter::MoveForward);
    // PlayerInputComponent->BindAxis("MoveRight", this, &ASTUPlayerCharacter::MoveRight);
    // PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASTUPlayerCharacter::OnStartFire);
    //  PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
    //  PlayerInputComponent->BindAxis("TurnAround", this, &ASTUPlayerCharacter::AddControllerYawInput);
   // PlayerInputComponent->BindAxis("LookUp", this, &ASTUPlayerCharacter::AddControllerPitchInput);
    // PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);


   // PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUPlayerCharacter::Jump);
   // PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUPlayerCharacter::OnStartRunning);
  //  PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUPlayerCharacter::OnStopRunning);

   // DECLARE_DELEGATE_OneParam(FZoomInputSignature, bool);
   // PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Zoom, true);
 //   PlayerInputComponent->BindAction<FZoomInputSignature>("Zoom", IE_Released, WeaponComponent, &USTUWeaponComponent::Zoom, false);
}

void ASTUPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);

    if (IsRunning() && WeaponComponent->IsFiring())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUPlayerCharacter::OnStartRunning()
{
    WantsToRun = true;
    if (IsRunning())
    {
        WeaponComponent->StopFire();
    }
}

void ASTUPlayerCharacter::OnStopRunning()
{
    WantsToRun = false;
}

bool ASTUPlayerCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    

}

void ASTUPlayerCharacter::OnStartFire()
{
    if (IsRunning()) return;
    WeaponComponent->StartFire();
}

void ASTUPlayerCharacter::Move(const FInputActionValue& Value) 
{
    MoveRight(Value.Get<FVector2D>().X);
    MoveForward(Value.Get<FVector2D>().Y);
}

void ASTUPlayerCharacter::StartFire(const FInputActionValue& Value)
{
    OnStartFire();
}

void ASTUPlayerCharacter::StopFire(const FInputActionValue& Value)
{
    WeaponComponent->StopFire();
}

void ASTUPlayerCharacter::TurnAround(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.Get<float>());
}

void ASTUPlayerCharacter::LookUp(const FInputActionValue& Value)
{
    AddControllerPitchInput(Value.Get<float>());
}

void ASTUPlayerCharacter::NextWeapon(const FInputActionValue& Value)
{
    WeaponComponent->NextWeapon();
}

void ASTUPlayerCharacter::OnTouchStarted(const FInputActionValue& Value) 
{

    FVector TouchPosition = Value.Get<FVector>();
    if (IsRightSide(TouchPosition))
    {
        StartFingerPosition.X= TouchPosition.X;
        StartFingerPosition.Y = TouchPosition.Y;
    }
}

void ASTUPlayerCharacter::UpdateCameraPosition(const FInputActionValue& Value) 
{
    FVector TouchPosition = Value.Get<FVector>();
    if (IsRightSide(TouchPosition))
    {
        FVector2D NewFingerPosition;

        NewFingerPosition.X = TouchPosition.X;
        NewFingerPosition.Y = TouchPosition.Y;

        AddControllerYawInput(-1*(StartFingerPosition.X - NewFingerPosition.X));

        AddControllerPitchInput(StartFingerPosition.Y - NewFingerPosition.Y);
        StartFingerPosition = NewFingerPosition;
    }
   
} 

bool ASTUPlayerCharacter::IsRightSide(FVector TouchLocation)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

    if (PlayerController)
    {
        ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

        if (LocalPlayer && LocalPlayer->ViewportClient)
        {
            FVector2D ViewportSize = LocalPlayer->ViewportClient->Viewport->GetSizeXY();

            if (TouchLocation.X > ViewportSize.X / 2)
            {
                return true;
            }
        }
    }

    return false;
}

