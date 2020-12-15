/*
 * File:   main.c
 * Author: Victor Hugo
 *
 * Created on 11 de Novembro de 2020, 10:14
 */

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

/*
led incorporado pisca a cada 500ms
led do pino 12 pisca a cada 780ms
enviar mensagem a cada 5s
*/


// para o correto funcionamento do programa, sabemos que o clock do microcontrolador funciona com 16MHz
// Dessa forma, o periodo T = 1/16M
// utilizamos o prescaler de CLK/64, logo o periodo sera 4 x 10^(6)
// para que a interrupcao ocorra a cada 1ms, 4 x 10^(-6) x OSR2A = 10^(-3)
// com  isso encontramos OSR2A = 250 - 1 visto que a contagem inicia em zero


/*Configuracao do temporizador
Interrupao gerada a cada 1 ms
Prescale = 64
OCR2A = 249
*/


char msg[] = "Atividade 7 - Interrupcoes temporizadas tratam concorrencia entre tarefas! \n\n";
volatile int cont_led13 ;
volatile int cont_led12 ;
volatile int i ;

unsigned char* p_ubrrh = (unsigned char *) 0x0C5;//ponteiro para definir a taxa de transmissao e recepcao
unsigned char* p_ubrrl = (unsigned char *) 0x0C4; //Ponteiro para definir a taxa de transmissao e recepcao

unsigned char* p_ucsra = (unsigned char *) 0x0C0; //ponteiro para definir UCSR0A
unsigned char* p_ucsrb = (unsigned char *) 0x0C1; //ponteiro para definir UCSR0B
unsigned char* p_ucsrc = (unsigned char *) 0x0C2; //ponteiro para definir UCSR0C
unsigned char* p_udr0 = (unsigned char*) 0x0C6; //ponteiro para UDR0, registrador de recepcao e transmissao de dados 

unsigned char* p_ddrb = (unsigned char *) 0x024; //ponteiro para configuracao dos leds
unsigned char* p_portb = (unsigned char *) 0x025; //ponteiro para enviar sinais para os 

unsigned char* p_tccr2a = (unsigned char *) 0xB0; //ponteiro para definir TCCR2A 
unsigned char* p_tccr2b = (unsigned char *) 0xB1;  //ponteiro para definir TCCR2B
unsigned char* p_osr2a = (unsigned char *) 0xB3;  //ponteiro para definir OSR2A
unsigned char* p_timsk2 = (unsigned char *) 0x70;  //ponteiro para definir TIMSK2



void setup(){
    cli();
    //Setando o valor do Baud Rate como 19.200
    *p_ubrrl = 0x33;
    *p_ubrrh = 0x00;
 
    //Configurando registrador para transmissao normal e assincrona     
     *p_ucsra = 0b00000000;
      
    //Interrupcoes de Trasnmissao completa ativadas e buffer vazio desativada; transmissao habilitada
    *p_ucsrb = 0b01001000;
  
    //Configurando Modo assincrono, sem bit de paridade, 1 bit de parada e uso de 8 bits por frame
    *p_ucsrc = 0b00000110; /*00000110*/

    //configurando DDRB1 como Saida (Nivel logico alto) para pinos 12 e 13
    *p_ddrb = *p_ddrb | 0b00110000;
    
    //Setando os leds para Inicializacao com eles apagados.
    *p_portb = *p_portb | 0x00;

    
    *p_tccr2a = 0x02; 
    *p_tccr2b = 0x04; //prescaler ativado com valor 64
    *p_osr2a =  249; //valor para ativar a interrupcao (a contagem deve parar quando atingir 250 vezes, visto que inicia do zero)
    *p_timsk2 = 0x02; //ativa a interrupcao relacionada a flag de comparacao do temporizador.
    
    //ativando interrupcao global
    sei();
}

void led_13(){     //funcao que muda o estado do led da placa conforme seu estado anterior
    if (*p_portb == 0x20 | *p_portb == 0x30){   //caso o led do pino 13 esteja ligado
        *p_portb &= 0b00010000;     // o led do pino 13 desliga sem interferir nos outros bits
    }
    else {      //caso o led esteja desligado
        *p_portb |= 0b00100000;      //liga o led sem interferir nos outros bits
    }
}
void led_12(){      //funcao que muda o estado do led conectado ao pino 12 conforme seu estado anterior
    if (*p_portb == 0x10 | *p_portb == 0x30){       //caso o led do pino 12 esteja ligado
        *p_portb &= 0b00100000;     // o led do pino 12 desliga sem interferir nos outros bits
    }
    else {      //caso o led esteja desligado
        *p_portb |= 0b00010000;         //liga o led sem interferir nos outros bits
    }
}

ISR(USART_TX_vect){
    if (msg[i] != '\0'){        //enquanto a mensagem for diferente de um caractere nulo (enquanto nao chegar o fim da mensagem)
        *p_udr0 = msg[i];       //sera inserido no registrador de transmissao o proximo caractere
        i++;        //incrementa a variavel de acesso do vetor
                //interessante lembrar que uma vez que o fim da mensagem chega, a interrupcao nao deixa de acontecer, entretanto ela nao ira imprimir nada.
                //a interrupcao so volta a imprimir alguma mensagem quando a variavel i for resetada.
    }
}





ISR(TIMER2_COMPA_vect){
	if (cont_led13 != 500){        //enquanto contador do led 1 nao atingir 500 (500ms) ele continuara a contagem das interrupcoes relacionadas ao led 1
		cont_led13 ++;     
	}
	else{
		cont_led13 = 0;         //zera contador
		led_13();       //funcao para trocar o estado do led 1
	}
	if (cont_led12 != 780){        //enquanto contador do led 2 nao atingir 780 (780ms) ele continuara a contagem das interrupcoes relacionadas ao led 2
		cont_led12 ++;              //incrementa
	}
	else{                //quando for
		cont_led12 = 0;   //zera contador
		led_12();       //funcao para trocar o estado do led 2
	}
}



int main(void) {

    setup();         //inicializa os registradores
    
    while (1) {
        *p_udr0 = msg[0];       //inicia o transmissor da mensagem
        i = 1;                  //resseta a mensagem a ser transmitida pela UART
    	_delay_ms(5000);        //Delay
    }
}
