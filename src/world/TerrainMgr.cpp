/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (C) 2014-2015 AscEmu Team <http://www.ascemu.org/>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "StdAfx.h"
#include "TerrainMgr.h"

uint16 TerrainHolder::GetAreaFlagWithoutAdtId(float x, float y)
{
    auto tile = this->GetTile(x, y);
    if (tile)
    {
        return tile->m_map.GetArea(x, y);
    }

    return 0;
}

TerrainTile* TerrainHolder::GetTile(float x, float y)
{
    int32 tx = (int32)(32 - (x / TERRAIN_TILE_SIZE));
    int32 ty = (int32)(32 - (y / TERRAIN_TILE_SIZE));

    return GetTile(tx, ty);
}

TerrainTile::~TerrainTile()
{
    m_parent->m_tiles[m_tx][m_ty] = NULL;
}

TerrainTile::TerrainTile(TerrainHolder* parent, uint32 mapid, int32 x, int32 y)
{
    m_parent = parent;
    m_mapid = mapid;
    m_tx = x;
    m_ty = y;
    ++m_refs;
}

float TileMap::GetHeightB(float x, float y, int x_int, int y_int)
{
    int32 a, b, c;
    uint8* V9_h1_ptr = &m_heightMap9B[x_int * 128 + x_int + y_int];
    if (x + y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h2 = V9_h1_ptr[129];
            int32 h5 = 2 * m_heightMap8B[x_int * 128 + y_int];
            a = h2 - h1;
            b = h5 - h1 - h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h3 = V9_h1_ptr[1];
            int32 h5 = 2 * m_heightMap8B[x_int * 128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            int32 h2 = V9_h1_ptr[129];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_heightMap8B[x_int * 128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            int32 h3 = V9_h1_ptr[1];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_heightMap8B[x_int * 128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }

    // Calculate height
    return (float)((a * x) + (b * y) + c) * m_heightMapMult + m_tileHeight;
}

float TileMap::GetHeightS(float x, float y, int x_int, int y_int)
{
    int32 a, b, c;
    uint16* V9_h1_ptr = &m_heightMap9S[x_int * 128 + x_int + y_int];
    if (x + y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h2 = V9_h1_ptr[129];
            int32 h5 = 2 * m_heightMap8S[x_int * 128 + y_int];
            a = h2 - h1;
            b = h5 - h1 - h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            int32 h1 = V9_h1_ptr[0];
            int32 h3 = V9_h1_ptr[1];
            int32 h5 = 2 * m_heightMap8S[x_int * 128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            int32 h2 = V9_h1_ptr[129];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_heightMap8S[x_int * 128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            int32 h3 = V9_h1_ptr[1];
            int32 h4 = V9_h1_ptr[130];
            int32 h5 = 2 * m_heightMap8S[x_int * 128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }

    // Calculate height
    return (float)((a * x) + (b * y) + c) * m_heightMapMult + m_tileHeight;
}

float TileMap::GetHeightF(float x, float y, int x_int, int y_int)
{
    float a, b, c;
    // Select triangle:
    if (x + y < 1)
    {
        if (x > y)
        {
            // 1 triangle (h1, h2, h5 points)
            float h1 = m_heightMap9F[(x_int)* 129 + y_int];
            float h2 = m_heightMap9F[(x_int + 1) * 129 + y_int];
            float h5 = 2 * m_heightMap8F[x_int * 128 + y_int];
            a = h2 - h1;
            b = h5 - h1 - h2;
            c = h1;
        }
        else
        {
            // 2 triangle (h1, h3, h5 points)
            float h1 = m_heightMap9F[x_int * 129 + y_int];
            float h3 = m_heightMap9F[x_int * 129 + y_int + 1];
            float h5 = 2 * m_heightMap8F[x_int * 128 + y_int];
            a = h5 - h1 - h3;
            b = h3 - h1;
            c = h1;
        }
    }
    else
    {
        if (x > y)
        {
            // 3 triangle (h2, h4, h5 points)
            float h2 = m_heightMap9F[(x_int + 1) * 129 + y_int];
            float h4 = m_heightMap9F[(x_int + 1) * 129 + y_int + 1];
            float h5 = 2 * m_heightMap8F[x_int * 128 + y_int];
            a = h2 + h4 - h5;
            b = h4 - h2;
            c = h5 - h4;
        }
        else
        {
            // 4 triangle (h3, h4, h5 points)
            float h3 = m_heightMap9F[(x_int)* 129 + y_int + 1];
            float h4 = m_heightMap9F[(x_int + 1) * 129 + y_int + 1];
            float h5 = 2 * m_heightMap8F[x_int * 128 + y_int];
            a = h4 - h3;
            b = h3 + h4 - h5;
            c = h5 - h4;
        }
    }
    // Calculate height
    return a * x + b * y + c;
}

float TileMap::GetHeight(float x, float y)
{
    if (m_heightMap9F == NULL)
        return m_tileHeight;

    x = TERRAIN_MAP_RESOLUTION * (32 - x / TERRAIN_TILE_SIZE);
    y = TERRAIN_MAP_RESOLUTION * (32 - y / TERRAIN_TILE_SIZE);

    int x_int = (int)x;
    int y_int = (int)y;
    x -= x_int;
    y -= y_int;
    x_int &= (TERRAIN_MAP_RESOLUTION - 1);
    y_int &= (TERRAIN_MAP_RESOLUTION - 1);

    if (m_heightMapFlags & MAP_HEIGHT_AS_INT16)
        return GetHeightS(x, y, x_int, y_int);
    else if (m_heightMapFlags & MAP_HEIGHT_AS_INT8)
        return GetHeightB(x, y, x_int, y_int);
    return GetHeightF(x, y, x_int, y_int);
}

const bool TerrainHolder::GetAreaInfo(float x, float y, float z, uint32 &mogp_flags, int32 &adt_id, int32 &root_id, int32 &group_id)
{
    float vmap_z = z;
    auto vmap_manager = VMAP::VMapFactory::createOrGetVMapManager();
    if (vmap_manager->getAreaInfo(m_mapid, x, y, vmap_z, mogp_flags, adt_id, root_id, group_id))
    {
        if (auto tile = this->GetTile(x, y))
        {
            float map_height = tile->m_map.GetHeight(x, y);
            if (z + 2.0f > map_height && map_height > vmap_z)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

void TileMap::Load(char* filename)
{
    sLog.Debug("Terrain", "Loading %s", filename);
    FILE* f = fopen(filename, "rb");

    if (f == NULL)
    {
        sLog.Error("Terrain", "%s does not exist", filename);
        return;
    }

    TileMapHeader header;

    fread(&header, 1, sizeof(header), f);

    if (header.buildMagic != 12340)  //wow version
    {
        sLog.Error("Terrain", "%s: from incorrect client (you: %u us: %u)", filename, header.buildMagic, 12340);
        fclose(f);
        return;
    }


    if (header.areaMapOffset != 0)
        LoadAreaData(f, header);

    if (header.heightMapOffset != 0)
        LoadHeightData(f, header);

    if (header.liquidMapOffset != 0)
        LoadLiquidData(f, header);

    fclose(f);
}

void TileMap::LoadLiquidData(FILE* f, TileMapHeader & header)
{
    TileMapLiquidHeader liquidHeader;

    if (fseek(f, header.areaMapOffset, SEEK_SET) != 0)
        return;

    if (fread(&liquidHeader, sizeof(liquidHeader), 1, f) != 1)
        return;

    m_defaultLiquidType = liquidHeader.liquidType;
    m_liquidLevel = liquidHeader.liquidLevel;
    m_liquidOffX = liquidHeader.offsetX;
    m_liquidOffY = liquidHeader.offsetY;
    m_liquidWidth = liquidHeader.width;
    m_liquidHeight = liquidHeader.height;

    if (!(liquidHeader.flags & MAP_LIQUID_NO_TYPE))
    {
        m_liquidType = new uint8[16 * 16];
        if (fread(m_liquidType, sizeof(uint8), 16 * 16, f) != 16 * 16)
            return;
    }

    if (!(liquidHeader.flags & MAP_LIQUID_NO_HEIGHT))
    {
        m_liquidMap = new float[m_liquidWidth * m_liquidHeight];
        if (fread(m_liquidMap, sizeof(float), m_liquidWidth * m_liquidHeight, f) != 16 * 16)
            return;
    }
}

void TileMap::LoadHeightData(FILE* f, TileMapHeader & header)
{
    TileMapHeightHeader mapHeader;

    if (fseek(f, header.areaMapOffset, SEEK_SET) != 0)
        return;

    if (fread(&mapHeader, sizeof(mapHeader), 1, f) != 1)
        return;

    m_tileHeight = mapHeader.gridHeight;
    m_heightMapFlags = mapHeader.flags;

    if (m_heightMapFlags & MAP_HEIGHT_AS_INT16)
    {
        m_heightMapMult = (mapHeader.gridMaxHeight - mapHeader.gridHeight) / 65535;

        m_heightMap9S = new uint16[129 * 129];
        m_heightMap8S = new uint16[128 * 128];
        if (fread(m_heightMap9S, sizeof(uint16), 129 * 129, f) != 129 * 129 ||
            fread(m_heightMap8S, sizeof(uint16), 128 * 128, f) != 128 * 128)
            return;
    }
    else if (m_heightMapFlags & MAP_HEIGHT_AS_INT8)
    {
        m_heightMapMult = (mapHeader.gridMaxHeight - mapHeader.gridHeight) / 255;

        m_heightMap9B = new uint8[129 * 129];
        m_heightMap8B = new uint8[128 * 128];
        if (fread(m_heightMap9B, sizeof(uint8), 129 * 129, f) != 129 * 129 ||
            fread(m_heightMap8B, sizeof(uint8), 128 * 128, f) != 128 * 128)
            return;
    }
    else
    {
        m_heightMap9F = new float[129 * 129];
        m_heightMap8F = new float[128 * 128];
        if (fread(m_heightMap9F, sizeof(float), 129 * 129, f) != 129 * 129 ||
            fread(m_heightMap8F, sizeof(float), 128 * 128, f) != 128 * 128)
            return;
    }
}

void TileMap::LoadAreaData(FILE* f, TileMapHeader & header)
{
    TileMapAreaHeader areaHeader;

    if (fseek(f, header.areaMapOffset, SEEK_SET) != 0)
        return;

    if (fread(&areaHeader, sizeof(areaHeader), 1, f) != 1)
        return;

    m_area = areaHeader.gridArea;
    if (!(areaHeader.flags & MAP_AREA_NO_AREA))
    {
        m_areaMap = new uint16[16 * 16];
        if (fread(m_areaMap, sizeof(uint16), 16 * 16, f) != 16 * 16)
            return;
    }
}

float TileMap::GetLiquidHeight(float x, float y)
{
    if (m_liquidMap == NULL)
        return m_liquidLevel;

    x = TERRAIN_MAP_RESOLUTION * (32 - x / TERRAIN_TILE_SIZE);
    y = TERRAIN_MAP_RESOLUTION * (32 - y / TERRAIN_TILE_SIZE);

    int cx_int = ((int)x & (TERRAIN_MAP_RESOLUTION - 1)) - m_liquidOffX;
    int cy_int = ((int)y & (TERRAIN_MAP_RESOLUTION - 1)) - m_liquidOffY;

    if (cx_int < 0 || cx_int >= m_liquidHeight)
        return TERRAIN_INVALID_HEIGHT;

    if (cy_int < 0 || cy_int >= m_liquidWidth)
        return TERRAIN_INVALID_HEIGHT;

    return m_liquidMap[cx_int * m_liquidWidth + cy_int];
}

uint8 TileMap::GetLiquidType(float x, float y)
{
    if (m_liquidType == NULL)
        return (uint8)m_defaultLiquidType;

    x = 16 * (32 - x / TERRAIN_TILE_SIZE);
    y = 16 * (32 - y / TERRAIN_TILE_SIZE);
    int lx = (int)x & 15;
    int ly = (int)y & 15;
    return m_liquidType[lx * 16 + ly];
}

uint32 TileMap::GetArea(float x, float y)
{
    if (m_areaMap == NULL)
        return m_area;

    x = 16 * (32 - x / TERRAIN_TILE_SIZE);
    y = 16 * (32 - y / TERRAIN_TILE_SIZE);
    int lx = (int)x & 15;
    int ly = (int)y & 15;
    return m_areaMap[lx * 16 + ly];
}
