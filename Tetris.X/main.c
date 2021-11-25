//=============================================================================
// Filename: MAIN.C
//=============================================================================
// Authors:   	VANNIER Marie-Laure -- LAPERT Grégoire
// Company:  	ECE Paris
// Revision:	1.00
// Date:   		28/11/2021
//=============================================================================
// Compiled using MPLAB-XC8 
//=============================================================================
//
// 							Tetris Game 
//
//=============================================================================


//---------------------------	Include		-----------------------------------

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "glcd_library/glcd.h"
#include "glcd_library/mcu.h"

//---------------------------	CONFIG		-----------------------------------

#pragma config FOSC = HS
#pragma config LVP = OFF

//GLCD SCREEN 128X64
//ATTENTION bit focntionne que en global ou static et pas de tableau 

//enum state{OFF,ON};//OFF=0 ON=1
enum orientation{UP,DOWN,LEFT,RIGHT};// 0 1 2 3

//position in the grid
typedef struct pos{
    uint8_t x;//column between 0 and 17
    uint8_t y;//line between 0 and 11
}t_pos;

//structure for the shape
typedef struct shape{
    t_pos index[4]; //x between 0 and 17 / y between 0 and 11 in the grid 
    enum orientation orientation; //1,2,3 ou 4
}t_shape;

//-------------------------	Global Variable	------------------------------------

//store the state of the game. 0 = free box and 1 = busy box 
char array_data[18][12]; //18 columns 12 lines
//array of all the shapes of the game 
t_shape all_shapes [20]; 
uint8_t score=0;

//---------------------------	FUNCTIONS		--------------------------------

//display the grid on the GLCD
void display_grid(){
    for (uint8_t i=0;i<128;i++){
        for(uint8_t j=0;j<64;j++){
            if ((j>=0 && j<=1) || (j>=62 && j<=63)) //bordure haut et bas 
                glcd_PlotPixel(i,j,1);
            else if (((i>=0 && i<=1) || ( i>=126 && i<=127)))//bordure gauche droite
                glcd_PlotPixel(i,j,1);
            else if (i>=91 && i<=92 ) //limite grille droite
                glcd_PlotPixel(i,j,1);
            
            //socle 
            if (i>=2 && i<=90 && j==61)
                glcd_PlotPixel(i,j,1);
            
            //colonnes
            if ( ((i==6) || (i==11) || (i==16) || (i==21) || (i==26) || (i==31) || (i==36) || (i==41) || (i==46) || (i==51) || (i==56) || (i==61) || (i==66) || (i==71) || (i==76) || (i==81) || (i==86)) )
                glcd_PlotPixel(i,j,1);
            
            //ligne
            if ( ((j==6) || (j==11) || (j==16) || (j==21) || (j==26) || (j==31) || (j==36) || (j==41) || (j==46) || (j==51) || (j==56)) && (i<92))
                glcd_PlotPixel(i,j,1);
        }
    }
}

/*void update_display(){
    
    uint8_t x,y;
    for(uint8_t i=0;i<17;i++){
        for(uint8_t j=0;j<11;j++){
            switch (i){
                case 0 : 
                    x=2;
                    break;
                case 1 : 
                    x=7;
                    break;
                case 2 : 
                    x=12;
                    break;
                case 3 : 
                    x=17;
                    break;
                case 4 : 
                    x=22;
                    break;
                case 5 : 
                    x=27;
                    break;
                case 6 : 
                    x=32;
                    break;
                case 7 : 
                    x=37;
                    break;
                case 8 : 
                    x=42;
                    break;
                case 9 : 
                    x=47;
                    break;
                case 10 : 
                    x=52;
                    break;
                case 11 : 
                    x=57;
                    break;
                case 12 : 
                    x=62;
                    break;
                case 13 : 
                    x=67;
                    break;
                case 14 : 
                    x=72;
                    break;
                case 15 : 
                    x=77;
                    break;
                case 16 : 
                    x=82;
                    break;
                case 17 : 
                    x=87;
                    break;
                default : 
                    break;
            }
            
            switch (y){
                case 0 : 
                    y=2;
                    break;
                case 1 : 
                    y=7;
                    break;
                case 2 : 
                    y=12;
                    break;
                case 3 : 
                    y=17;
                    break;
                case 4 : 
                    y=22;
                    break;
                case 5 : 
                    y=27;
                    break;
                case 6 : 
                    y=32;
                    break;
                case 7 : 
                    y=37;
                    break;
                case 8 : 
                    y=42;
                    break;
                case 9 : 
                    y=47;
                    break;
                case 10 : 
                    y=52;
                    break;
                case 11 : 
                    y=57;
                    break;
                default : 
                    break;
            }
            
        
        
        if(array_data[i][j]=='1'){
            //display the current box of the shape in the grid 
            for(uint8_t m=x;m<x+4;m++){
                for(uint8_t n=y;n<y+4;n++){
                    glcd_PlotPixel(m,n,1);
                }
            }
        }
        else{
            //no display  
            for(uint8_t m=x;m<x+4;m++){
                for(uint8_t n=y;n<y+4;n++){
                    glcd_PlotPixel(m,n,0);
                }
            }
        }
      }  
    }
}*/

