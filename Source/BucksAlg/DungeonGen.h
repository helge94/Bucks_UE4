// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include <map>
#include <set>
#include "CoreMinimal.h"

/**
*
*/
typedef std::pair<int, int> Vector2D;
typedef std::vector<std::vector<Tile>> Tilemap;
class BUCKSALG_API DungeonGen
{
public:
	DungeonGen(int c, int r, int room, bool multi, int winding);
	Tilemap GetTilemap() { return m_Tiles; }
	int GetRoomAmount() { return m_AmountOfRooms; }
	int GetRegionValue() { return m_Regions; }
	std::vector<FVector2i> GetNeighbours() { return m_Neighbours; }
	~DungeonGen();

private:
	void AddRooms();
	void CreRegions();
	void CreateTilemap();
	void CreateMaze(Tile start);
	void RemoveDeadEnds();
	bool CanCarve(Tile tile, FVector2i dir);

	int m_Col, m_Row, m_Regions, m_MazeStartRegion, m_Winding, m_AmountOfRooms;
	bool m_MultiRoom;

	std::vector<FVector2i> m_Neighbours;
	std::vector<Rect> m_Rooms;
	Tilemap m_Tiles;
	std::map<int, Vector2D> m_RemovableTiles;
};
