#include <xc.h>
#include "glcd_library/glcd.h"

#pragma config FOSC = HS
#pragma config LVP = OFF

/*typedef struct box{
    int empty; //0=empty 1=busy
}t_box;*/

typedef struct pos{
    int x;
    int y;
}t_pos;

typedef struct shape{
    int nBox; //number of box of the shape
    t_pos index[]; //will be dynamically allocated with size = nBox when creation
    int orientation; //1,2,3 ou 4
}t_shape;



void display_array(){
    for (int i=0;i<128;i++){
        for(int j=0;j<64;j++){
            if ((j>=0 && j<=1) || (j>=62 && j<=63)) //bordure haut et bas 
                glcd_PlotPixel(i,j,1);
            else if ((i>=0 && i<=1) || ( i>=126 && i<=127))//bordure gauche droite
                glcd_PlotPixel(i,j,1);
            else if (i>=91 && i<=92) //limite grille droite
                glcd_PlotPixel(i,j,1);
            /*    
            //colonnes
            if ( (i>=13 && i<=16) || (i>=26 && i<=29) || (i>=39 && i<=42) || (i>=52 && i<=55) || (i>=65 && i<=68) || (i>=78 && i<=81) || (i>=91 && i<=93) || (i>=103 && i<=106) || (i>=116 && i<=119) )
                glcd_PlotPixel(i,j,1);*/
            
            //ligne
            if ( (j>=10 && j<=13) || (j>=20 && j<=23) || (j>=30 && j<=33) || (j>=40 && j<=43) || (j>=50 && j<=53) )
                glcd_PlotPixel(i,j,1);
        }
    }
    
}

void main(void) {
    
    int array [10][7];
    
    // Swith the screen ON 
    glcd_Init(GLCD_ON);
    display_array();
    while (1){
        // Clear the screen
        //glcd_FillScreen(0);
    }
    
    return;
}
