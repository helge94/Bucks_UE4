// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BUCKSALG_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	void SetCanInteract(bool NewInteract);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = Keycard)
		void ChangeOwner(AActor* NewOWner);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PickupMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* PickupRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UShapeComponent* PickupBox;

	UPROPERTY(EditAnywhere)
		uint32 bCanInteract : 1;

	UFUNCTION()
		void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:

	class ABucksAlgCharacter* OwningPlayer;
};
