
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMap.generated.h"

UENUM(BlueprintType)
enum ETileType {
	mazeTile,
	roomTile,
	wallTIle,
	noneTile,
	doorTile,
	keycardTile,
	lockedDoorTile,
	elevatorTile,
	elevatorStopTile,
	elevatorPassingTile
};

UCLASS(Blueprintable)
class BUCKSALG_API ATileMap : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATileMap();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void CreateTilemap(int i);

	UFUNCTION(BlueprintCallable, Category = "Functions")
		void ConnectLevels();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TArray<int> m_Tilemap1;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TArray<int> m_Tilemap2;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TArray<int> m_Tilemap3;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TArray<int> m_Tilemap4;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		TArray<int> m_Tilemap5;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		int m_MaxY;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		int m_MaxX;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		int m_Tilesize;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		int m_Rooms;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		int m_Winding;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		bool m_MultipleConnections;
};
