#pragma once
#include "Gridmanager.h"
#include "Renderer.h"



class Entity{
    TileType type;
    int x,y; 

public:

    Entity(int x, int y ,TileType Entitytype,Grid &board );
    virtual ~Entity();   //dont get confused it just makes sures that derived objects also get cleaned. 
    virtual void render(float tileSize,Renderer &r) =0; //this will make sure that every derived entity must know how to render itself...
    int getX();
    int getY();
    void SetPos(int newx, int newy,Grid &board);
    TileType get_type() const;
};


//---------colored_rectangle_entity--------------


class colored_entity : public Entity{
    float r,g,b,a;
public:
    colored_entity(int x, int y,TileType type ,int r, int g ,int b ,int a,Grid &board); 
    void render(float TileSize,Renderer &r);
};


//------------textured_entity---------------------

class textured_entity : public Entity{
    texture tex;
public:
    textured_entity(int x, int y,TileType type ,texture t,Grid &board); 
    void render(float TileSize,Renderer &r);
};


