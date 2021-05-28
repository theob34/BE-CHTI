#include "Affichage_Valise.h"
#include "GestionSon.h"
#include "DriverJeuLaser.h"

#define SEUIL 0x0000900
#define NBPLAYERS 4
#define DELAIS_SCORE 20
#define SEUIL_SCORE 12
int players_freq[NBPLAYERS] = {17,18,19,20};
int LED_cibles[4] = {LED_Cible_1, LED_Cible_2, LED_Cible_3, LED_Cible_4};


// Structures
typedef struct player {
	int freq;
	
	int score;
	unsigned int time_last_score;
	
	unsigned int time_last_touch;
	int temp_count;
} player_t;

// Fonctions ASM
int DFT_ModuleAuCarre(void *, int);


// Variables globales
short int dma_buf[64];
int fft[64];
player_t players[NBPLAYERS];
int cible = 1;

// Compteur
unsigned int time_5ms = 0;


void update_score() {
	for(int i = 0; i < NBPLAYERS; i++){
		Prepare_Afficheur(i+1, players[i].score);
	}
	
	
	Mise_A_Jour_Afficheurs_LED();
}

void Systick_function () {
	time_5ms++;
	
	Start_DMA1(64);
	Wait_On_End_Of_DMA1();
	Stop_DMA1;
	
	for (int i = 0; i<64; i++) {
		fft[i] = DFT_ModuleAuCarre(dma_buf, i);
	}
	
	for(int i = 0; i < NBPLAYERS; i++) {
		if(fft[players[i].freq] > SEUIL) {
			// Touche détectée
			if(time_5ms > players[i].time_last_touch) {
				// Détection d'une touche successive
				players[i].temp_count++;
				players[i].time_last_touch = time_5ms;
			}
			if(players[i].temp_count > SEUIL_SCORE && players[i].time_last_score + DELAIS_SCORE < time_5ms) {
				players[i].score ++;
				players[i].time_last_score = time_5ms;
				players[i].temp_count = 0;
				StartSon();
			}
			if(players[i].time_last_score + 20 > time_5ms) {
				players[i].temp_count = 0;
			}
		}
	}
	
	if((time_5ms & 0xFF) == 0xFF) {
		Prepare_Clear_LED(LED_cibles[cible]);
		cible = ((fft[17]+fft[22]) & 3);
		Choix_Capteur(cible+1);
		Prepare_Set_LED(LED_cibles[cible]);
	}
	
	update_score();
}

void init_players() {
	for(int i = 0; i < NBPLAYERS; i++) {
		players[i].freq = players_freq[i];
		players[i].score = 0;
		players[i].time_last_score = 0;
		players[i].time_last_touch = 0;
		players[i].temp_count = 0;
	}
}

int main(void)
{
	CLOCK_Configure(); // Horloge principale
	
	init_players(); // Initialisation des joueurs
	
	// Initialisation ADC
	Init_TimingADC_ActiveADC_ff( ADC1, 72 );
	Single_Channel_ADC( ADC1, 2 );
	Init_Conversion_On_Trig_Timer_ff( ADC1, TIM2_CC2, 225 );
	Init_ADC1_DMA1( 0, dma_buf );
	
	// Initialisation du callback de comptage de score
	Systick_Period_ff(360000) ;
	Systick_Prio_IT(9, Systick_function) ;
	SysTick_On ;
	SysTick_Enable_IT ;	
	
	// Initialisation son (callback + PWM)
	Timer_1234_Init_ff(TIM4, 6552); // Timer 4 rgl  91us
	PWM_Init_ff(TIM3, 3, 720);
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL);
	Active_IT_Debordement_Timer(TIM4, 2, &callback_son);
	
	// Initialisation de l'affichage
	Init_Affichage();
	
	// Boucle infinie
	while	(1){}
}

