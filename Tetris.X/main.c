#include <xc.h>
#include "glcd_library/glcd.h"
#include "glcd_library/TS.h"

#define _XTAL_FREQ 4000000 // 4Mhz

#pragma config FOSC = HS
#pragma config LVP = OFF
#pragma config WDT=OFF // watchdog is off

typedef struct box{
    int empty; //0=empty 1=busy
}t_box;

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
            if ((j>=3 && j<=4) || (j>=62 && j<=63)) //bordure haut et bas 
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
    
}

void main(void) {

    glcd_Init(GLCD_ON);
    glcd_SetCursor(1,1);
    setupADC();
    char str[20]="x = ";
    while (1){
        
        __delay_ms(1000);
        sprintf(str, "%f",readX());
        glcd_text_write(str, 1, 1);
        
        
    }
    
    return;
}
