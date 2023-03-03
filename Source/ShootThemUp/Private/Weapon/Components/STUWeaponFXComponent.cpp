// Shoot them up game

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImapactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            ImapactData = ImpactDataMap[PhysMat];
        }
    }

    // niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),  //
        ImapactData.NiagaraEffect,                              //
        Hit.ImpactPoint,                                        //
        Hit.ImpactNormal.Rotation());

    // decal
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  //
        ImapactData.DecalData.Material,                                       //
        ImapactData.DecalData.Size,                                           //
        Hit.ImpactPoint,                                                      //
        Hit.ImpactNormal.Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImapactData.DecalData.LifeTime, ImapactData.DecalData.FadeOutTime);
    }
}