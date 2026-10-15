#include "Gridmanager.h"



class Game{


private:  // this will include all the game data members and classes
    Grid Board; 
    






public:

    Game(int W,int H): Board(W,H ){};

    void Update();
    void render();



};