#pragma once

#include "GeometryData.h"

void GenerateTriangle(GeometryData *putDataHere);
void GenerateBox(GeometryData *putDataHere);
void GenerateCircle(GeometryData *putDataHere);

void InitializeGeometry(unsigned int programId, GeometryData *initThis);