//update the dsplay of all the shapes on the grid from the array "all_shapes"
//operation = 1 --> display the shapes on the grid 
//operation = 0 --> erase the shape from the grid 
void update_display_shapes(char operation){
    
    uint8_t x,y,i=0;

    while (all_shapes[i].index[0].x != 99){ //for all elem of all_shapes array
        for(uint8_t j=0;j<4;j++){ //for all box of the current shape 
            //switch according to the value of x and y. Each one corresponds to an int = top left of the box in the grid 
            switch (all_shapes[i].index[j].x){
                case 0 : 
                    x=2;
                    break;
                case 1 : 
                    x=7;
                    break;
                case 2 : 
                    x=12;
                    break;
                case 3 : 
                    x=17;
                    break;
                case 4 : 
                    x=22;
                    break;
                case 5 : 
                    x=27;
                    break;
                case 6 : 
                    x=32;
                    break;
                case 7 : 
                    x=37;
                    break;
                case 8 : 
                    x=42;
                    break;
                case 9 : 
                    x=47;
                    break;
                case 10 : 
                    x=52;
                    break;
                case 11 : 
                    x=57;
                    break;
                case 12 : 
                    x=62;
                    break;
                case 13 : 
                    x=67;
                    break;
                case 14 : 
                    x=72;
                    break;
                case 15 : 
                    x=77;
                    break;
                case 16 : 
                    x=82;
                    break;
                case 17 : 
                    x=87;
                    break;
                default : 
                    x=-1;
                    break;
            }
            switch (all_shapes[i].index[j].y){
                case 0 : 
                    y=2;
                    break;
                case 1 : 
                    y=7;
                    break;
                case 2 : 
                    y=12;
                    break;
                case 3 : 
                    y=17;
                    break;
                case 4 : 
                    y=22;
                    break;
                case 5 : 
                    y=27;
                    break;
                case 6 : 
                    y=32;
                    break;
                case 7 : 
                    y=37;
                    break;
                case 8 : 
                    y=42;
                    break;
                case 9 : 
                    y=47;
                    break;
                case 10 : 
                    y=52;
                    break;
                case 11 : 
                    y=57;
                    break;
                default : 
                    y=-1;
                    break;
            }
            
            if(x!=-1 && y!=-1){//if the box of the shape exists (hasn't been erased by a completed line))
                if(operation=='1'){
                    //display the current box of the shape in the grid 
                    for(uint8_t m=x;m<x+4;m++){
                        for(uint8_t n=y;n<y+4;n++){
                            glcd_PlotPixel(m,n,1);
                        }
                    }
                }
                else {
                    //erase the current box of the shape from the grid 
                    for(uint8_t m=x;m<x+4;m++){
                        for(uint8_t n=y;n<y+4;n++){
                            glcd_PlotPixel(m,n,0);
                        }
                    }
                } 
            }
        }
        i++;
    }
}

//test if pos (x,y) belong to the shape n°index
//return 1 if yes / 0 if no 
uint8_t dontBelong2Shape(uint8_t index,uint8_t x,uint8_t y){
    
    for(uint8_t k=0;k<4;k++){
        if(all_shapes[index].index[k].x == x && all_shapes[index].index[k].y == y){
            return 1;
        }
    }
    return 0;
}

