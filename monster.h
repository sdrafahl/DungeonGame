void initMonsterLib(Map *map);
void MonsterInit(Map *map,);
Monster getMonster(yl,xl);
 int moveUp(Monster *mon);
 int moveDown(Monster *mon);
 int moveRight(Monster *mon);
 int moveLeft(Monster *mon);
 int moveTopRight(Monster *mon);
 int moveTopLeft(Monster *mon);
 int moveBottomLeft(Monster *mon);
 int moveBottomRight(Monster *mon);
 int isIntelegent(Monster *mon);
 int isTelapathic(Monster *mon);
 int canTunnle(Monster *mon);
 int isErratic(Monster *mon);
int hasMonster(int xl, int yl);
typedef struct Monster;