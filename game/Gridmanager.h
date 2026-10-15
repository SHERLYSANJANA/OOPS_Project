#include "Renderer.h"
#include <vector>
using namespace std;
enum class TileType{
    Empty,
    obstacle,
    Player,
    Enemy
};

class Grid{
    //int  tile_size =Renderer::tileSize;
    int height;
    int widht;
    vector<vector<TileType>>grid;;
public:
    Grid(int W,int H);

    bool isOutOfBounds(int x,int y);
    bool isBlocked(int x,int y);
    void setTile(int x,int y ,TileType t);
    TileType getTile(int x,int y);
    int GetWidht();
    int GetHeight();
};