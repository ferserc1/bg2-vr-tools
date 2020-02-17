
#include "HMDSettings.h"
#include "Kismet/GameplayStatics.h"
#include "IXRTrackingSystem.h"

FString UHMDSettings::SaveSlotName = "HMDSettings";
uint32 UHMDSettings::SaveUserIndex = 0;

UHMDSettings::UHMDSettings()
	: Super()
{
}

UHMDSettings* UHMDSettings::Save(UWorld* World)
{
	UHMDSettings* SaveInstance = Cast<UHMDSettings>(UGameplayStatics::CreateSaveGameObject(UHMDSettings::StaticClass()));
	if (SaveInstance && GEngine && GEngine->XRSystem)
	{
		SaveInstance->HMDOffset = GEngine->XRSystem->GetBasePosition();

		if (UGameplayStatics::SaveGameToSlot(SaveInstance, SaveSlotName, SaveUserIndex))
		{
			UE_LOG(LogTemp, Display, TEXT("HMD settings saved"));
			return SaveInstance;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Error saving HMD settings"));
		}
	}
	return nullptr;
}

UHMDSettings* UHMDSettings::Restore(UWorld* World)
{
	UHMDSettings* SaveInstance = Cast<UHMDSettings>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveUserIndex));
	if (SaveInstance && GEngine && GEngine->XRSystem)
	{
		GEngine->XRSystem->SetBasePosition(SaveInstance->HMDOffset);
		UE_LOG(LogTemp, Display, TEXT("HMD settings restored: offset=%f,%f,%f"),
			SaveInstance->HMDOffset.X,
			SaveInstance->HMDOffset.Y,
			SaveInstance->HMDOffset.Z);
		return SaveInstance;
	}
	return nullptr;
}
