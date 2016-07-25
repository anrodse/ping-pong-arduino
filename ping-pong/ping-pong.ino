
const int nLed = 6;
const int led[nLed] = {6,7,8,9,10,11};
// Led de jugador y pulsador
const int led0 = 5, bot0 = 4;
const int led1 = 12, bot1 = 13;
const int zumba = 0;

int turno;	// Jugador que mueve
int turno_inicial;// Jugador que hace primer mueve
int juego;	// Turno de la partida (de 0 a 11)

int puntuacion[2];

int vel = 100;	// Velocidad de movimiento

int incr = 1;	// Direccion de movimiento;
int pos;	// Led encendido

void setup() {
	for (int i=0; i<=nLed; i++){
		pinMode(led[i], OUTPUT);
		digitalWrite(led[i], LOW);
	}
	pinMode(A3, INPUT);	// Potenciometro
	pinMode(bot0, INPUT);	// Boton de jugador
	pinMode(bot1, INPUT);
	pinMode(led0, OUTPUT);	// Led rojo de señal
	pinMode(led1, OUTPUT);

	juego = turno_inicial = turno = 0;
	puntuacion[0] = puntuacion[1] = 0;
}

void tono(int tono){
//	tone(zumba, tono, 100);
//	delay(100);
//	noTone(zumba);
}

// Espera la pulsacion del jugador
// Si no se pulsa, señala el fallo y
// actualiza la puntuacion
int esperoPulsacion(){
	int v = vel/100;
	for (int i=0; i<100;i++) {
		delay(v);
		if (digitalRead(bot0)==HIGH && turno==0){
			if (i<10) break; // Para evitar una pulsacion continua
			tono(73);
			return 1;  // Pulsado!
		}
		else if (digitalRead(bot1)==HIGH && turno==1){
			if (i<10) break; // Para evitar una pulsacion continua
			tono(73);
			return 1;  // Pulsado!
		}
	}

	int l;
	if (turno == 0){
		l = led0;
		// Actualizar puntuacion
		puntuacion[1]++;
	}
	else if (turno == 1){
		l = led1;
		// Actualizar puntuacion
		puntuacion[0]++;
	}

	for (int i=0; i<5; i++){
		digitalWrite(l,HIGH);
		delay(500);
		digitalWrite(l,LOW);
		delay(500);
	}

	return 0;
}

void ganador0(){
	digitalWrite(led0, HIGH);
	digitalWrite(led1, LOW);

	while(1){  // Paro
		for (int i=nLed-1; i>=0; i--){
			digitalWrite(led[i], HIGH);
			delay(100);
			digitalWrite(led[i], LOW);
		}
	}
}
void ganador1(){
	digitalWrite(led1, HIGH);
	digitalWrite(led0, LOW);

	while(1){  // Paro
		for (int i=0; i<=nLed; i++){
			digitalWrite(led[i], HIGH);
			delay(100);
			digitalWrite(led[i], LOW);
		}
	}
}


void loop() {
	digitalWrite(led0,HIGH);	// Empieza el turno
	digitalWrite(led1,HIGH);
	delay(1000);
	digitalWrite(led0,LOW);
	digitalWrite(led1,LOW);
	vel = (analogRead(A3)*1.46)+500;	// Velocidad de movimiento [500, 2000]

	if (turno_inicial == 1){
		digitalWrite(led0,LOW);
		digitalWrite(led1,HIGH);
		while (digitalRead(bot1) != HIGH);
		digitalWrite(led1,LOW);
		incr = -1; pos = nLed-1; turno = 0;
	} else {
		digitalWrite(led1,LOW);
		digitalWrite(led0,HIGH);
		while (digitalRead(bot0) != HIGH);
		digitalWrite(led0,LOW);
		incr = 1; pos = 0; turno = 1;
	}
	tono(225);

	while (1) {
		for (int i=0; i<nLed; i++){	// Movimiento led
			digitalWrite(led[pos],HIGH);
			delay(vel);
			digitalWrite(led[pos],LOW);
			pos = pos + incr;	// Paso a led siguiente
		}

		// Espero pulsacion
		if (esperoPulsacion()==0) break;
		incr = incr*(-1); turno = (turno+1)%2;
		vel--;		// Cada vez mas rapido
	}
	juego++;
	if ((juego%3)==0){	// Cambio el turno inicial
		turno_inicial = (turno_inicial+1)%2;
	}

		// Gana el jugador 0
	if (puntuacion[0]==11){
		ganador0();
	}
		// Gana el jugador 1
	if (puntuacion[1]==11){
		ganador1();
	}

}

