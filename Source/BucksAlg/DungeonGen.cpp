// Fill out your copyright notice in the Description page of Project Settings.
#include "DungeonGen.h"
#include <stdlib.h>  
#include <crtdbg.h>  
#include <iterator>
#include <time.h>
#include <iostream>
#include <unordered_set>

DungeonGen::DungeonGen(int c, int r, int room, bool multi, int winding) :
	m_Col(c), m_Row(r), m_MultiRoom(multi), m_Winding(winding)
{
	m_Regions = -1;

	m_Neighbours.push_back(FVector2i(0, 1));
	m_Neighbours.push_back(FVector2i(0, -1));
	m_Neighbours.push_back(FVector2i(1, 0));
	m_Neighbours.push_back(FVector2i(-1, 0));

	CreateTilemap();
	for (int i = 0; i < room; i++)
		AddRooms();

	m_AmountOfRooms = m_Regions;

	m_MazeStartRegion = m_Regions + 1;

	for (int i = 1; i < m_Col; i += 2) {
		for (int j = 1; j < m_Row; j += 2) {
			if (m_Tiles[i][j].m_Type != m_none) continue;
			CreateMaze(m_Tiles[i][j]);
		}
	}

	CreRegions();
	RemoveDeadEnds();


}

DungeonGen::~DungeonGen()
{
}

void DungeonGen::RemoveDeadEnds() {

	bool done = false;
	while (!done) {
		done = true;
		std::map<int, Vector2D>::iterator it;
		for (it = m_RemovableTiles.begin(); it != m_RemovableTiles.end(); it) {
			Vector2D tempVec = it->second;
			FVector2i pos = FVector2i(tempVec.first, tempVec.second);

			int exits = 0;
			for (int i = 0; i < m_Neighbours.size(); i++) {
				FVector2i temp = (pos + m_Neighbours[i]);
				if ((m_Tiles[temp.x][temp.y].m_Type != m_none) && (m_Tiles[temp.x][temp.y].m_Type != connectionFloor)) exits++;
			}
			if (exits != 1) {
				it++;
				continue;
			}
			done = false;

			m_Tiles[pos.x][pos.y].m_Type = m_none;
			m_Tiles[pos.x][pos.y].m_Region = -1;
			m_RemovableTiles.erase(it++);
		}

	}
}
void DungeonGen::CreRegions() {
	std::map<Vector2D, std::vector<std::pair<int, int>>> m_MapRegionTiles;
	std::map<Vector2D, std::vector<std::pair<int, int>>> m_MazeTiles;

	for (int ix = 0; ix < m_Tiles.size(); ix++) {
		for (int iy = 0; iy < m_Tiles[ix].size(); iy++) {
			FVector2i pos = FVector2i(ix, iy);
			Vector2D RegionTiles;
			if (m_Tiles[ix][iy].m_Type != m_none) continue;

			std::unordered_set<int> regions;
			for (int i = 0; i < m_Neighbours.size(); i++) {
				FVector2i index = (pos + m_Neighbours[i]);

				Rect worldBounds = Rect(0, 0, m_Row, m_Col);
				if (!worldBounds.contains(index.x, index.y)) continue;

				int region = m_Tiles[index.x][index.y].m_Region;

				if (region != -1) regions.insert(region);

			}
			if (regions.size() < 2) continue;
			int x = *std::next(regions.begin(), 0);
			int y = *std::next(regions.begin(), 1);

			if (x > y) RegionTiles = std::make_pair(y, x);
			else RegionTiles = std::make_pair(x, y);

			m_Tiles[pos.x][pos.y].m_Type = connectionFloor;
			m_MapRegionTiles[RegionTiles].push_back(std::make_pair(pos.x, pos.y));
			if (RegionTiles.second >= m_MazeStartRegion)
				m_MazeTiles[RegionTiles].push_back(std::make_pair(pos.x, pos.y));

		}
	}

	std::map<int, int> merged;
	std::unordered_set<int> openRegions;
	for (int i = 0; i <= m_Regions; i++) {
		merged[i] = i;
		openRegions.insert(i);
	}

	while ((openRegions.size() > 1) && (m_MapRegionTiles.size() > 0)) {
		Vector2D region;

		if (m_MultiRoom) {
			int index = rand() % m_MapRegionTiles.size();
			int i = 0;
			for (auto const& x : m_MapRegionTiles) {
				if (i < index) {
					i++;
					continue;
				}
				region = x.first;
				break;
			}
		}
		else {
			if (m_MazeTiles.size() > 0)
				for (auto const& x : m_MazeTiles) {
					region = x.first;
					m_MazeTiles.erase(region);
					break;
				}
			else
				for (auto const& x : m_MapRegionTiles) {
					region = x.first;
					break;
				}
		}

		int dest = region.first;
		int source = region.second;

		if (m_MultiRoom) {
			for (int i = 0; i < merged.size(); i++) {
				if (source == merged[i]) {
					merged[i] = merged[dest];
					break;
				}
			}
		}
		else {
			if (merged[dest] == merged[source]) {
				m_MapRegionTiles.erase(region);
				continue;
			}
			int merge = merged[source];
			for (int i = 0; i < merged.size(); i++) {
				if (merge == merged[i])
					merged[i] = merged[dest];
			}
		}
		std::vector<Vector2D> connectors = m_MapRegionTiles[region];
		Vector2D randConnector = connectors[rand() % connectors.size()];
		m_Tiles[randConnector.first][randConnector.second].m_Type = connectedFloor;
		m_RemovableTiles[m_RemovableTiles.size()] = randConnector;

		std::unordered_set<int>::const_iterator got = openRegions.find(source);
		if (!(got == openRegions.end()))
			openRegions.erase(got);
		m_MapRegionTiles.erase(region);

	}

}

