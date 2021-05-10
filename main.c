/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

CY_ISR_PROTO(timer_ISR);

uint8 BandTimer=0;

uint16 Contador=0;

uint16 VALADCX=0;
uint16 VALADCZ=0;

uint16 Vx=0;
uint16 Vz=0;
uint16 Angulo=0;

void Visualizacion(void);

int main(void)
{
    CyGlobalIntEnable;
    timer_StartEx(timer_ISR);
    Contador_Start();
    ADCX_Start();
    ADCZ_Start();
    LCD_Start();
    LCD_Position(0,4);
    LCD_PrintString("ACELEROMETRO");


    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        if(BandTimer){
            //entra cada 1ms
            BandTimer=0;
            Contador++;
            if(Contador%100 == 0){
                //cada 100ms toma una muestra
                ADCX_StartConvert();
                ADCX_IsEndConversion(ADCX_WAIT_FOR_RESULT);
                VALADCX+=ADCX_GetResult16();
                ADCZ_StartConvert();
                ADCZ_IsEndConversion(ADCZ_WAIT_FOR_RESULT);
                VALADCZ+=ADCZ_GetResult16();
                if (Contador>=1000){
                    //al llegar a 1s hace el promedio y la visualización se activa
                    Contador=0;
                    //promedios
                    VALADCX/=10;
                    VALADCZ/=10;
                    //Vx con 3 cifras decimales
                    Vx= (uint64)(VALADCX * 5000)/4095;
                    Vz= (uint64)(VALADCZ * 5000)/4095;
                    
                    if(VALADCX<=1619){
                        Angulo=0;
                    }
                    else if(VALADCX>1619 && VALADCX<=1832){
                        //debido a que en un rango el valor se vuelve negativo
                        if(Vx*10-19813 < 0){
                            Angulo= (uint32)(19813-Vx*10)/59;
                        }
                        else{
                            Angulo= (uint32)(Vx*10-19813)/59;
                        }
                    }
                    else if(VALADCZ>1669 && VALADCZ<=1880){
                        Angulo=(uint32)(25820-Vz*10)/61;
                    }
                    else if(VALADCZ<=1669){
                        Angulo=90;
                    }
                    Visualizacion();
                }
            }
        }
    }
}
CY_ISR(timer_ISR){
    //configurado F=1KHz
    BandTimer=1;
}
void Visualizacion(void){
    //Angulo
    LCD_Position(3,1);
    LCD_PrintString("Angulo:");
    LCD_Position(3,8);
    LCD_PrintString("       ");
    LCD_Position(3,8);
    LCD_PrintNumber(Angulo);
    LCD_PutChar(0xDF);
    //ADCx
    LCD_Position(1,1);
    LCD_PrintString("ADCX:");
    LCD_Position(1,6);
    LCD_PrintNumber(VALADCX);
    //Vx
    /*
    LCD_PrintNumber(VALADCX);
    LCD_Position(1,11);
    LCD_PrintString("Vx=");
    Vx= (uint64)(VALADCX * 5000)/4095;
    LCD_PrintNumber(Vx);
    LCD_PrintString(".");
    LCD_PrintNumber(Vx/100%10);
    LCD_PrintNumber(Vx/10%10);
    */
    //ADCz
    LCD_Position(2,1);
    LCD_PrintString("ADCZ:");
    LCD_Position(2,6);
    LCD_PrintNumber(VALADCZ);
    //Vz
    /*
    LCD_Position(2,11);
    LCD_PrintString("Vz=");
    Vz= (uint64)(VALADCZ * 5000)/4095;
    LCD_PrintNumber(Vz);
    LCD_PrintString(".");
    LCD_PrintNumber(Vz/100%10);
    LCD_PrintNumber(Vz/10%10);
    */
}

/* [] END OF FILE */
