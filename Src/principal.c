#include "DriverJeuLaser.h"
#include "GestionSon.h"

int main(void) {
// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

	// Aprs excution : le coeur CPU est clock  72MHz ainsi que tous les timers
	CLOCK_Configure();
	Timer_1234_Init_ff(TIM4, 6552); // Timer 4 rgl  91us
	PWM_Init_ff(TIM3, 3, 720);
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	
	Active_IT_Debordement_Timer(TIM4, 2, &timer4_callback);

//============================================================================	

	while (1) {
		//10 million = 1 sec
		for (int i = 0; i < 10000000; i++) {
			i = i + 1 - 1;
		}
		StartSon();
	}
}
