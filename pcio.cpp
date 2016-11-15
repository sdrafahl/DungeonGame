
#include <ncurses.h>
#include "gameMap.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define UP_STAIRS '<'
#define DOWN_STAIRS '>'
#define QUIT 'Q'

static int upLeft(void);
static int up(void);
static int left(void);
static int bottomLeft(void);
static int bottom(void);
static int bottomRight(void);
static int right(void);
static int upRight(void);
static int upStairs(void);
static int downStairs(void);
static void clearData();
static void displayEnemyStatus(int scale);
static void displayInventoryStatus(unsigned int selection);
static string convertType(int tyi);
static void inspectItem(int slot);
/*Global Data*/
char ch; /*command*/
Monster *pc;
//WINDOW *temppoint;
/*******************/


int performPCMove(Monster *pci){    
    int scale=0;
    int done = 1;
    int escape=0;
    pc = pci;
    while(done){
        printGrid();
        //wrefresh(window);
        while(1){
        ch = getch(); 
        if(ch){
            break;
        }
        }
        switch(ch){
            
            case '7' : case 'y': /*Up Left*/
                done=upLeft();
                break;
             
           case '8' : case 'k': /*Up*/
                done=up();
                break;
            
            case 'u': case '9' :
                done = upRight(); /*Up Right*/
                break;
            case 'l' :  case '6' : /*Right*/
              done = right();
                break;
           case 'n': case '3' : /*Bottom Right*/
           done = bottomRight();
                break;
            case 'j': case '2': /*Bottom*/
                done = bottom();
                break;
           case 'b':  case '1': /*Bottom Left*/
                done = bottomLeft();
                break;
          case 'h':  case '4' : /*Left*/
                done = left();
                break;
            case '>' :
                done = upStairs();
                break;
            case '<':
                done =downStairs();
                break;
           case ' ' : case '5' :
                done =0;
                break;
            case 'm':
                while(!escape){
                clear();
                refresh();
                displayEnemyStatus(scale);

                int32_t userCommand = getch();
                escape=1;
                if(userCommand==258){ /*Down List*/
                    scale++;
                    escape=0;
                }
                if(userCommand==259){ /*Up Arrow*/
                    escape=0;
                    scale--;
                }
                if(userCommand==27){
                    clear();
                    printw("Loading Buffer");
                    
                    }
                }
                escape=0;
                break;
            case QUIT :
                return 1;
                break;
            case 'i':
                    int selection = 0;
                    int escape = 0;
                    int size = m->thePlayer->inventory.size();
                        clear();
                        refresh();
                    while(!escape){
                        
                        int convert = (unsigned int)selection;
                        displayInventoryStatus(convert);
                        int32_t userCommand = getch();
                        if(userCommand==27){
                            clear();
                            escape=1;
                        }
                        if(userCommand==259){ /*Up Arrow*/
                            selection--;
                            if(selection<0){
                                selection=size-1;
                            }
                        }
                        if(userCommand==258){ /*Up Arrow*/
                            selection++;
                            if(selection>size-1){
                                selection=0;
                            }
                        }
                        if(userCommand==73){ /*Inspect Item*/
                            inspectItem(selection);
                        }
                    }
                break;
        }
    }
    return 0;
}
static string prettyDescription(string desc){
    std::stringstream stream;
    stream.str(desc);
    string temp;
    std::stringstream newStream;
    int counter=0;
    while(stream >> temp){
        counter+= temp.size();
        if(counter>60){
            counter=0;
            newStream << endl;
        }
        newStream << temp;
        newStream << " ";
    }
    return newStream.str();
}

static void inspectItem(int slot){
    clear();
    refresh();
    std::stringstream stream;
    stream << m->thePlayer->inventory.at(slot).name << endl << 
    prettyDescription(m->thePlayer->inventory.at(slot).desc) << 
    endl << convertType(m->thePlayer->inventory.at(slot).type);
    mvaddstr(0,0,stream.str().c_str());
    string question = "Would you like to equip, y or n, entering neither will result in no";
    mvaddstr(1,0,question.c_str());
    unsigned int answer = getch();
    if(answer==89 || answer==121){
        Item temp = m->thePlayer->inventory.at(slot);
        m->thePlayer->inventory.erase(m->thePlayer->inventory.begin()+slot);
        equip(temp);
    }
    
}



