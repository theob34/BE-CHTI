#include "DriverJeuLaser.h"

void timer4_callback(void);

int main(void) {
// ===========================================================================
// ============= INIT PERIPH (faites qu'une seule fois)  =====================
// ===========================================================================

	// Aprs excution : le coeur CPU est clock  72MHz ainsi que tous les timers
	CLOCK_Configure();
	Timer_1234_Init_ff(TIM4, 6552); // Timer 4 rgl  91us
	Active_IT_Debordement_Timer(TIM4, 2, &timer4_callback);

//============================================================================	

	while (1) {
		
	}
}