void DungeonGen::CreateMaze(Tile start) {
	m_Regions++;
	std::vector<Tile> cells;
	FVector2i lastDir = FVector2i(0, 0);
	start.m_Region = m_Regions;
	cells.push_back(start);

	while (cells.size() > 0) {
		Tile cell = cells.back();
		std::vector<FVector2i> adjacentCells;

		for (int i = 0; i < m_Neighbours.size(); i++) {
			if (CanCarve(cell, m_Neighbours[i])) adjacentCells.push_back(m_Neighbours[i]);
		}
		if (adjacentCells.size() > 0) {
			FVector2i dir;
			for (int i = 0; i < adjacentCells.size(); i++) {
				if (adjacentCells[i] == lastDir) {
					int temp = rand() % 100;
					if (temp < m_Winding) dir = lastDir;
					break;
				}
				else if (i == adjacentCells.size() - 1)
					dir = adjacentCells[rand() % adjacentCells.size()];
			}
			cell.pos += dir;
			m_Tiles[cell.pos.y][cell.pos.x].m_Type = tileFloor;
			m_Tiles[cell.pos.y][cell.pos.x].m_Region = m_Regions;
			m_RemovableTiles[m_RemovableTiles.size()] = std::make_pair(cell.pos.y, cell.pos.x);
			cell.pos += dir;
			m_Tiles[cell.pos.y][cell.pos.x].m_Type = tileFloor;
			m_Tiles[cell.pos.y][cell.pos.x].m_Region = m_Regions;
			m_RemovableTiles[m_RemovableTiles.size()] = std::make_pair(cell.pos.y, cell.pos.x);

			cells.push_back(m_Tiles[cell.pos.y][cell.pos.x]);
			lastDir = dir;

		}
		else {
			cells.pop_back();
			lastDir = FVector2i(0, 0);
		}
	}
}


bool DungeonGen::CanCarve(Tile tile, FVector2i dir) {
	FVector2i temp = tile.pos + dir * 3;
	Rect worldBounds = Rect(0, 0, m_Row, m_Col);
	if (!worldBounds.contains(temp.x, temp.y))
		return false;

	temp = tile.pos + dir * 2;

	if (m_Tiles[temp.y][temp.x].m_Type != m_none)
		return false;

	for (int i = 0; i < m_Neighbours.size(); i++) {
		if (m_Neighbours[i] == dir || m_Neighbours[i] == -dir) continue;
		if (m_Tiles[temp.y + m_Neighbours[i].y][temp.x + m_Neighbours[i].x].m_Type != m_none)
			return false;
	}

	return true;
}



void DungeonGen::CreateTilemap() {

	for (int i = 0; i < m_Col; i++) {
		std::vector<Tile> col;
		for (int j = 0; j < m_Row; j++) {
			Tile tile;
			tile.pos = FVector2i(j, i);
			col.push_back(tile);
		}
		m_Tiles.push_back(col);
	}

}

void DungeonGen::AddRooms() {
	int xSize = (rand() % 3 + 2) * 2 + 1, ySize = (rand() % 3 + 2) * 2 + 1;
	int x = rand() % (int)((m_Row - xSize) * 0.5) * 2 + 1;
	int y = rand() % (int)((m_Col - ySize) * 0.5) * 2 + 1;

	if (x + xSize > m_Row - 1 || y + ySize > m_Col - 1)
		return;

	FVector2i size(xSize, ySize);
	Rect room(x, y, size.x, size.y);

	for (int i = 0; i < m_Rooms.size(); i++) {
		if (room.intersects(m_Rooms[i]))
			return;
	}
	m_Regions++;
	for (int j = y; j < (y + ySize); j++) {
		for (int i = x; i < (x + xSize); i++) {
			if (i == x || j == y || i == (x + xSize - 1) || j == (y + ySize - 1))
				m_Tiles[j][i].m_Type = roomWall;
			else
				m_Tiles[j][i].m_Type = roomFloor;

			m_Tiles[j][i].m_Region = m_Regions;
			m_Tiles[j][i].m_isRoom = true;
		}
	}

	m_Rooms.push_back(room);
}
