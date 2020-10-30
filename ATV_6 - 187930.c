/*
 * File:   main.c
 * Author: Victor Hugo
 *
 * Created on 29 de Outubro de 2020, 15:12
 */


#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

/* Tamanho do Buffer */
#define MAX_BUFFER 5

/* Buffer */
volatile char buffer[MAX_BUFFER];
/* Controle de posicao para adicionar valores no buffer */
volatile char add_buf = 0;
/* Controle de posicao para remover valores do buffer */
volatile char del_buf = 0;
/* Numero de posicoes usadas no buffer */
volatile char ocup_buf = 0;

volatile int i = 0;

char msg[100];

volatile int sinalizacao;


char msg_v [] = "Vazio!\n";
char msg_0 [] = "Comando: Apagar todos os LEDs\n";
char msg_1 [] = "Comando: Varredura com um LED aceso\n";
char msg_2 [] = "Comando: Varredura com um LED apagado\n";
char msg_3 [] = "Comando: Acender todos os LEDs\n";
char msg_inc [] = "Comando incorreto\n";

unsigned char* p_ubrrh = (unsigned char *) 0xC5;//ponteiro para definir os valores de saÃ­da
unsigned char* p_ubrrl = (unsigned char *) 0xC4; //Ponteiro para definir a entrada ou saida

unsigned char* p_ucsra = (unsigned char *) 0xC0; //ponteiro para definir UCSR0A
unsigned char* p_ucsrb = (unsigned char *) 0xC1; //ponteiro para definir UCSR0B
unsigned char* p_ucsrc = (unsigned char *) 0xC2; //ponteiro para definir UCSR0C
unsigned char* p_udr0 = (unsigned char*) 0xC6; //UDR0 (registrador que contem o caractere))

unsigned char* p_ddrc = (unsigned char *) 0x027;
unsigned char* p_portc = (unsigned char *) 0x028;

void setup(){
    cli();
        //Setando o valor do Baud Rate como 19.200
        *p_ubrrl = 0x33;
        *p_ubrrh = 0x00;
 
// ? Velocidade de transmissão normal (Bit 1 desativado))
// ? Modo de transmissão multi-processador desabilitado(bit 0 desativado))
    /*xxxxxx00*/
     *p_ucsra = 0b11100000;
      
// ? Todas as interrupções relacionadas à USART desabilitadas (bits 7, 6 e 5 desativados))
// RX desabilitado (bit 4) TX habilitado (bit 3)
// (processamento com 8 bits - UCSZ02 desativado))
     /*00001000*/
    *p_ucsrb = 0b11111000; /*11011000*/
  
// ? Modo assíncrono(Bits 7 e 6 desativados);
//paridade (par)(Bit 5 ativado e bit 4 desativado);   
// ? Uso de dois bits de parada (bit 3 ativado);
// 8 bit por frame (bits 2 e 1 ativados);
// bit 0 desativado para transmissão assincrona
    /*00000110*/
    *p_ucsrc = 0b00000110; /*00000110*/

    //configurando DDRB1 como Saída (Nivel logico alto)
    *p_ddrc = *p_ddrc | 0b00111000; /*11111101*/
    
    *p_portc = *p_portc & 0x00;
    //Ativando resistor pull up para PB1 (Nivel logico alto)
    //*p_portb |= 0x02; /*00000010*/   
    sei();
}

void adicionar_buffer(char c){
  
    /* Se o buffer nao esta cheio */
    if(ocup_buf < MAX_BUFFER){
        /* Adiciona valor no buffer */
      	buffer[add_buf] = c;
      	/* Incrementa o numero de posicoes utilizadas no buffer */
      	ocup_buf++;
        /* Incrementa condicionalmente o controle de posicao para adicionar.
           Se esta na ultima posicao, retorna pra primeira.
           Caso contrario, vai pra posicao seguinte. */
      	if(add_buf == (MAX_BUFFER-1)) add_buf=0;
      	else                          add_buf++;
    }
}

/* Funcao para remover valores do buffer */
char remover_buffer(){
    
    /* variavel auxiliar para capturar o caractere do buffer */
    char c;
  
    /* Se o buffer nao esta vazio */
    if (ocup_buf > 0){
      
    	/* Pega o caractere do buffer */
        c = buffer[del_buf];
      	/* Decrementa o numero de posicoes utilizadas no buffer */
      	ocup_buf--;        
        /* Incrementa condicionalmente o controle de posicao para remover.
           Se esta na ultima posicao, retorna pra primeira.
           Caso contrario, vai pra posicao seguinte. */
      	if(del_buf == (MAX_BUFFER-1)) del_buf=0;
      	else                    	  del_buf++;
    }
  
    return c;
}

ISR(USART_TX_vect){
        if (msg[i] != '\0'){
            *p_udr0 = msg[i];
            i++;
        }
}

ISR(USART_RX_vect){
    adicionar_buffer(*p_udr0);
}
void um_led(){
    *p_portc = 0x01;
    _delay_ms(500);
    *p_portc = 0x02;
    _delay_ms(500);
    *p_portc = 0x03;
    _delay_ms(500);
    *p_portc = 0x02;
    _delay_ms(500);
}
void dois_led(){
    *p_portc = 0x06;
    _delay_ms(500);
    *p_portc = 0x05;
    _delay_ms(500);
    *p_portc = 0x03;
    _delay_ms(500);
    *p_portc = 0x05;
    _delay_ms(500);
}
void tres_led(){
    *p_portc = 0x07;
    _delay_ms(500);
}
void zero_led(){
    *p_portc = 0;
    _delay_ms(500);
}

int main(void) {
    char c;
    
    setup();
    *p_ucsra |= 0b01000000; 
    while(1){
        if (ocup_buf == 0){
            strcpy(msg,msg_v);
            i = 0;
            *p_ucsra |= 0b01000000; 
        }
        else{
            c = remover_buffer();
            if (c == '0'){
                zero_led();
                strcpy(msg,msg_0);
                i = 0;
                *p_ucsra |= 0b01000000; 
            }
            else if (c == '1'){
                um_led();
                strcpy(msg,msg_1);
                i = 0;
                *p_ucsra |= 0b01000000; 
            }
            else if (c == '2'){
                dois_led();
                strcpy(msg,msg_2);
                i = 0;
                *p_ucsra |= 0b01000000; 
            }
            else if (c == '3'){
                tres_led();
                strcpy(msg,msg_3);
                i = 0;
                *p_ucsra |= 0b01000000; 
            }
            else {
                strcpy(msg,msg_inc);
                i = 0;
                *p_ucsra |= 0b01000000; 
            }
        }
    _delay_ms(500);
    }
    return 0;
}
