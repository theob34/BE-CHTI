	PRESERVE8
	THUMB   
	include Driver/DriverJeuLaser.inc

; ====================== zone de rservation de donnes,  ======================================
;Section RAM (read only) :
	area    mesdata,data,readonly


;Section RAM (read write):
	area    maram,data,readwrite
	export CurrentIndex
CurrentIndex	dcd	5513
	export SortieSon
SortieSon	dcw	0
	extern Son
	extern LongueurSon

	
; ===============================================================================================
	


		
;Section ROM code (read only) :		
	area    moncode,code,readonly
; crire le code ici		

	export timer4_callback
	export StartSon

StartSon proc
	ldr r0, =CurrentIndex
	mov r1, #0
	str r1, [r0]
   bx lr
	endp


timer4_callback proc
; r0 = Son
; r1 = Indice actuel
; r2 = Adresse indice actuel
; r3 = Registre temporaire pour le calcul

; int current_index ;
; int sortie_son ;
; int *son ;
; int longueur_son = 5513 ;

; if (current_index <= longueur_son) {
; 	r0 = résultat de la conversion ;
; 	current_index ++;
;} else {
;	r0 = 0;
;}
; mettre pwm à r0;
; mettre sortie_son à r0;



; Chargement de l'incide
	ldr r2, =CurrentIndex
	ldr r1, [r2]
; Chargement de la longueur du son
	ldr r3, =LongueurSon
	ldr r3,[r3]
	cmp r3,r1
	bhs debutCalcul

	mov r0, #0
	
	b finCacul

debutCalcul
	; Ajout de l'index  l'adresse de "Son", deux fois pour décaler de 2 octets
	ldr r0, =Son
;	ldr r0, [r0]
	add r0, r1
	add r0, r1

	; Lecture du son actuel
	ldrsh r0, [r0]

	;Mise sur [0;719]
	;on décale sur [0, 2^16] puis on divise par 2^16*719
	ldr r3, =719 ;On met 719 dans r3
	mul r0, r3	;On multiplie r0 par 719
	asr r0, r0, #15 ;On divise r0 par 2^15
	add r0, #719 ;On ajoute à r0 719
	asr r0, r0, #1 ;On divise r0 par 2

; Increment de l'indice en meme temps que le calcul pour ne pas dépasser la fin du son
	add r1, #1
	str r1, [r2]
finCacul
	
	; Stockage du son actuel
	push {lr, r2, r1, r0}
	bl PWM_Set_Value_TIM3_Ch3
	pop {lr, r2, r1, r0}
	ldr r3, =SortieSon
	str r0, [r3]

	bx lr
	endp
	END	