//gravity on the shape n°index of the array all_shapes
//return 0 if the shape isn't blocked 
//return 1 if the shape is blocked
//return -1 if the shape is blocked on line 0 of the grid = game over 
uint8_t gravity(uint8_t index){
    
    //sort the array "index" of the current (n° index) shape (array "index" part of the structure shape)
    //so that the array contains the (x,y) position sorted in the descendant order for y 
    //avoid problems  
    uint8_t a,b,ok;
    for (uint8_t i = 0; i < 4; ++i){
      for (uint8_t j = i + 1; j < 4; ++j){
         if (all_shapes[index].index[i].y < all_shapes[index].index[j].y){
            a = all_shapes[index].index[i].y; 
            b = all_shapes[index].index[i].x;
            all_shapes[index].index[i].y = all_shapes[index].index[j].y;
            all_shapes[index].index[i].x = all_shapes[index].index[j].x;
            all_shapes[index].index[j].y = a;
            all_shapes[index].index[j].x = b;
         }
      }
   } 
    
    //try to increment y position of the current shape 
    for(uint8_t j=0;j<4;j++){
        
        if(all_shapes[index].index[j].x !=-1 && all_shapes[index].index[j].y !=-1){//if the box hasn't been erased 
            //if the shape hasn't reached the bottom of the grid or hasn't toutch another shape 
            if(all_shapes[index].index[j].y <11 && (array_data[all_shapes[index].index[j].x][(all_shapes[index].index[j].y)+1]!='1' || dontBelong2Shape(index,all_shapes[index].index[j].x,(all_shapes[index].index[j].y)+1))){
                ok=1;//we can take it down 
            }
            else {
                ok=0;//we can't take it down 
                break;
            }   
        }
           
    }
    if(ok==1){//take down the shape 
        for(uint8_t j=0;j<4;j++){
            if(all_shapes[index].index[j].x !=-1 && all_shapes[index].index[j].y !=-1){//if the box hasn't been erased 
                array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='0';//erase the old y pos from array_data
                all_shapes[index].index[j].y +=1;//increment y
                array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='1';//update with the new y pos from array_data
            }
        }
        return 0;
    }
    else{//can't take down the shape, it is blocked 
        for(uint8_t j=0;j<4;j++){
            if(all_shapes[index].index[j].y ==0){//the shape is blocked on line 0, game over
                return -1;
            }
        }
        return 1;//the shape isn't blocked on line 0, game continue 
    }
        
}

void moveRight(uint8_t index){
    
    //sort the array "index" of the current (n° index) shape (array "index" part of the structure shape)
    //so that the array contains the (x,y) position sorted in the descendant order for x 
    //avoid the shape to crash when in contact to another shape from the right   
    uint8_t a,b,ok;
    for (uint8_t i = 0; i < 4; ++i){
      for (uint8_t j = i + 1; j < 4; ++j){
         if (all_shapes[index].index[i].x < all_shapes[index].index[j].x){
            a = all_shapes[index].index[i].y; 
            b = all_shapes[index].index[i].x;
            all_shapes[index].index[i].y = all_shapes[index].index[j].y;
            all_shapes[index].index[i].x = all_shapes[index].index[j].x;
            all_shapes[index].index[j].y = a;
            all_shapes[index].index[j].x = b;
         }
      }
   }
    
    //try to increment x position of the current shape 
    for(uint8_t j=0;j<4;j++){
        //if the shape hasn't reached the bottom of the grid or hasn't toutch another shape 
        if(all_shapes[index].index[j].x <17 && (array_data[(all_shapes[index].index[j].x)+1][all_shapes[index].index[j].y]!='1' || dontBelong2Shape(index,(all_shapes[index].index[j].x)+1,all_shapes[index].index[j].y))){
            ok=1;
        }
        else {
            ok=0;
            break;
        }      
    }
    if(ok==1){
        for(uint8_t j=0;j<4;j++){
            array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='0';//erase the old y pos from array_data
            all_shapes[index].index[j].x +=1;//increment x
            array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='1';//update with the new y pos from array_data
        }
    }
}

void moveLeft(uint8_t index){
    
    //sort the array "index" of the current (n° index) shape (array "index" part of the structure shape)
    //so that the array contains the (x,y) position sorted in the ascendant order for x 
    //avoid the shape to crash when in contact to another shape from the left   
    uint8_t a,b,ok;
    for (uint8_t i = 0; i < 4; ++i){
      for (uint8_t j = i + 1; j < 4; ++j){
         if (all_shapes[index].index[i].x > all_shapes[index].index[j].x){
            a = all_shapes[index].index[i].y; 
            b = all_shapes[index].index[i].x;
            all_shapes[index].index[i].y = all_shapes[index].index[j].y;
            all_shapes[index].index[i].x = all_shapes[index].index[j].x;
            all_shapes[index].index[j].y = a;
            all_shapes[index].index[j].x = b;
         }
      }
   }
    
    //try to decrement x position of the current shape 
    for(uint8_t j=0;j<4;j++){
        //if the shape hasn't reached the bottom of the grid or hasn't toutch another shape 
        if(all_shapes[index].index[j].x >0 && (array_data[(all_shapes[index].index[j].x)-1][all_shapes[index].index[j].y]!='1' || dontBelong2Shape(index,(all_shapes[index].index[j].x)-1,all_shapes[index].index[j].y))){
            ok=1;
        }
        else {
            ok=0;
            break;
        }      
    }
    if(ok==1){
        for(uint8_t j=0;j<4;j++){
            array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='0';//erase the old y pos from array_data
            all_shapes[index].index[j].x -=1;//decrement x
            array_data[all_shapes[index].index[j].x][all_shapes[index].index[j].y]='1';//update with the new y pos from array_data
        }
    }
}


