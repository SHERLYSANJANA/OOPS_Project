#include "Gridmanager.h"    
    
    
    Grid::Grid(int W,int H){
        widht = W;
        height = H;
        grid = vector<vector<TileType>>(widht,vector<TileType>(height,TileType::Empty));
    }
    
    
    bool Grid::isOutOfBounds(int x,int y){
        if(x>=widht || y>=height ||x<0 ||y<0){
            return true;
        }
        return false;
    };
    bool Grid::isBlocked(int x,int y){
        if(!isOutOfBounds(x,y)&& grid[x][y] == TileType::Empty){
            return false;
        }
        return true;
    };
    bool Grid::setTile(int x,int y ,TileType t){
        if(!isOutOfBounds(x,y) && !isBlocked(x,y)){
            grid[x][y] = t;
            return true;
        }
        return false;
    };
    TileType Grid::getTile(int x,int y){
        if(!isOutOfBounds(x,y)){
            return grid[x][y];
        }
         
        return TileType::obstacle; // FIXED: index 2 was Player, not obstacle

        
    };
    void Grid::empty(int x,int y ){
        if(!isOutOfBounds(x,y)){
            grid[x][y] = TileType::Empty;
        }
    }
    int Grid::GetWidht(){
        return widht;
    };
    int Grid::GetHeight(){
        return height;
    };