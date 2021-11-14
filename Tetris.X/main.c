#include <xc.h>
#include <stdint.h>
#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "glcd_library/glcd.h"

#pragma config FOSC = HS
#pragma config LVP = OFF

//GLCD SCREEN 128X64

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

//update the dsplay of all the shapes on the grid from the array "all_shapes"
//operation = 1 --> display the shapes on the grid 
//operation = 0 --> erase the shape from the grid 
void update_display_shapes(t_shape all_shapes[], char operation){
    
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
                    break;
            }
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
        i++;
    }
}

void gravity(t_shape all_shapes[]){
    
    uint8_t i=0;
    while (all_shapes[i].index[0].x != 99){
        for(uint8_t j=0;j<4;j++){
            if(all_shapes[i].index[j].y <11 ){//que pour les box du bas sinon s'écrase 
                all_shapes[i].index[j].y +=1;
                //update array data
                //détecter collision
            }
            else 
                break;
        }
        i++;
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

//store the state of the game. 0 = free box and 1 = busy box 
char array_data[18][12]; //18 columns 12 lines

//ATTENTION bit focntionne que en global ou static et pas de tableau 

void main(void) {
    
    t_shape all_shapes [20]; //array of all the shapes of the game 
    glcd_Init(GLCD_ON);// Swith the screen ON
    display_grid();//display the grid 
    
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
    
    ///////test creation une shape
    ///ATTENTION METTRE LES BLOC DU BAS DE LA FORME AU INDICES LES PLUS PETITS 
    all_shapes[0].index[0].x=6;
    all_shapes[0].index[0].y=1;
    all_shapes[0].index[1].x=7;
    all_shapes[0].index[1].y=1;
    all_shapes[0].index[2].x=8;
    all_shapes[0].index[2].y=1;
    all_shapes[0].index[3].x=7;
    all_shapes[0].index[3].y=0;
    ///////////////////////////////////////////////////////////////////////////
    
    update_display_shapes(all_shapes,'1');
    //mettre chaque shape créée dans array data
    
    while (1){
        delay();
        update_display_shapes(all_shapes,'0');
        gravity(all_shapes);
        update_display_shapes(all_shapes,'1');
    }
    return;
}