void delay(){
   uint16_t c, d;
   for (c = 0; c <= 65530; c++){
      // for (d = 0; d <= 1000; d++)
      // {} 
   }
   return;
}

void delayHomeScreen(){
   uint16_t c, d;
   for (c = 0; c <= 65530; c++){
       for (d = 0; d <= 10; d++)
       {} 
   }
   return;
}

//create a random shape in the array all_shapes at index i
void createRandomShape(uint8_t i){
      
    //random de 1 à 7 pour le type de forme 
    uint8_t type = (rand() % (7)) + 1;//int num = (rand() % (upper - lower + 1)) + lower;
    //random de 0 à 3 pour l'orientation 
    uint8_t orientation =rand()%4;
 
    //switch type 
    switch (type){
        case 1 :
            switch (orientation){ //din each case of type switch orientation 
                case 0 :  //define the position of blocks accordingly
                    all_shapes[i].orientation=0;
                    uint8_t start=8;
                    for (uint8_t k=0;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=0;
                        start++;
                    }
                    break;
                case 1 : 
                    all_shapes[i].orientation=1;
                    uint8_t start=8;
                    for (uint8_t k=0;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 2 :
                    all_shapes[i].orientation=2;
                    uint8_t start=0;
                    for (uint8_t k=0;k<4;k++){
                        all_shapes[i].index[k].x=10;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                case 3 : 
                    all_shapes[i].orientation=3;
                    uint8_t start=0;
                    for (uint8_t k=0;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                default : 
                    break;
            }
            break; 
        case 2 :
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=0;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 1 : 
                    all_shapes[i].orientation=1;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=2;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 2 :
                    all_shapes[i].orientation=2;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=2;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                case 3 : 
                    all_shapes[i].orientation=3;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=0;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                default : 
                    break;
            }
            break; 
        case 3 :
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=0;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 1 :
                    all_shapes[i].orientation=1;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=2;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 2 :
                    all_shapes[i].orientation=2;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=0;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                case 3 :
                    all_shapes[i].orientation=3;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=2;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                default : 
                    break;
            }
            break; 
        case 4 :
            all_shapes[i].index[0].x=11;
            all_shapes[i].index[0].y=0;
            all_shapes[i].index[1].x=12;
            all_shapes[i].index[1].y=0;
            all_shapes[i].index[2].x=11;
            all_shapes[i].index[2].y=1;
            all_shapes[i].index[3].x=12;
            all_shapes[i].index[3].y=1;
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    break;
                case 1 : 
                    all_shapes[i].orientation=1;
                    break;
                case 2 :
                    all_shapes[i].orientation=2;
                    break;
                case 3 : 
                    all_shapes[i].orientation=3;
                    break;
                default : 
                    break;
            }
            break; 
        case 5 :
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=1;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=0;
                    all_shapes[i].index[3].x=12;
                    all_shapes[i].index[3].y=0;
                    break;
                case 1 : 
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=2;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=2;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=12;
                    all_shapes[i].index[3].y=1;
                    all_shapes[i].orientation=1;
                    break;
                case 2 :
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=0;
                    all_shapes[i].index[1].x=10;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=11;
                    all_shapes[i].index[3].y=2;
                    all_shapes[i].orientation=2;
                    break;
                case 3 : 
                    all_shapes[i].index[0].x=11;
                    all_shapes[i].index[0].y=0;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=12;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=12;
                    all_shapes[i].index[3].y=2;
                    all_shapes[i].orientation=3;
                    break;
                default : 
                    break;
            }
            break; 
        case 6 :
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=11;
                    all_shapes[i].index[0].y=0;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 1 : 
                    all_shapes[i].orientation=1;
                    uint8_t start=10;
                    all_shapes[i].index[0].x=11;
                    all_shapes[i].index[0].y=2;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=start;
                        all_shapes[i].index[k].y=1;
                        start++;
                    }
                    break;
                case 2 :
                    all_shapes[i].orientation=2;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=1;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                case 3 : 
                    all_shapes[i].orientation=3;
                    uint8_t start=0;
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=1;
                    for (uint8_t k=1;k<4;k++){
                        all_shapes[i].index[k].x=11;
                        all_shapes[i].index[k].y=start;
                        start++;
                    }
                    break;
                default : 
                    break;
            }
            break; 
        case 7 :
            switch (orientation){
                case 0 :
                    all_shapes[i].orientation=0;
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=0;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=0;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=12;
                    all_shapes[i].index[3].y=1;
                    break;
                case 1 : 
                    all_shapes[i].index[0].x=10;
                    all_shapes[i].index[0].y=1;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=2;
                    all_shapes[i].index[3].x=12;
                    all_shapes[i].index[3].y=2;
                    all_shapes[i].orientation=1;
                    break;
                case 2 :
                    all_shapes[i].index[0].x=11;
                    all_shapes[i].index[0].y=0;
                    all_shapes[i].index[1].x=11;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=10;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=10;
                    all_shapes[i].index[3].y=2;
                    all_shapes[i].orientation=2;
                    break;
                case 3 : 
                    all_shapes[i].index[0].x=12;
                    all_shapes[i].index[0].y=0;
                    all_shapes[i].index[1].x=12;
                    all_shapes[i].index[1].y=1;
                    all_shapes[i].index[2].x=11;
                    all_shapes[i].index[2].y=1;
                    all_shapes[i].index[3].x=11;
                    all_shapes[i].index[3].y=2;
                    all_shapes[i].orientation=3;
                    break;
                default : 
                    break;
            }
            break; 
        default :
            break; 
    }
    
    //insert the new shape into the "array data"
    for (uint8_t k=0;k<4;k++){
        array_data[all_shapes[i].index[k].x][all_shapes[i].index[k].y]='1';
    }
    //update_display();
}

