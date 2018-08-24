// Fill out your copyright notice in the Description page of Project Settings.

#include "TileMap.h"
#include "Time.h"
#include <unordered_set>
#include "DungeonGen.h"


// Sets default values
ATileMap::ATileMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	srand(time(NULL));

}


// Called when the game starts or when spawned
void ATileMap::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATileMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileMap::CreateTilemap(int i) {

	TArray<int> m_Tilemap;
	m_Tilemap.SetNum((m_MaxY * m_MaxX));


	DungeonGen dungeon(m_MaxX, m_MaxY, m_Rooms, m_MultipleConnections, m_Winding);

	int startRoom = rand() % dungeon.GetRoomAmount();

	int keycardRoom = -1, lockedRoom = -1;
	bool isLockedDoor = false;

	if (dungeon.GetRoomAmount() > 2) {
		while (keycardRoom == -1 || keycardRoom == startRoom)
			keycardRoom = rand() % dungeon.GetRoomAmount();

		while (lockedRoom == -1 || lockedRoom == keycardRoom || lockedRoom == startRoom)
			lockedRoom = rand() % dungeon.GetRoomAmount();
	}


	std::vector<Vector2D> validTiles, keycardTiles;

	Vector2D lockedDoor;
	for (int i = 0; i < dungeon.GetTilemap().size(); i++) {
		for (int j = 0; j < dungeon.GetTilemap()[i].size(); j++) {

			if (dungeon.GetTilemap()[i][j].m_Region == startRoom)
				validTiles.push_back(std::make_pair(i, j));
			else if (dungeon.GetTilemap()[i][j].m_Region == keycardRoom)
				keycardTiles.push_back(std::make_pair(i, j));

			if (dungeon.GetTilemap()[i][j].m_Type == connectedFloor && !isLockedDoor) {
				bool mTemp = false;
				for (int k = 0; k < dungeon.GetNeighbours().size(); k++) {
					FVector2i temp = dungeon.GetNeighbours()[k];

					if (dungeon.GetTilemap()[i + temp.x][j + temp.y].m_Region == lockedRoom)
						mTemp = true;
					if (dungeon.GetTilemap()[i + temp.x][j + temp.y].m_Region == startRoom ||
						dungeon.GetTilemap()[i + temp.x][j + temp.y].m_Region == keycardRoom)
						break;

					if(mTemp && k == dungeon.GetNeighbours().size() - 1) {
						isLockedDoor = true;
						m_Tilemap[(i * m_MaxX) + j] = lockedDoorTile;
						lockedDoor = std::make_pair(i, j);
						break;
					}
				}
			}
			if (isLockedDoor && dungeon.GetTilemap()[i][j].m_Type == connectedFloor)
			{
				for (int k = 0; k < dungeon.GetNeighbours().size(); k++) {
					FVector2i temp = dungeon.GetNeighbours()[k];
					if (dungeon.GetTilemap()[i + temp.x][j + temp.y].m_Region == lockedRoom)
						m_Tilemap[(lockedDoor.first * m_MaxX) + lockedDoor.second] = doorTile;
				}
			}

			if (dungeon.GetTilemap()[i][j].m_Type == tileFloor ||
				dungeon.GetTilemap()[i][j].m_Type == roomWall) {
				m_Tilemap[(i * m_MaxX) + j] = mazeTile;
			}
			else if (dungeon.GetTilemap()[i][j].m_Type == roomFloor)
				m_Tilemap[(i * m_MaxX) + j] = roomTile;
			else if (dungeon.GetTilemap()[i][j].m_Type == connectedFloor)
				m_Tilemap[(i * m_MaxX) + j] = doorTile;
			else
				m_Tilemap[(i * m_MaxX) + j] = wallTIle;


		}
	}
	Vector2D temp = validTiles[validTiles.size() / 2];
	Vector2D temp1 = keycardTiles[keycardTiles.size() / 2];

	//m_Tilemap[(temp.first * m_MaxX) + temp.second] = 0;
	m_Tilemap[(temp1.first * m_MaxX) + temp1.second] = keycardTile;

	if (i == 0)
		m_Tilemap1 = m_Tilemap;
	else if(i == 1)
		m_Tilemap2 = m_Tilemap;
	else if (i == 2)
		m_Tilemap3 = m_Tilemap;
	else if (i == 3)
		m_Tilemap4 = m_Tilemap;
	else if (i == 4)
		m_Tilemap5 = m_Tilemap;
}

