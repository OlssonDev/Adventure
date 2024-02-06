// Fill out your copyright notice in the Description page of Project Settings.


#include "Volumes/AdventureTriggerBoxVolume.h"
#include "Components/BoxComponent.h"
#include "AdventureComponent.h"
#include "AdventureGlobals.h"
#include "GameFramework/PlayerState.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameState.h"


AAdventureTriggerBoxVolume::AAdventureTriggerBoxVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBoxVolume = CreateDefaultSubobject<UBoxComponent>("TriggerBoxVolume");
	TriggerBoxVolume->SetBoxExtent(FVector(400.f));
	SetRootComponent(TriggerBoxVolume);

	AdventureIconBillboard = CreateDefaultSubobject<UBillboardComponent>("Adventure Icon Billboard");
	AdventureIconBillboard->SetupAttachment(TriggerBoxVolume);
	AdventureIconBillboard->SetWorldScale3D(FVector(4.0f));

	static ConstructorHelpers::FObjectFinder<UTexture2D> StaticTexture(TEXT("/Adventure/Icons/T_AdventureAssetIcon.T_AdventureAssetIcon"));
	AdventureIconBillboard->SetSprite(StaticTexture.Object);
}

void AAdventureTriggerBoxVolume::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TriggerBoxVolume->OnComponentBeginOverlap.AddDynamic(this, &AAdventureTriggerBoxVolume::OnComponentOverlapped);
	}
}

void AAdventureTriggerBoxVolume::OnComponentOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		UAdventureComponent* AdventureComponent = nullptr;

		auto FindComponent = [](AActor* Actor)
		{
			return UAdventureGlobals::GetAdventureComponentFromActor(Actor, true);
		};
	
		switch (GetterSettings)
		{
			case CharacterOrPawn:
			{
				if (APawn* Pawn = Cast<APawn>(OtherActor))
				{
					AdventureComponent = FindComponent(OtherActor);
				}
				break;
			}

			case PlayerController:
			{
				if (APawn* Pawn = Cast<APawn>(OtherActor))
				{
					AdventureComponent = FindComponent(Pawn->Controller);
				}
				break;
			}

			case PlayerState:
			{
				if (APawn* Pawn = Cast<APawn>(OtherActor))
				{
					AdventureComponent = FindComponent(Pawn->GetPlayerState());
				}
				break;
			}

			case GameState:
			{
				AdventureComponent = FindComponent(GetWorld()->GetGameState());
				break;
			}

			case GameMode:
			{
				AdventureComponent = FindComponent(GetWorld()->GetAuthGameMode());
				break;
			}
		
			default:
			{
				break;
			};
		}
	
		if (IsValid(AdventureComponent))
		{
			RunAdventureComponentOperation(AdventureComponent);
		}
	}
}