//erase the completed line and update data from the game 
void completedUpdate(uint8_t line){
    
    uint8_t i=0;
    while (all_shapes[i].index[0].x != 99){ //for all elem of all_shapes array
        for(uint8_t j=0;j<4;j++){
            if(all_shapes[i].index[j].y==line){//if a box of a shape is on the completed line 
                all_shapes[i].index[j].x=-1;//erase it 
                all_shapes[i].index[j].y=-1;
            }
        }
  
    }
    
    while (all_shapes[i].index[0].x != 99){ //for all elem of all_shapes array
        for(uint8_t j=0;j<4;j++){
            if(all_shapes[i].index[j].y < line && all_shapes[i].index[j].y!=-1){//increase y of all the box who has y < line (and hasn't been erased))
                all_shapes[i].index[j].y+=1;
            }
        }
    }
    
    //down line above the completed line in array data
    for(uint8_t j=line;j>0;j--){
        for(uint8_t i=0;i<17;i++){
            array_data[i][j]=array_data[i][j-1];
        }    
    }
    
    update_display_shapes('1');
}

//test if a line has been completed 
void lineCompletedTest(){
    
    uint8_t completed=0;
    for(uint8_t j=0;j<11;j++){//for all line 
        for(uint8_t i=0;i<17;i++){//for all columns 
            if(array_data[i][j]=='1')
                completed=1;
            else{
                completed=0;
                break;
            }
        }    
        if(completed==1){//a completed line has been detected 
            completedUpdate(j);//operate changes 
            score+=10;
        }
    }
}

