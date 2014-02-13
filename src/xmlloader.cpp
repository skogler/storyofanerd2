/*------------------------------------------------------------------------/
 *
 * Copyright (c) 2013 David Robin Cvetko
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *-----------------------------------------------------------------------*/

#include <SDL2/SDL.h>
#include <sstream>
#include <cassert>

#include "xmlloader.h"
#include "logging.h"

//monkey monkey
const string LoadedMap::XML_MAP             = "map";
const string LoadedMap::XML_MAP_WIDTH       = "width";
const string LoadedMap::XML_MAP_HEIGHT      = "height";
const string LoadedMap::XML_MAP_TILEWIDTH   = "tilewidth";
const string LoadedMap::XML_MAP_TILEHEIGHT  = "tileheight";

const string LoadedMap::XML_TILESET         = "tileset";
const string LoadedMap::XML_TILESET_NAME    = "name";
const string LoadedMap::XML_TILESET_WIDTH   = "tilewidth";
const string LoadedMap::XML_TILESET_HEIGHT  = "tileheight";
const string LoadedMap::XML_TILESET_SPACING = "spacing";
const string LoadedMap::XML_TILESET_MARGIN  = "margin";

const string LoadedMap::XML_IMAGE           = "image";
const string LoadedMap::XML_IMAGE_SOURCE    = "source";
const string LoadedMap::XML_IMAGE_WIDTH     = "width";
const string LoadedMap::XML_IMAGE_HEIGHT    = "height";

const string LoadedMap::XML_TERRAINTYPE     = "terraintypes";
const string LoadedMap::XML_TERRAIN         = "terrain";
const string LoadedMap::XML_TERRAIN_NAME    = "name";
const string LoadedMap::XML_TERRAIN_TILE    = "tile";
const string LoadedMap::XML_TERRAIN_PROPS   = "properties";
const string LoadedMap::XML_TERRAIN_PROP    = "property";
const string LoadedMap::XML_TERRAIN_PROP_NAME   = "name";
const string LoadedMap::XML_TERRAIN_PROP_VALUE  = "value";

const string LoadedMap::XML_TILE            = "tile";
const string LoadedMap::XML_TILE_ID         = "id";
const string LoadedMap::XML_TILE_TERRAIN    = "terrain";

const string LoadedMap::XML_LAYER           = "layer";
const string LoadedMap::XML_LAYER_NAME      = "name";
const string LoadedMap::XML_LAYER_WIDTH     = "width";
const string LoadedMap::XML_LAYER_HEIGHT    = "height";
const string LoadedMap::XML_LAYER_DATA      = "data";
const string LoadedMap::XML_LAYER_DATA_ENCODING     = "encoding";
const string LoadedMap::XML_LAYER_DATA_COMPRESSION  = "compression";

const string LoadedMap::XML_OBJECTGROUP             = "objectgroup";
const string LoadedMap::XML_OBJECTGROUP_DRAWORDER   = "draworder";
const string LoadedMap::XML_OBJECTGROUP_NAME        = "name";
const string LoadedMap::XML_OBJECTGROUP_WIDTH       = "width";
const string LoadedMap::XML_OBJECTGROUP_HEIGHT      = "height";

const string LoadedMap::XML_OBJECTGROUP_PROPS       = "properties";
const string LoadedMap::XML_OBJECTGROUP_PROP        = "property";
const string LoadedMap::XML_OBJECTGROUP_PROP_NAME   = "name";
const string LoadedMap::XML_OBJECTGROUP_PROP_VALUE  = "value";

const string LoadedMap::XML_OBJECT                  = "object";
const string LoadedMap::XML_OBJECT_NAME             = "name";
const string LoadedMap::XML_OBJECT_X                = "x";
const string LoadedMap::XML_OBJECT_Y                = "y";
const string LoadedMap::XML_OBJECT_WIDTH            = "width";
const string LoadedMap::XML_OBJECT_HEIGHT           = "height";

///////////////////////////////////////////////////////////////////////////

LoadedMap::LoadedMap(const string& filename) :
    m_filename(filename)
{
}

///////////////////////////////////////////////////////////////////////////

LoadedMap::~LoadedMap()
{
}

///////////////////////////////////////////////////////////////////////////

