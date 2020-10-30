/*
 * File:   main.c
 * Author: Victor Hugo
 *
 * Created on 22 de Outubro de 2020, 10:33
 */

//Defini��es e inclusao de bibliotecas e clock do microprocessador
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

 
//Criar ponteiros para GPIOs 2 a 13 (PORTD)
unsigned char* p_ubrrh = (unsigned char *) 0xC5;//ponteiro para definir os valores de saída
unsigned char* p_ubrrl = (unsigned char *) 0xC4; //Ponteiro para definir a entrada ou saida

unsigned char* p_ucsra = (unsigned char *) 0xC0; //ponteiro para definir UCSR0A
unsigned char* p_ucsrb = (unsigned char *) 0xC1; //ponteiro para definir UCSR0B
unsigned char* p_ucsrc = (unsigned char *) 0xC2; //ponteiro para definir UCSR0C
unsigned char* udr = (unsigned char*) 0xC6; //UDR0 (registrador que contem o caractere))

//Criar ponteiros para entrada de sinal (PB1)
unsigned char* p_pinb = (unsigned char *) 0x023; //ponteiro de leitura da entrada pelo GPIO
unsigned char* p_ddrb = (unsigned char *) 0x024; //poneiro para configuracao de entrada ou saida
unsigned char* p_portb = (unsigned char *) 0x025; //ponteiro para definir o pull up


char msg_np[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao. O botao nao esta pressionado.\n\n";    
char msg_p[] = "Transmissao serial utilizando a USART: verificacao do termino da transmissao por varredura. O botao esta pressionado.\n\n";
unsigned int cont;  
unsigned int botao;

   
    void setup(){
//Setando o valor do Baud Rate como 115.200
        *p_ubrrl = 0x08;
        *p_ubrrh = 0x00;
 
// Velocidade de transmissao normal (Bit 1 desativado))
// Modo de transmissao multi-processador desabilitado(bit 0 desativado))
     *p_ucsra = 0x20; /*00100000*/
      
// Todas as interrupcoes relacionadas a USART desabilitadas (bits 7, 6 e 5 desativados))
// RX desabilitado (bit 4) TX habilitado (bit 3)
// (processamento com 8 bits - UCSZ02 desativado))
    *p_ucsrb = 0x08; /*00001000*/
 

// Modo assincrono(Bits 7 e 6 desativados);
// paridade (par)(Bit 5 ativado e bit 4 desativado);   
// Uso de dois bits de parada (bit 3 ativado);
// 8 bit por frame (bits 2 e 1 ativados);
// bit 0 desativado para transmissao assincrona
    *p_ucsrc = 0b00101110; /*00101110*/

//configurando DDRB1 como entrada (Nivel logico baixo)
    *p_ddrb = *p_ddrb & 0xFD; /*11111101*/

//Ativando resistor pull up para PB1 (Nivel logico alto)
    *p_portb = *p_portb | 0x02; /*00000010*/   
}

void botao_nao_pressionado (){ //funcao para botao nao pressionado
    cont = 0; //reinicia a passagem pelo vetor
    while (msg_np[cont]!= '\0'){ //enquanto nao chegar no final da frase, o loop continua
        while (!(*p_ucsra & 0x20)){};//enquanto o bit de indicacao de registrador vazio nao for setado, o programa nao continua
        *udr = msg_np[cont]; // coloca o proximo caractere no registrador vazio   
        cont++; //incrementa contador da posicao do vetor de caracteres
    }
}

void botao_pressionado(){
    cont = 0;//reinicia a passagem pelo vetor
    while (msg_p[cont]!= '\0'){ //enquanto nao chegar no final da frase, o loop continua
         while (!(*p_ucsra & 0x20)){};//enquanto o bit de indicacao de registrador vazio nao for setado, o programa nao continua
        *udr = msg_p[cont]; // coloca o proximo caractere no registrador vazio 
        cont++; //incrementa contador da posicao do vetor de caracteres
    }   
}

int main(void) {
    
    setup(); //Inicia os valores dos registradores
  
    while (1){   //loop infinito
        
        if ((*p_pinb & 0x02)==0x02) { //Se o botao nao estiver pressionado, o nivel logico de PINB sera 1
            _delay_ms(50); //delay para debounce
            if ((*p_pinb & 0x02)==0x02){ 
                 botao_nao_pressionado();//funcao de botao nao pressionado
            }

        }
        
        if ((*p_pinb & 0x02) != 0x02){ // quando o botao for pressionado, o nivel logico sera zero
            _delay_ms(50); //delay para debounce
            if ((*p_pinb & 0x02) != 0x02){
                botao_pressionado();   //funcao de botao pressionado
            }   
        }
        
    _delay_ms(500); //delay pre estabelecido
    
    }
}