void ATileMap::ConnectLevels()
{
	std::map<Vector2D, std::vector<int>> m_ConnectionMap;

	std::vector<std::vector<Vector2D>> changeVector;
	std::vector<std::vector<Vector2D>> targetVector;
	for (int i = 0; i < 5; i++) {
		changeVector.push_back(std::vector<Vector2D>());
		targetVector.push_back(std::vector<Vector2D>());
	}
	std::vector<TArray<int>*> m_MapVector;
	m_MapVector.push_back(&m_Tilemap1);
	m_MapVector.push_back(&m_Tilemap2);
	m_MapVector.push_back(&m_Tilemap3);
	m_MapVector.push_back(&m_Tilemap4);
	m_MapVector.push_back(&m_Tilemap5);


	for (int i = 0; i < m_MapVector.size(); i++) {
		for (int j = i + 1; j < m_MapVector.size(); j++) {
			TArray<int> map1 = *m_MapVector[i];
			TArray<int> map2 = *m_MapVector[j];
			Vector2D temp = std::make_pair(i, j);
			for (int k = 0; k < m_MapVector[i]->Num(); k++) {
				if (map1[k] == roomTile && map2[k] == roomTile)
					m_ConnectionMap[temp].push_back(k);
			}
		}
	}

	std::map<int, int> merged;
	std::unordered_set<int> openLevels;
	for (int i = 0; i < m_MapVector.size(); i++) {
		merged[i] = i;
		openLevels.insert(i);
	}

	while ((openLevels.size() > 1) && (m_ConnectionMap.size() > 0)) {
		Vector2D level;

		int index = rand() % m_ConnectionMap.size();
		int i = 0;
		for (auto const& x : m_ConnectionMap) {
			if (i < index) {
				i++;
				continue;
			}
			level = x.first;
			break;
		}
		int dest = level.first;
		int source = level.second;



		for (int i = 0; i < merged.size(); i++) {
			if (source == merged[i]) {
				merged[i] = merged[dest];
				break;
			}
		}

		std::vector<int> connectors = m_ConnectionMap[level];
		int randConnector = connectors[rand() % connectors.size()];


		changeVector[level.first].push_back(std::make_pair(randConnector, elevatorTile));

		for (int i = 1; i < changeVector.size(); i++) {
			TArray<int> map1 = *m_MapVector[i];
			if (i == level.first || i == level.second)
				continue;
			else if(map1[randConnector] == 1)
				targetVector[i].push_back(std::make_pair(randConnector, 10));
			else if(map1[randConnector] == 2)
				targetVector[i].push_back(std::make_pair(randConnector, 11));
		}
		if (level.second - 1 != level.first)
			targetVector[level.second - 1].push_back(std::make_pair(randConnector, elevatorPassingTile));

		targetVector[level.second].push_back(std::make_pair(randConnector, elevatorStopTile));


		std::unordered_set<int>::const_iterator got = openLevels.find(source);
		if (!(got == openLevels.end()))
			openLevels.erase(got);
		m_ConnectionMap.erase(level);
	}


	for (int i = 0; i < m_MapVector.size(); i++) {
		TArray<int>& map = *m_MapVector[i];
		for(int j = 0; j < changeVector[i].size(); j++)
			map[changeVector[i][j].first] = changeVector[i][j].second;
		for(int j = 0; j < targetVector[i].size(); j++)
			map[targetVector[i][j].first] = targetVector[i][j].second;
	}

}

