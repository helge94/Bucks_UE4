// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include <vector>
#include <algorithm>
#include "CoreMinimal.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FVector2i
{

	GENERATED_USTRUCT_BODY();

	FVector2i() {
		x = 0;
		y = 0;
	}

	FVector2i(int X, int Y) {
		x = X;
		y = Y;
	}

	UPROPERTY()
		int x;

	UPROPERTY()
		int y;

	FVector2i operator*(int other) {
		FVector2i temp;
		temp.x = this->x * other;
		temp.y = this->y * other;
		return temp;
	}
	FVector2i operator+(FVector2i other) {
		FVector2i temp;
		temp.x = this->x + other.x;
		temp.y = this->y + other.y;
		return temp;
	}
	FVector2i operator-() {
		FVector2i temp;
		temp.x = -this->x;
		temp.y = -this->y;
		return temp;
	}
	bool operator==(FVector2i other) {
		if (other.x == this->x && other.y == this->y)
			return true;
		return false;
	}
	void operator+=(FVector2i other) {
		this->x += other.x;
		this->y += other.y;
	}

};

struct Rect {


	Rect();

	Rect(int left, int top, int width, int height) {
		rectLeft = left;
		rectTop = top;
		rectWidth = width;
		rectHeight = height;
	}

	bool contains(int x, int y) const {
		int minX = std::min(rectLeft, static_cast<int>(rectLeft + rectWidth));
		int maxX = std::max(rectLeft, static_cast<int>(rectLeft + rectWidth));
		int minY = std::min(rectTop, static_cast<int>(rectTop + rectHeight));
		int maxY = std::max(rectTop, static_cast<int>(rectTop + rectHeight));
		return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
	}

	bool valueInRange(int value, int min, int max) {
		return (value >= min) && (value <= max);
	}

	bool intersects(Rect other) {

		bool xOverlap = valueInRange(rectLeft, other.rectLeft, other.rectLeft + other.rectWidth - 1) ||
			valueInRange(other.rectLeft, rectLeft, rectLeft + rectWidth - 1);

		bool yOverlap = valueInRange(rectTop, other.rectTop, other.rectTop + other.rectHeight - 1) ||
			valueInRange(other.rectTop, rectTop, rectTop + rectHeight - 1);

		return xOverlap && yOverlap;
	}

	int rectLeft;
	int rectTop;
	int rectWidth;
	int rectHeight;

};

UENUM(BlueprintType)
enum Type {
	roomFloor, m_none, tileFloor, lame, connectionFloor, connectedFloor, roomWall
};
/**
*
*/
class BUCKSALG_API Tile
{
public:
	Tile();
	Tile(FVector2i p);
	~Tile();

	Type m_Type;
	int m_Region;
	FVector2i pos;
	bool m_isRoom = false;
};