//rotate 90° clockwise the shape n°index 
void rotate(uint8_t index){
    
    uint8_t temp_x,temp_y,ok;
    uint8_t new_coord[8];//x,y for 4 box 
    uint8_t pivot_x=all_shapes[index].index[1].x;//(pivot_x,pivot_y) = point to rotate around 
    uint8_t pivot_y=all_shapes[index].index[1].y;
    
    //calculate the new position for each box of the shape 
    for (uint8_t k=0;k<4;k++){
        temp_x=all_shapes[index].index[k].x;
        temp_y=all_shapes[index].index[k].y;
        new_coord[2*k]=pivot_x - pivot_y + temp_y;//new x position
        new_coord[2*k+1]=pivot_x + pivot_y - temp_x;//new y position 
        
        //if the new position is in the grid && (empty || dont belong to the shape)
        if(new_coord[2*k] >= 0 && new_coord[2*k] <=17 && new_coord[2*k+1] >= 0 && new_coord[2*k+1] <=11 && (array_data[new_coord[2*k]][new_coord[2*k+1]]!='1' || dontBelong2Shape(index,new_coord[2*k],new_coord[2*k+1]))){
            ok=1;
        }
        else {
            ok=0;
            break;
        }
    }
    
    if(ok){//make the modification on the new coord of the shape if all new positions are ok

        for (uint8_t k=0;k<4;k++){
            array_data[all_shapes[index].index[k].x][all_shapes[index].index[k].y]='0';//erase old position of the shape in array data
            all_shapes[index].index[k].x=new_coord[2*k];//update the position of the shape with the value previously calculated and stored in the array new_coord
            all_shapes[index].index[k].y=new_coord[2*k+1];
            array_data[all_shapes[index].index[k].x][all_shapes[index].index[k].y]='1';//insert new position of the shape in array data
        }
    }
    
}

//---------------------------	function score		-----------------------------------

void displayStartScore(char bestScore)
{
    char string[20]="";
    sprintf(string,"%d",bestScore);
    glcd_WriteString("best score : ",64,32);
    delayHomeScreen();
    glcd_FillScreen(0);//clear screen
    glcd_WriteString(string,64,32);
    delayHomeScreen();
    glcd_FillScreen(0);//clear screen
    
}

//---------------------------	MAIN		-----------------------------------

void main(void) {
    
    uint8_t index=-1;//index = index of the current shape 
    int8_t next=1;// next=1 if we have to generate a new shape / 0 if not / -1 if the game is over 
    TRISE = 0xFF;
    PORTE = 0x00;
    char bestScore = EEPROM_Read(ADDRESSZERO);
    
    glcd_Init(GLCD_ON);// Swith the screen ON
    glcd_Image();//Display home screen
    delayHomeScreen();//wait
    glcd_FillScreen(0);//clear screen
    displayStartScore(bestScore);
    display_grid();//display the grid 
    setupPWM(PR2_VALUE_2);
    
    for(uint8_t i=0;i<20;i++){//init of the array all_shapes
        for(uint8_t y=0;y<4;y++){
            all_shapes[i].index[y].x=99;
            all_shapes[i].index[y].y=99;
        }
    }
    
    for(uint8_t i=0;i<18;i++){//init of the array array_data
        for(uint8_t y=0;y<12;y++){
            array_data[i][y]='0';
        }
    }
    
    while (1){  
        if(next==-1){//if game over 
            break;
        }
        if (next){//if the current shape is blocked
            index++;
            createRandomShape(index);//create a new shape 
            next=0;
            score++;
        }
        ////////////////////AMELIORER LE POLLING EN TESTANT PLUSIEURS FOIS DANS LE WHILE ///////////////////////////
        if(PORTEbits.RE0 ==1){//if button right
            moveRight(index);
        }
        
        if(PORTEbits.RE1 ==1){//if button left
            moveLeft(index);
        }
        
        if(PORTEbits.RE2 ==1){//if button orientation
            rotate(index);
        }
        
        update_display_shapes('1');//display all the shapes in the grid
        delay();//wait 
        update_display_shapes('0');//erase the old position of all the shape in the grid
        //update_display();//update en fonction de array_data pas all_shapes ?
        
        if(PORTEbits.RE0 ==1){//if button right
            moveRight(index);
            if(T2CONbits.TMR2ON == 0)
            {
                pwmStart();
            }

        }
        
        if(PORTEbits.RE1 ==1){//if button left
            moveLeft(index);
            if(T2CONbits.TMR2ON == 0)
            {
                pwmStart();
            }
        }
        
        if(PORTEbits.RE2 ==1){//if button orientation
            rotate(index);
            if(T2CONbits.TMR2ON == 0)
            {
                pwmStart();
            }
        }
        if( (PORTEbits.RE0 != 1) && (PORTEbits.RE1 != 1) && (PORTEbits.RE2 != 1) )
        {
            pwmStop();
        }
        
        next=gravity(index);//gravity on the current shape 
        lineCompletedTest();//test if a line is completed and perform operations if needed 
        
    }
    
    //game is over 
    glcd_FillScreen(0);//clear screen
    if(score >= bestScore )
    {
        glcd_text_write("you beat your best score !",1,1);
    }
    EEPROM_Write(ADDRESSZERO, score);
    
    return;
}
