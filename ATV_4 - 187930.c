        /*
         * File:   main.c
         * Author: Victor Hugo Picerni
         * RA 187930
         * Created on 15 de Outubro de 2020, 14:20
         */



        #include <avr/io.h>
        #define F_CPU 16000000UL
        #include <util/delay.h>
        #include <avr/interrupt.h>

        #define INTERVALO 100

        //Criar ponteiros para GPIOs 2 a 13 (PORTD)
        unsigned char* p_ddrd; //Ponteiro para definir a entrada ou saida
        unsigned char* p_portd; //ponteiro para definir os valores de saída

        //unsigned char* p_sreg;
        unsigned char* p_pcicr; //Ponteiro para habilitar as interrupçoes
        unsigned char* p_pcice; //ponteiro para habilitar a leitura de interrupcoes em PCINT8
        unsigned char* p_pcmsk; //Ponteiro para habilitar o disparo de interrupcoes
        unsigned char* p_pcifr; //Ponteiro para obter o sinal de disparo da interrupcao
        int aux = 0;

        void setup (){
            p_ddrd = (unsigned char *) 0x02A; //Setando o ponteiro com o endereço de DDRD
            p_portd =  (unsigned char *) 0x02B; //Setando o ponteiro com o endereço de PORTD

            //p_sreg = (unsigned char *) 0x5F;
            p_pcicr = (unsigned char *) 0x068; //Setando o ponteiro para habilitar as interrupcoes geradas pelos pinos de 8 a 14
            p_pcmsk = (unsigned char *) 0x06C; //Setando o ponteiro para PCMSK1 

            //configurando todas as portas de DDRD como saida (nivel logico alto))
            *p_ddrd = *p_ddrd | 0xFE; /*11111110*/

            //Configurando porta pcicr para habilitar as interrup��es dos pinos 8 a 14
            *p_pcicr = *p_pcicr | 0x02; /*00000010*/

            //configurando PCMSK para habilitar o disparo de interrupcoes
            *p_pcmsk = *p_pcmsk | 0x01; /*00000001*/


        }

        //Nesse ponto foram feitas duas funcoes basicas apenas para facilitar o entendimento do codigo

        void ativar_interrupcao(){ //ativa a flag de interrupcao em SREG
            sei();//funcao para flag global tornando possivel o atendimento de interrupcoes 
        }
        void desativar_interrupcao(){ //desativa a flag de interrupcao em SREG
            cli();//desabilita o atendimento de interrupcoes

        }

         ISR(PCINT1_vect){ //Rotina de interrupcao que ocorre a cada mudanca de nivel logico da entrada da interrupcao (quando PCIF1 = 1)
                aux++;//incrementa a variavel que conta cada mudanca de nivel logico. Vale ressaltar que um pulso gera dois incrementos, um para subida e um para descida.
                sei(); //seta novamente a flag de interrupcao, uma vez que o estado dela nao e salvo na pilha 
            }

        int main(void) {
            int rpm; //variável que irá computar a frequencia em rpm

            setup(); //configuração dos registradores

            while(1){ //loop infinito
                aux = 0; //a cada loop, a variavel auxiliar sera resetada para computar a frequencia em tempo real.

                ativar_interrupcao(); //ativa a interrupcao global
               //obs: há uma pequena variação entre as leituras relacionadas ao intervalo, bem como a rotação do motor, de forma que não é totalmente preciso, entretanto para ter um pouco mais de precisão
               // é possivel descontar 3 milisegundos correspondentes a uma possivel variação (considerando 3% de variacao).
               //_delay_ms(INTERVALO-3);
                _delay_ms(INTERVALO); //intervalo em que o programa nao ira rodar, entretanto a interrupcao continua valendo, por ser uma parte paralela ao programa, logo a cada pulso a interrupcao ainda ira incrementar nossa variavel auxiliar
               
                desativar_interrupcao(); //desativa a interrupcao, para nao ficar incrementando a variavel auxiliar

                rpm = aux*60000/(96*INTERVALO); //encontra a frequencia pela equacao dada
                //rpm = rpm/2; //divide por 2 para contar apenas uma vez cada pulso (lembrando que cada pulso e contado duas vezes, pela variacao do nivel logico)

                *p_portd &= 0x00; //zera o display


                //Conforme a frequencia RPM obtida, sera mostrado no display um valor especifico previamente dado. aqui estamos selecionando qual valor sera mostrado no display
                if (rpm < 100) {
                    *p_portd = *p_portd | (0x7E); /*0 - 01111110*/
                }
                if (100 <= rpm && rpm <200) {
                    *p_portd |= 0x0C; /*1 - 10110110*/
                }
                if (200 <= rpm && rpm <300) {
                    *p_portd = *p_portd | (0xB6); /*2 - 10110110*/
                }
                if (300 <= rpm && rpm <400) {
                    *p_portd = *p_portd | (0x9E); /*3 - 10011110*/
                }
                if (400 <= rpm && rpm <500) {
                    *p_portd = *p_portd | (0xCC); /*4 - 11001100*/
                }
                if (500 <= rpm && rpm <600) {
                    *p_portd = *p_portd | (0xDA); /*5 - 11011010*/
                }
                if (600 <= rpm && rpm <700) {
                    *p_portd = *p_portd | (0xFA); /*6 - 11111010*/
                }
                if (700 <= rpm && rpm <800) {
                    *p_portd = *p_portd | (0x0E); /*7 - 00001110*/
                }                
                if (800 <= rpm && rpm <900) {
                    *p_portd = *p_portd | (0xFE); /*8 - 11111110*/
                }
                if (rpm >= 900) {
                    *p_portd = *p_portd | (0xDE); /*9 - 11011110*/
                }

                //_delay_ms(50); //tempo extra para ter tempo do display apagar, sem que fiquem dois numeros por um instante
            }

            return 0;
        }
