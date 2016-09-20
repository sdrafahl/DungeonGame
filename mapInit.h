#include "binheap.h"
typedef struct {
    int topLeft[2];
    int topright[2];
    int bottomLeft[2];
    int bottomRight[2];
}Room;

typedef struct {
    char grid[21][80];
    char distanceGrid[21][80];
    unsigned char hardness[21][80];
    Room rooms[100];
    int numOfRooms;
    int pcX;
    int pcY;
}Map;
typedef struct corridor_path corridor_path_t;
int initMap(void);
void printGrid(void);
int saveGame(void);
int loadGame(void);