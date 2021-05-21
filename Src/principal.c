
#include "GestionSon.h"
#include "DriverJeuLaser.h"

int DFT_ModuleAuCarre(void *, int) ;
extern int * LeSignal;
extern int * LeSignalSinus;
short int dma_buf[64];
int fft[64];
int compteur = 0;
int score[4] = [0,0,0,0];	//pour compter le score
int	touche[4] = [0,0,0,0];	//pour compter le nombre de touche. Si supérieur à 12 (12x5 = 60ms) alors on estime que le tir a bien touché la cible

int freq[6] = {17,18,19,20,23,24};

void Systick_function () {
	compteur++;
	
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	
	for (int i = 0; i<64; i++) {
		fft[i] = DFT_ModuleAuCarre(dma_buf, i);
	}
	
	for(int i = 0; i < 6; i++) {
		if(fft[freq[i]] >= 0x0100000 && touche[i] < touche[i] + 20) {
			touche[i] = compteur ;
			score[i] ++;
			StartSon();
		}
	}
	

}

int main(void)
{

// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

// Après exécution : le coeur CPU est clocké à 72MHz ainsi que tous les timers
CLOCK_Configure();

//Configuration du timer
Systick_Period_ff(360000) ;
Systick_Prio_IT(3, Systick_function) ;
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

