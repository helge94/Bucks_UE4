// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

Tile::Tile() {
	m_Type = m_none;
	m_Region = -1;
}
Tile::Tile(FVector2i p)
{
	pos = p;
	m_Type = m_none;
	m_Region = -1;
}

Tile::~Tile()
{
}

