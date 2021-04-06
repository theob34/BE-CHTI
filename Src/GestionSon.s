	PRESERVE8
	THUMB   
	include Driver/DriverJeuLaser.inc

; ====================== zone de rservation de donnes,  ======================================
;Section RAM (read only) :
	area    mesdata,data,readonly


;Section RAM (read write):
	area    maram,data,readwrite
	export CurrentIndex
CurrentIndex	dcd	0
	export SortieSon
SortieSon	dcw	0
	extern Son
	extern LongueurSon

	
; ===============================================================================================
	


		
;Section ROM code (read only) :		
	area    moncode,code,readonly
; crire le code ici		

	export timer4_callback

timer4_callback proc
; int current_index
; int SortieSon
; int *Son

; SortieSon = *(Son + current_index) (Opération de pointeur, se décaler de 2 octets pour s'aligner sur le son)
;
; if(current_index > 5512)
;    current_index = 0;
;
	; Chargement de l'index
	ldr r2, =CurrentIndex
	ldr r1, [r2]

	; Ajout de l'index  l'adresse de "Son", deux fois pour dcaler de 2 octets
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
	
	
	; Stockage du son actuel
	push {lr, r2, r1, r0}
	bl PWM_Set_Value_TIM3_Ch3
	pop {lr, r2, r1, r0}
	ldr r3, =SortieSon
	str r0, [r3]

	; Incrment de CurrentIndex
	add r1, #1

	; Mise en boucle du signal pour s'amuser
;	ldr r3, =LongueurSon
;	ldr r3,[r3]
;	cmp r3,r1
;	bne fin
;	mov r1, #0

;fin

	str r1, [r2]
	bx lr
	endp
	END	