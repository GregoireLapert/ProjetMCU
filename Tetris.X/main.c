#include <xc.h>
<<<<<<< Updated upstream
=======
#include <stdint.h>
#include <string.h>
<<<<<<< Updated upstream
#include<stdio.h>
#include<stdlib.h>
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes
#include "glcd_library/glcd.h"

#pragma config FOSC = HS
#pragma config LVP = OFF

<<<<<<< Updated upstream
/*typedef struct box{
    int empty; //0=empty 1=busy
}t_box;*/
=======
//GLCD SCREEN 128X64
<<<<<<< Updated upstream
=======

//enum state{OFF,ON};//OFF=0 ON=1
enum orientation{UP,DOWN,LEFT,RIGHT};// 0 1 2 3
>>>>>>> Stashed changes
>>>>>>> Stashed changes

typedef struct pos{
<<<<<<< Updated upstream
    int x;
    int y;
=======
<<<<<<< Updated upstream
    uint8_t x;//column between 0 and 17
    uint8_t y;//line between 0 and 11
=======
    uint8_t x;//line
    uint8_t y;//column
>>>>>>> Stashed changes
>>>>>>> Stashed changes
}t_pos;

typedef struct shape{
<<<<<<< Updated upstream
    int nBox; //number of box of the shape
    t_pos index[]; //will be dynamically allocated with size = nBox when creation
    int orientation; //1,2,3 ou 4
}t_shape;



void display_array(){
    for (int i=0;i<128;i++){
        for(int j=0;j<64;j++){
            if ((j>=3 && j<=4) || (j>=62 && j<=63)) //bordure haut et bas 
=======
<<<<<<< Updated upstream
    t_pos index[4]; //x between 0 and 17 / y between 0 and 11 in the grid 
    enum orientation orientation; //1,2,3 ou 4
}t_shape;

//display the grid on the GLCD
=======
    t_pos index[4]; //will be dynamically allocated with size = nBox when creation
    enum orientation orientation; //1,2,3 ou 4
}t_shape;



>>>>>>> Stashed changes
void display_grid(){
    for (uint8_t i=0;i<128;i++){
        for(uint8_t j=0;j<64;j++){
            if ((j>=0 && j<=1) || (j>=62 && j<=63)) //bordure haut et bas 
                glcd_PlotPixel(i,j,1);
            else if (((i>=0 && i<=1) || ( i>=126 && i<=127)))//bordure gauche droite
>>>>>>> Stashed changes
                glcd_PlotPixel(i,j,1);
            else if (((i>=0 && i<=1) || ( i>=126 && i<=127)) && (j>4))//bordure gauche droite
                glcd_PlotPixel(i,j,1);
            else if (i>=91 && i<=92 && j>4) //limite grille droite
                glcd_PlotPixel(i,j,1);
             
            //colonnes
            if ( ((i==10) || (i==19) || (i==28) || (i==37) || (i==46) || (i==55) || (i==64) || (i==73) || (i==82)) && (j>4) )
                glcd_PlotPixel(i,j,1);
            
            //ligne
            if ( ((j==17) || (j==26) || (j==35) || (j==44) || (j==53) || (j==62)) && (i<92))
                glcd_PlotPixel(i,j,1);
        }
    }
<<<<<<< Updated upstream
    
}

void main(void) {
    
    int array [10][7];
=======
}

<<<<<<< Updated upstream
//update the dsplay of all the shapes on the grid from the array "all_shapes"
//operation = 1 --> display the shapes on the grid 
//operation = 0 --> erase the shape from the grid 
void update_display_shapes(t_shape all_shapes[], char operation){
    
    uint8_t x,y,i=0;

    while (all_shapes[i].index[0].x != 99){ //for all elem of all_shapes array
=======
void display_shapes(t_shape all_shapes[]){
    
    for(uint8_t i=0;i<strlen(all_shapes);i++){ //for all elem of all_shapes array
>>>>>>> Stashed changes
        for(uint8_t j=0;j<4;j++){ //for all box of the current shape 
            //switch according to the value of x and y. Each one corresponds to an int = top left of the box in the grid 
            switch (all_shapes[i].index[j].x){
                case 0 : 
<<<<<<< Updated upstream
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
=======
                    
                    break;
                case 1 : 
                    break;
                case 2 : 
                    break;
                case 3 : 
                    break;
                case 4 : 
                    break;
                case 5 : 
                    break;
                case 6 : 
                    break;
                case 7 : 
                    break;
                case 8 : 
                    break;
                case 9 : 
                    break;
                case 10 : 
                    break;
                case 11 : 
                    break;
                case 12 : 
>>>>>>> Stashed changes
                    break;
                default : 
                    break;
            }
<<<<<<< Updated upstream
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
=======
        }
    }
}

//ATTENTION bit focntionne que en global ou static et pas de tableau 

//store the state of the game. 0 = free box and 1 = busy box 
char array_data[18][12]; //18 lines 12 columns
>>>>>>> Stashed changes

void main(void) {
    
    t_shape all_shapes [20]; //array of all the shapes of the game 
    glcd_Init(GLCD_ON);// Swith the screen ON
    display_grid();//display the grid 
<<<<<<< Updated upstream
    
    for(uint8_t i=0;i<20;i++){//init of the array all_shapes
        for(uint8_t y=0;y<4;y++){
            all_shapes[i].index[y].x=99;
            all_shapes[i].index[y].y=99;
        }
=======
    
    ///////test creation une shape
    t_shape shape1;
    t_pos i0;
    i0.x=0;
    i0.y=10;
    t_pos i1;
    i1.x=1;
    i1.y=9;
    t_pos i2;
    i2.x=1;
    i2.y=10;
    t_pos i3;
    i3.x=1;
    i3.y=11;
    shape1.index[0].x=i0.x;
    shape1.index[0].y=i0.y;
    shape1.index[1].x=i1.x;
    shape1.index[1].y=i1.y;
    shape1.index[2].x=i2.x;
    shape1.index[2].y=i2.y;
    shape1.index[3].x=i3.x;
    shape1.index[3].y=i3.y;
    
    all_shapes[0]=shape1;
    
    display_shapes(all_shapes);
    
    while (1){
        // Clear the screen
        //glcd_FillScreen(0);
>>>>>>> Stashed changes
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
>>>>>>> Stashed changes
    
    // Swith the screen ON 
    glcd_Init(GLCD_ON);
    display_array();
    while (1){
        // Clear the screen
        //glcd_FillScreen(0);
    }
    
    return;
}