ErrorCode LoadedMap::loadFile()
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadFile start\n");

    int ret = m_doc.LoadFile(m_filename.c_str());

    if(ret != 0)
    {
        return ERROR_OPENING_FILE;
    }

    XMLElement* root_map = m_doc.FirstChildElement(XML_MAP.c_str());
    assert(root_map);
    loadMap(root_map);

    XMLElement* child = root_map->FirstChildElement();
    assert(child);

    while(child != NULL)
    {
        if(child->Name() == XML_TILESET)
        {
            loadTileset(child);
        }
        else if(child->Name() == XML_LAYER)
        {
            loadLayer(child);
        }
        else if(child->Name() == XML_OBJECTGROUP)
        {
            loadObjectGroup(child);
        }

        child = child->NextSiblingElement();
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadFile end\n");

    printMapInformation();
    return OK;
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadMap(XMLElement* element)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadMap start\n");
    stringstream width (element->Attribute(XML_MAP_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_MAP_HEIGHT.c_str()));
    stringstream tilewidth (element->Attribute(XML_MAP_TILEWIDTH.c_str()));
    stringstream tileheight (element->Attribute(XML_MAP_TILEHEIGHT.c_str()));
    width >> m_map.width;
    height >> m_map.height;
    tilewidth >> m_map.tilewidth;
    tileheight >> m_map.tileheight;
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadMap end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTileset(XMLElement* element)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTileset\n");

    assert(element);

    TileSet tileset;
    tileset.name = element->Attribute(XML_TILESET_NAME.c_str());

    stringstream tilewidth (element->Attribute(XML_TILESET_WIDTH.c_str()));
    stringstream tileheight (element->Attribute(XML_TILESET_HEIGHT.c_str()));
    stringstream spacing (element->Attribute(XML_TILESET_SPACING.c_str()));
    stringstream margin (element->Attribute(XML_TILESET_MARGIN.c_str()));

    tilewidth >> tileset.tilewidth;
    tileheight >> tileset.tileheight;
    spacing >> tileset.spacing;
    margin >> tileset.margin;

    XMLElement* image = element->FirstChildElement(XML_IMAGE.c_str());
    assert(image);
    loadImageSource(image, &tileset);

    XMLElement* terrains = element->FirstChildElement(XML_TERRAINTYPE.c_str());
    assert(terrains);
    loadTerrains(terrains, &tileset);

    XMLElement* first_tile = element->FirstChildElement(XML_TILE.c_str());
    assert(first_tile);

    //NOTE: MUST push first because we reference afterwards (terrain
    //pointers)
    m_tilesets.push_back(tileset);
    loadTiles(first_tile, &m_tilesets.back());

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTileset end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadImageSource(XMLElement* element, TileSet* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadImageSource start\n");

    assert(element);
    assert(target);

    target->image.source_image = getAttributeString(element, XML_IMAGE_SOURCE);

    stringstream width (element->Attribute(XML_IMAGE_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_IMAGE_HEIGHT.c_str()));

    width >> target->image.width;
    height >> target->image.height;

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadImageSource end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTerrains(XMLElement* element, TileSet* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTerrains start\n");

    assert(element);
    assert(target);

    XMLElement* terrain = element->FirstChildElement(XML_TERRAIN.c_str());

    Logger.logMessage(LOG_DEBUG, LOG_MAP,
                      "LoadedMap::loadTerrains: Found first terrain\n");

    while(terrain)
    {
        TerrainType parsed_terrain;
        parsed_terrain.name = getAttributeString(terrain, XML_TERRAIN_NAME);

        stringstream tile (terrain->Attribute(XML_TERRAIN_TILE.c_str()));
        tile >> parsed_terrain.tile;

        XMLElement* properties = terrain->FirstChildElement(XML_TERRAIN_PROPS.c_str());

        if(properties)
        {
            loadTerrainProperties(properties, &parsed_terrain);
        }

        target->terraintypes.push_back(parsed_terrain);
        terrain = terrain->NextSiblingElement();
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTerrains end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTerrainProperties(XMLElement* element, TerrainType* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP,
                      "LoadedMap::loadTerrainProperties start\n");

    assert(element);
    assert(target);

    XMLElement* property = element->FirstChildElement(XML_TERRAIN_PROP.c_str());

    while(property != NULL)
    {
        std::pair<string, string> parsed_property;
        parsed_property.first = getAttributeString(property, XML_TERRAIN_PROP_NAME);
        parsed_property.second = getAttributeString(property, XML_TERRAIN_PROP_VALUE);

        target->properties.insert(parsed_property);
        property = property->NextSiblingElement();
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTerrainProperties end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadTiles(XMLElement* element, TileSet* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTiles start\n");

    assert(target);

    while(element != NULL)
    {
        Tile parsed_tile;

        stringstream tile_id (element->Attribute(XML_TILE_ID.c_str()));
        tile_id >> parsed_tile.id;

        string tile_terrains = element->Attribute(XML_TILE_TERRAIN.c_str());
        mapTilesToTerrainPointers(tile_terrains, target, &parsed_tile);

        target->tiles.push_back(parsed_tile);

        element = element->NextSiblingElement();
    }

    Logger.logMessage(LOG_DEBUG, LOG_MAP,
                      "LoadedMap::loadTiles: Loaded %d tiles for %s\n",
                      target->tiles.size(), target->name.c_str());
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadTiles start end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::mapTilesToTerrainPointers(string parsed, TileSet* tset,
        Tile* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP,
                      "LoadedMap::mapTilesToTerrainPointers start\n");

    assert(tset);
    assert(target);

    std::stringstream ss(parsed);

    int parsed_val;
    int cnt = 1;

    while (ss >> parsed_val && cnt <= 4)
    {
        switch(cnt)
        {
            case 1:
                target->terrain_1 = &tset->terraintypes[parsed_val];
                Logger.logMessage(LOG_DEBUG2, LOG_MAP, "Mapping terraintype 1 to %s\n",
                                  target->terrain_1->name.c_str());
                break;

            case 2:
                target->terrain_2 = &tset->terraintypes[parsed_val];
                Logger.logMessage(LOG_DEBUG2, LOG_MAP, "Mapping terraintype 2 to %s\n",
                                  target->terrain_2->name.c_str());
                break;

            case 3:
                target->terrain_3 = &tset->terraintypes[parsed_val];
                Logger.logMessage(LOG_DEBUG2, LOG_MAP, "Mapping terraintype 3 to %s\n",
                                  target->terrain_3->name.c_str());
                break;

            case 4:
                target->terrain_4 = &tset->terraintypes[parsed_val];
                Logger.logMessage(LOG_DEBUG2, LOG_MAP, "Mapping terraintype 4 to %s\n",
                                  target->terrain_4->name.c_str());
                break;
        }

        if (ss.peek() == ',')
        {
            ss.ignore();
        }

        cnt = cnt + 1;
    }

    Logger.logMessage(LOG_STATE, LOG_MAP,
                      "LoadedMap::mapTilesToTerrainPointers end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadLayer(XMLElement* element)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadLayer start\n");

    assert(element);

    Layer parsed_layer;

    parsed_layer.name = getAttributeString(element, XML_LAYER_NAME);

    stringstream width (element->Attribute(XML_LAYER_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_LAYER_HEIGHT.c_str()));

    width >> parsed_layer.width;
    height >> parsed_layer.height;

    XMLElement* data = element->FirstChildElement(XML_LAYER_DATA.c_str());

    if(data != NULL)
    {
        parsed_layer.encoding = getAttributeString(data, XML_LAYER_DATA_ENCODING);

        if(parsed_layer.encoding != "csv")
        {
            parsed_layer.compression = getAttributeString(data, XML_LAYER_DATA_COMPRESSION);
        }

        parsed_layer.data = data->GetText();
    }

    Logger.logMessage(LOG_DEBUG, LOG_MAP,
                      "LoadedMap::loadLayer: Loaded layer: %s\n",
                      parsed_layer.name.c_str());
    m_layers.push_back(parsed_layer);
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadLayer end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjectGroup(XMLElement* element)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadObjectGroup start\n");

    assert(element);

    ObjectGroup parsed_group;

    parsed_group.draworder = getAttributeString(element, XML_OBJECTGROUP_DRAWORDER);
    parsed_group.name = getAttributeString(element, XML_OBJECTGROUP_NAME);

    stringstream width (element->Attribute(XML_OBJECTGROUP_WIDTH.c_str()));
    stringstream height (element->Attribute(XML_OBJECTGROUP_HEIGHT.c_str()));

    width >> parsed_group.width;
    height >> parsed_group.height;

    XMLElement* first_object = element->FirstChildElement(XML_OBJECT.c_str());
    loadObjects(first_object, &parsed_group);

    XMLElement* properties = element->FirstChildElement(
                                 XML_OBJECTGROUP_PROPS.c_str());

    if(properties != NULL)
    {
        loadObjectGroupProperties(properties, &parsed_group);
    }

    m_objectgroups.push_back(parsed_group);

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadObjectGroup end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjects(XMLElement* element, ObjectGroup* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadObjects start\n");

    assert(target);

    while(element != NULL)
    {
        Object parsed_object;

        parsed_object.name = getAttributeString(element, XML_OBJECT_NAME);

        stringstream x (element->Attribute(XML_OBJECT_X.c_str()));
        stringstream y (element->Attribute(XML_OBJECT_Y.c_str()));

        stringstream width (element->Attribute(XML_OBJECT_WIDTH.c_str()));
        stringstream height (element->Attribute(XML_OBJECT_HEIGHT.c_str()));

        x >> parsed_object.bbox.x;
        y >> parsed_object.bbox.y;

        width >> parsed_object.bbox.w;
        height >> parsed_object.bbox.h;

        target->objects.push_back(parsed_object);
        element = element ->NextSiblingElement();
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::loadObjects end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::loadObjectGroupProperties(XMLElement* element,
        ObjectGroup* target)
{
    Logger.logMessage(LOG_STATE, LOG_MAP,
                      "LoadedMap::loadObjectGroupProperties start\n");

    assert(element);
    assert(target);

    XMLElement* property = element->FirstChildElement(XML_OBJECTGROUP_PROP.c_str());

    while(property != NULL)
    {
        std::pair<string, string> parsed_property;
        parsed_property.first = getAttributeString(property, XML_OBJECTGROUP_PROP_NAME);
        parsed_property.second = getAttributeString(property,
                                 XML_OBJECTGROUP_PROP_VALUE);

        target->properties.insert(parsed_property);
        property = property->NextSiblingElement();
    }

    Logger.logMessage(LOG_STATE, LOG_MAP,
                      "LoadedMap::loadObjectGroupProperties end\n");
}

///////////////////////////////////////////////////////////////////////////

string LoadedMap::getAttributeString(XMLElement* element,
                                     const string& attribute_name)
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::getAttributeString start\n");

    assert(element);
    const char* buffer = element->Attribute(attribute_name.c_str());

    if(buffer != NULL)
    {
        return string(buffer);
    }
    else
    {
        Logger.logMessage(LOG_WARNING, LOG_MAP, "Unable to parse attribute %s",
                          attribute_name.c_str());
        return "";
    }

    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::getAttributeString end\n");
}

///////////////////////////////////////////////////////////////////////////

void LoadedMap::printMapInformation()
{
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::printMapInformation start\n");

    Logger.logMessage(LOG_INFO, LOG_MAP, "=== Printing map information... ===\n");
    Logger.logMessage(LOG_INFO, LOG_MAP, "Map stats follow...\n"
                      "Filename: %s\nNumber of tilesets: %d\n"
                      "Number of layers: %d\nNumber of objectgroups: %d\n",
                      m_filename.c_str(), m_tilesets.size(),
                      m_layers.size(), m_objectgroups.size());

    for(uint i = 0; i < m_tilesets.size(); i++)
    {
        Logger.logMessage(LOG_INFO, LOG_MAP, "Tileset information (idx: %d)\n"
                          "Tileset name: %s\n"
                          "Number of tiles: %d\n"
                          "Number of terrains: %u\n",
                          i,
                          m_tilesets.at(i).name.c_str(),
                          m_tilesets.at(i).tiles.size(),
                          m_tilesets.at(i).terraintypes.size());
    }

    for(uint i = 0; i < m_objectgroups.size(); i++)
    {
        Logger.logMessage(LOG_INFO, LOG_MAP, "Objectgroup information (idx %d)\n"
                          "Objectgroup name: %s\n"
                          "Number of properties: %u\n",
                          i,
                          m_objectgroups.at(i).name.c_str(),
                          m_objectgroups.at(i).properties.size());
    }

    Logger.logMessage(LOG_INFO, LOG_MAP, "=== Printing map information end ===\n");
    Logger.logMessage(LOG_STATE, LOG_MAP, "LoadedMap::printMapInformation end\n");
}

///////////////////////////////////////////////////////////////////////////

