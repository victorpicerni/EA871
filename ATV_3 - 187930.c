/*
 * File:   Atividade 3.c
 * Author: Victor Hugo Picerni
 * RA 187930
 * 
 * Created on 2 de Outubro de 2020, 11:53
 */

//Defini��es de fun��es e clock do microprocessador
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

//Criar ponteiros para GPIOs 2 a 13 (PORTD)
unsigned char* p_ddrd; //Ponteiro para definir a entrada ou saida
unsigned char* p_portd; //ponteiro para definir os valores de sa�da

//Criar ponteiros para entrada de sinal (PC0)
unsigned char* p_pinc; //ponteiro de leitura da entrada pelo GPIO
unsigned char* p_ddrc; //poneiro para configura��o de entrada ou saida
unsigned char* p_portc; //ponteiro para definir o pull up

void setup() { //mostra a quem os ponteiros ir�o apontar
    
    //Ponteiros apontando para os endere�os da PORTC
    p_pinc = (unsigned char *) 0x026; 
    p_ddrc = (unsigned char *) 0x027;
    p_portc = (unsigned char *) 0x028;
    
    //Ponteiros apontando para os endere�os de PORTD
    p_ddrd = (unsigned char *) 0x02A;
    p_portd =  (unsigned char *) 0x02B;
    
    //configurando DDRC0 como entrada (Nivel logico baixo)
    *p_ddrc = *p_ddrc | 0xFE; /*11111110*/

    //Ativando resistor pull up para PC0 (Nivel logico alto)
    *p_portc |= 0x01; /*00000001*/
    
    //configurando todas as portas de DDRD como saida (nivel logico alto))
    *p_ddrd = *p_ddrd | 0xFF; /*11111111*/
    
    //setando a inicializacao em zero 
    *p_portd = *p_portd | 0x7E; /*011111110*/
  	
}

int main(void) {
    
    int i = 0;  // Variavel auxiliar para informar quais portas terao nivel alto devido 
                // aos LEDs do display de 7 segmentos
    
    setup(); //Iniciando os registradores
      
    while (1) {
        
        if ((*p_pinc & 0x01) == 0x01){ /* Mascara de comparacao para botao nao pressionado 
                                       * (essa primeira comparacao e necessaria para que o botao passe apenas um numero por vez no
                                       * display; Caso contr�rio, os numeros iriam passar se o botao ficasse apertado)
                                       */
            _delay_ms(50);              //delay para estabilizar os valores logicos em caso de mudanca
            if ((*p_pinc & 0x01) == 0x00){  //M�scara de compara��o para botao pressionado
                _delay_ms(50);          //delay para debounce
                if ((*p_pinc & 0x01) == 0x00){      //M�scara de compara��o para descobrir se continuou pressionado (Passando o efeito debounce))
                    *p_portd = *p_portd | (0x01);   //Se o botao for pressionado, o Ponto e setado usando uma mascara/*00000001*/
                    i++;                //uma vez que o botao foi pressionado, a variavel auxiliar e incrementada
                }
            }
        }
        
        if ((*p_pinc & 0x01) == 0x00){      //Mascara para botao pressionado
            _delay_ms(50);              //delay para estabilizar os valores logicos em caso de mudan�a
            if ((*p_pinc & 0x01) == 0x01){      //M�scara de compara��o para botao solto
                _delay_ms(50);          //Delay para debouce
                if ((*p_pinc & 0x01) == 0x01){      //M�scara de compara��o para descobrir se continuou solto
                   *p_portd = *p_portd & (0x00);        //Se o bot�o foi solto, o ponto ir� para nivel l�gico baixo
                }
            }
        }
        
        //La�o para que a vari�vel auxiliar retorne a zero ap�s passar por F
        if (i ==16){
            i = 0;
        }

        //M�scara de reset
        *p_portd &= 0x01;         /* M�scara de compara��o com o ponto e uma das formas de ressetar os registradores para que seja 
                                   * implementado o nivel l�gico correto no display*/
        _delay_ms(5);           //Delay para que a transi��o fique menos aparente
        
        /* OBS: Tentei fazer de outra forma a m�scara acima, com um loop duplo, cheguei a conclus�o de que embora 
        * fosse mais rapida a comunica��o com o display, o mesmo apagava quando o botao ficava setado por muito tempo
        * if *p_pinc & 0x01) == 0x00){
        * *p_portd &= 0x01;         
        * _delay_ms(5);}
        * else {
        *  *p_portd &= 0x00;
        * _delay_ms(5);}
        */
        
        //M�scaras para setar as portas de saida para o display de 7 segmentos
        if (i == 0) {
            *p_portd = *p_portd | (0x7E); /*01111110*/
        }
        if (i == 1) {
			*p_portd = *p_portd | (0x0C); /*10110110*/
        }
        if (i == 2) {
           *p_portd = *p_portd | (0xB6); /*10110110*/
        }
        if (i == 3) {
            *p_portd = *p_portd | (0x9E); /*10011110*/
        }
        if (i == 4) {
            *p_portd = *p_portd | (0xCC); /*11001100*/
        }
        if (i == 5) {
            *p_portd = *p_portd | (0xDA); /*11011010*/
        }
        if (i == 6) {
            *p_portd = *p_portd | (0xFA); /*11111010*/
        }
        if (i == 7) {
            *p_portd = *p_portd | (0x0E); /*00001110*/
        }                
        if (i == 8) {
            *p_portd = *p_portd | (0xFE); /*11111110*/
        }
        if (i == 9) {
            *p_portd = *p_portd | (0xDE); /*11011110*/
        }
        if (i == 10) {
            *p_portd = *p_portd | (0xEE); /*11101110*/
        }
        if (i == 11) {
            *p_portd = *p_portd | (0xF8); /*11111000*/
        }        
        if (i == 12) {
            *p_portd = *p_portd | (0x72); /*01110010*/
        }
        if (i == 13) {
            *p_portd = *p_portd | (0xBC); /*10111100*/
        }
        if (i == 14) {
            *p_portd = *p_portd | (0xF2); /*11110010*/
        }
        if (i == 15) {
            *p_portd = *p_portd | (0xE2); /*11100010*/
        }          
    }
}