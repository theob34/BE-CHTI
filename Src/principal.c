
#include "GestionSon.h"
#include "DriverJeuLaser.h"

int DFT_ModuleAuCarre(void *, int) ;
extern int * LeSignal;
extern int * LeSignalSinus;
short int dma_buf[64];
int fft[64];

int freq[3] = {17,18,19};

void Systick_function () {
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	
	for (int i = 0; i<64; i++) {
		fft[i] = DFT_ModuleAuCarre(dma_buf, i);
	}
	
	for(int i = 0; i < 3; i++) {
		if(fft[freq[i]] >= 0x0100000) {
			StartSon();
		}
	}

}

int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

// Apr�s ex�cution : le coeur CPU est clock� � 72MHz ainsi que tous les timers
CLOCK_Configure();

//Configuration du timer
Systick_Period_ff(360000) ;
Systick_Prio_IT(1, Systick_function) ;
SysTick_On ;
SysTick_Enable_IT ;	
	
	
//Configuration ADC et DMA
Init_TimingADC_ActiveADC_ff( ADC1, 72 );
Single_Channel_ADC( ADC1, 2 );
 Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
 Init_ADC1_DMA1( 0, dma_buf );

 // Son
 Timer_1234_Init_ff(TIM4, 6552); // Timer 4 rgl  91us
	PWM_Init_ff(TIM3, 3, 720);
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	
	Active_IT_Debordement_Timer(TIM4, 2, &timer4_callback);

	
	

//============================================================================	
	
	
	while	(1)
	{
	}
}

