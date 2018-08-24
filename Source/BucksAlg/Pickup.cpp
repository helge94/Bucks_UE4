// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup.h"
#include "BucksAlgCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(PickupRoot);
	//PickupMesh->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	PickupBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupBox->bGenerateOverlapEvents = true;
	PickupBox->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnPlayerEnterPickupBox);
	PickupBox->SetupAttachment(PickupRoot);
	//PickupBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PickupBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void APickup::SetCanInteract(bool NewInteract)
{

	bCanInteract = NewInteract;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APickup::ChangeOwner(AActor * NewOWner)
{
	ABucksAlgCharacter* Player = Cast<ABucksAlgCharacter>(NewOWner);
	if (Player) {
		OwningPlayer = Player;
	}
	SetOwner(NewOWner);
}

void APickup::OnPlayerEnterPickupBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ABucksAlgCharacter* Player = Cast<ABucksAlgCharacter>(OtherActor);
	if (Player) {
		Player->AddKeycardToInventory(this);
	}
	//Destroy();
}