static void displayInventoryStatus(unsigned int selection){
    init_pair(5,COLOR_GREEN,COLOR_BLACK);
    init_pair(0,COLOR_WHITE,COLOR_BLACK);
    unsigned int inc;
    for(inc=0;inc<m->thePlayer->inventory.size();inc++){
        Item tem = m->thePlayer->inventory.at(inc);
        std::ostringstream stream;
        stream << "ITEM: " << tem.name << " TYPE: " <<  convertType(tem.type) << " DAMAGE: " << prettyDescription(tem.dam.getDescription()) << " VALUE: " << tem.value;
        if(selection==inc){
            attron(COLOR_PAIR(5));
        }else{
            attron(COLOR_PAIR(0));
        }
        mvaddstr(inc,0,stream.str().c_str());
        if(selection==inc){
             attroff(COLOR_PAIR(5));
        }else{
            attroff(COLOR_PAIR(0));
        }
    }                
}
static string convertType(int tyi){
    switch(tyi){
        case 0:
            return "LIGHT";
            break;
        case 1:
            return "RING";
            break;
        case 2:
            return "WEAPON";
            break;
        case 3:
            return "OFFHAND";
            break;
        case 4:
            return "RANGED";
            break;
        case 5:
            return "ARMOR";
            break;
        case 6:
            return "HELMET";
            break;
        case 7:
            return "CLOAK";
            break;
        case 8:
            return "GLOVES";
            break;
        case 9:
            return "BOOTS";
            break;
        case 10:
            return "AMULET";
            break;
        case 11:
            return "SCROLL";
            break;
        case 12:
            return "BOOK";
            break;
        case 13:
            return "FLASK";
            break;
        case 14:
            return "GOLD";
            break;
        case 15:
            return "AMMUNITION";
            break;
        case 16:
            return "FOOD";
            break;
        case 17: 
            return "WAND";
            break;
        case 18:
            return "CONTAINER";
            break;
    }
    return " ";
}
static int upLeft(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y-1][x-1])
    {
        return moveTopLeft(pc);
    }
    return 1;
}
static int up(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y-1][x])
    {
       return moveUp(pc);
    }
    return 1;
}
static int upRight(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y-1][x+1])
    {
        return moveTopRight(pc);         
    }
    return 1;
}
static int right(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y][x+1])
    {
        return moveRight(pc);        
    }
    return 1;
}
static int bottomRight(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y+1][x+1])
    {
        return moveBottomRight(pc);         
    }
    return 1;
}
static int bottom(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y+1][x])
    {
        return moveDown(pc);         
    }
    return 1;
}
static int bottomLeft(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y+1][x-1])
    {
        return moveBottomLeft(pc);         
    }
    return 1;
}
static int left(void){
   int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(!(*m).hardness[y][x-1])
    {
        return moveLeft(pc);         
    }
    return 1;
}
static int upStairs(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(m->grid[y][x] == '>' ){
        clearData();
        reInitMap(NUMBER_OF_MONSTERS);
        playGame();
        return 0;
    }
    return 1;
}
static int downStairs(void){
    int x = m->thePlayer->xloc;
    int y = m->thePlayer->yloc;
    if(m->grid[y][x] == '<' ){
        clearData();
        reInitMap(NUMBER_OF_MONSTERS);
        playGame();
        return 0;
    }
    return 1;
}
static void clearData(){
    while(!binheap_is_empty(&heap)){
        deconstructor((Monster*)binheap_remove_min(&heap));
    }
    int x,y;for(x=0;x<21;x++){
        for(y=0;y<80;y++){
            m->distanceGrid[x][y].distance=0;
            m->grid[x][y]='\0';
            monsterArray[x][y]=NULL;        
        }
    }
}

static void displayEnemyStatus(int scale){
    Monster* monsters[NUMBER_OF_MONSTERS];
    Monster *tem;
    int numOfMon = heap.size;
    int x;
    for(x=0;x<numOfMon;x++){
         tem = (Monster*)binheap_remove_min(&heap);
         if(tem->alive){
             monsters[x]=tem;
         }else{
             x--;
             numOfMon--;
         }
    }
    int ui;
    for(ui=0;ui<numOfMon;ui++){
        binheap_insert(&heap, monsters[ui]);
    }
    int cou;
    for(cou=0+scale;cou<numOfMon && cou<11+scale;cou++){
        if(cou>0 && cou<NUMBER_OF_MONSTERS){
        Monster *mo; 
        mo = monsters[cou];
        if(mo){
        if(!mo->thePlayer){
        char string[9];
        char string2[7];
        
        char temp = mo->symbol;
        
        string[0] = temp;
        string[1] = ' ';
        
        int xtemppc = m->thePlayer->xloc;
        int ytemppc = m->thePlayer->yloc;
        
        int xtempmon = mo->xloc;
        int ytempmon = mo->yloc;
        int xval = xtempmon - xtemppc;
        if(xval<0){
            xval=xval*-1;
        } 
        int yval = ytempmon-ytemppc;
        if(yval<0){
            yval=yval*-1;
        }
        if(xtemppc<xtempmon){
            string[2]='E';
            string[3]='A';
            string[4]='S';
            string[5]='T';
            string[6]=':';
        }
        if(xtemppc>xtempmon){
            string[2]='W';
            string[3]='E';
            string[4]='S';
            string[5]='T';
            string[6]=':';
        }
        if(xtemppc==xtempmon){
            string[2]=' ';
            string[3]=' ';
            string[4]=' ';
            string[5]=' ';
            string[6]=' ';
            string[7]=' ';
            string[8]=' ';
        }
        string[7]=' ';
        if(ytemppc<ytempmon){
            string2[0]='N';
            string2[1]='O';
            string2[2]='R';
            string2[3]='T';
            string2[4]='H';
            string2[5]=':';
        }
        if(ytemppc>ytempmon){
            string2[0]='S';
            string2[1]='O';
            string2[2]='U';
            string2[3]='T';
            string2[4]='H';
            string2[5]=':';
        }
        printw("%s\n"," ");
        printw("%s",string);
        printw("%i",xval);
        printw("  %s",string2);
        printw("%i\n",yval);
        }
        }
        }
    }
}

