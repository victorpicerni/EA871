/*
 * File:   main.c
 * Author: Victor Hugo Picerni
 * RA187930
 * Created on 26 de Novembro de 2020, 15:21
 */
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

    //Obs: o codigo foi feito com auxilio da placa, sem a utilizacao do tinkercad

  // Primeiramente, vamos definir o compare match para OCR2B
  // além disso, vamos configurar a placa para modo PWM além de ter a limitação em OCRA. (Interrupcao TIMER2_COMPA)
  // Para configuracao em fase PWM os bits de Waveform Generation Mode (WGM) estão todos setados (bits 0 e 1 de TCCR2A e bit 3 de TCCR2B)
  // Para configuração do prescaler, escolhemos 256, a partir disso F = 256*1/16M = 16*10^(-6)s
  // Com isso, esolhemos 250 para o valor de comparação com o contador, de forma que colocamos 249 em OCR2A
  // por fim, temos T = 1s, logo
  // T = F*(249+1)*Duty Cicle
  // Descobrimos então o dudy cicle igual a 250
    
  /*Definindo os registradores*/

unsigned char* p_ocr2a = (unsigned char *) 0x0B3;  //ponteiro para definir OCR2A
unsigned char* p_ocr2b = (unsigned char *) 0x0B4;  //ponteiro para definir OCR2B

unsigned char* p_ddrd = (unsigned char *) 0x02A; //ponteiro para configuracao do led
unsigned char* p_portd = (unsigned char *) 0x02B; //ponteiro para apagar os leds

unsigned char* p_tccr2a = (unsigned char *) 0x0B0; //ponteiro para definir TCCR2A 
unsigned char* p_tccr2b = (unsigned char *) 0x0B1;  //ponteiro para definir TCCR2B
unsigned char* p_timsk2 = (unsigned char *) 0x070;  //ponteiro para definir TIMSK2

volatile int controle = 0;

  /*inicializacao*/
void setup(){
    cli(); //desativando as interrupcoes
   
    *p_tccr2a |= 0b00100011; //Ativando o Compare Match para OCR2B e setando waveform escolhida
    *p_tccr2b |= 0b00001110; //prescaler ativado com valor 256
    *p_timsk2 |= 0x02; //ativa a interrupcao relacionada a flag de comparacao com OCRA do temporizador.
    
    *p_ocr2a =  249; //Valor para comparação com o contador
    
    *p_ddrd = *p_ddrd | 0x08; //configurando DDRD1 como Saída (Nivel logico alto) para pino 3
    *p_portd = *p_portd | 0x00;//Setando os leds para Inicializacao com eles apagados.
    DDRB |= 0x20; //Configurando DDRB5 (pino 13) como saida
    sei(); //ativando interrupcoes
}

  /*Rotina de interrupcao*/
 //Para a rotina de interrupcao, introduzimos a variavel de controle para poder incrementar o valor do duty cicle até seu valor máximo

ISR(TIMER2_COMPA_vect){ //interrupcao de comparacao 
  if (controle == 0){ //se led está acendendo
    PORTB |= 0x20; //mascara para acender led da placa
    if ((*p_ocr2b) != 250){ // e OCR2B não é igual a 250
      (*p_ocr2b) ++; //incrementar
    }
    else{
      controle = 1;
    } //quando OCR2B = 250, trocar valor da variável controle
  }
  else{//se led está apagando
    PORTB &= 0x00; //mascara para apagar led da placa
    if ((*p_ocr2b) != 0){ //e OCR2B não é 0.
      (*p_ocr2b) --;//decrementar
    }
    else{
      controle = 0;
    }//quando OCR2B = 0, trocar valor da variável controle
  }
}

int main(void) {
    
  setup(); //inicializacao dos registradores
  while(1){
  }
}
