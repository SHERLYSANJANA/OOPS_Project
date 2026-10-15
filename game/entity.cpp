#include "entity.h"


//entity
Entity:: Entity(int x, int y ,TileType Entitytype,Grid &board){
    type =Entitytype;
    this->x = x;
    this->y = y;
    board.setTile(x,y,Entitytype);

}
Entity::~Entity() {};

int Entity:: getX(){
    return x;
};
int Entity::getY(){
    return y;
};
void Entity::SetPos(int newx, int newy,Grid &board){
    
    bool b = board.setTile(newx,newy,type);
    if(b){
        board.empty(x,y);
        x = newx;
        y = newy;
    }
   
};
TileType Entity::get_type() const{
    return type;
};



//------colored_entity--------


colored_entity:: colored_entity(int x1, int y1,TileType type ,int r, int g ,int b ,int a,Grid &board): Entity(x1, y1,type,board){
    this->r = (float)(r%256)/255.0f;
    this->g = (float)(g%256)/255.0f;
    this->b = (float)(b%256)/255.0f;
    this->a = (float)(a%256)/255.0f;
}; 
void colored_entity::render(float TileSize,Renderer &renderer){
    renderer.draw_colored_rect(Entity::getX(),Entity::getY(),r,g,b,a);
};


//---------textured_entity--------
textured_entity::textured_entity(int x1, int y1,TileType type ,texture t,Grid &board):Entity(x1, y1,type,board){
    tex = t;
}; 
void textured_entity::render(float TileSize,Renderer &renderer){
    renderer.draw_textured_rect(Entity::getX(),Entity::getY(),tex);
};

