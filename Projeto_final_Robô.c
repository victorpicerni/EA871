/*
 * Author: Victor Hugo - Turma U
 * RA: 187930
 * 
 * Author: Giulia Steck - Turma W
 * RA: 173458
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/*Variaveis*/
volatile int contagem_interrupcao; //percorrer o vetor de mensagens para enviar os caracteres para a interface serial
volatile char comando; //comandos da interface serial e dos motores
volatile int aux_comando;//variavel auxiliar para continuar em caso de caractere invalido 
volatile int cont_tempo; //contagem de tempo para mensagens (enviar em um intervalo de 1s))
volatile int cont_distancia; //contagem de tempo para envio do sinal ao sensor ultrasonico
volatile int aux_interrupcao_externa; //variavel de estado para iniciar a contagem do tempo relacionada ao sensor ultrasonico
volatile int tempo_distancia; //contagem do tempo relacionada a distancia
volatile int distancia; //distancia em cm
volatile int distancia_1; //centena da distancia
volatile int distancia_2; //dezena da distancia
volatile int distancia_3; //unidade da distancia
volatile int led;
volatile int velocidade_led;

char msg[40]; //Criação de um vetor para as mensagens de forma a nao sobrecarregar muito a interrupcao. Dessa forma, a interrupcao tera poucas linhas de codigo.
                //a criacao de um vetor com 40 posicoes deve-se ao fato de as mensagens terem menos de 100 caracteres, dessa forma com certeza todas as mensagens seriam transmitidas em sua totalizada
                //Caso a memória estivesse demasiadamente ocupada, o vetor msg poderia ser diminuido para apenas as posicoes necessárias para os caracteres da maior mensagem que será repassada

/*Mensagens a serem impresssas*/
//criacao de um vetor para cada mensagem.
char msg_w [] = "FRENTE\n";
char msg_o [] = "OBSTACULO!\n";
char msg_s [] = "TRAS\n";
char msg_a [] = "ANTI-HORARIO\n";
char msg_d [] = "HORARIO\n";
char msg_q [] = "PARADO\n";
char msg_e [] = "DDDcm\n";
char msg_6 [] = "Velocidade 60%\n";
char msg_8 [] = "Velocidade 80%\n";
char msg_0 [] = "Velocidade 100%\n";

void setup(){
    cli();
  
    //Setando o valor do Baud Rate como 9.600
  	UBRR0L = 0x67;
  	UBRR0H = 0x00;
  
  /*REGS DA UART CONFIGURADOS CORRETAMENTE*/
  //Configurando registrador para transmissao normal e assincrona; Multiprocessador desabilitado   
    UCSR0A = 0b00000000;
      
  //Interrupcoes de Trasnmissao completa ativadas e buffer vazio desativada; Recepcao e transmissao habilitadas
    UCSR0B = 0b11011000;
  
  //Configurando Modo assincrono, sem bit de paridade, 1 bit de parada e uso de 8 bits por frame
    UCSR0C = 0b00000110; /*00000110*/

    //configurando DDRC como Saída (Nivel logico alto)
    DDRC |= 0b00111111; /*XXX11111*/
   
    //Setando motores para Inicializacao com eles desligados.
    PORTC|= 0x00; //motor inicialmente parado
	DDRD |= 0x08;//porta configurada para saida
  	PORTD = 0x08;//inicio com motor ligado
  	OCR2B = 149; //inicio com motor a 60% da velocidade
    
    /*NAO MEXER NESSE - JA ESTA CERTO*/ /*temporizador para contagem com interrupcao a cada  da mensagem transmitida pela interface serial*/
  	OCR0A =  24; //valor para ativar a interrupcao (a contagem deve parar quando atingir 25 vezes, visto que inicia do zero)
  	TIMSK0 = 0x02; //ativa a interrupcao relacionada a flag de comparacao do temporizador. 
  	TCCR0B = 0x03; //prescaler ativado com valor 64
  	TCCR0A =  0x02;    
  	
    
  /*temporizador do PWM*/
    OCR2A =  249; //valor para ativar a interrupcao (a contagem deve parar quando atingir 250 vezes, visto que inicia do zero)
    TIMSK2 = 0x02; //ativa a interrupcao relacionada a flag de comparacao do temporizador.
	TCCR2B = 0x0B; //prescaler ativado com valor 32
 	TCCR2A =  0x23; //Ativando o Compare Match para OCR2B e setando waveform escolhida
            
  /*Interrupcao externa INT0*/
    EICRA = 0x01; //seta a forma de interrupcao para ocorrer a cada mudançca de nivel lógico
    EIMSK = 0x01; //ativa a interrupcao associada a INT0
    
    
    //ativando interrupcao global
    sei();
}

/*PRONTO*/ /*Funcao da interrupcao relacionada a transmissao*/
ISR(USART_TX_vect){
        if (msg[contagem_interrupcao] != '\0'){ //enquanto a mensagem for diferente de um caractere nulo (enquanto nao chegar o fim da mensagem)
            UDR0 = msg[contagem_interrupcao]; //será inserido no registrador de transmissao o próximo caractere
            contagem_interrupcao++;  //incrementa a variavel de acesso do vetor
            //interessante lembrar que uma vez que o fim da mensagem chega, a interrupcao nao deixa de acontecer, entretanto ela nao ira imprimir nada.
            //a interrupcao só volta a imprimir alguma mensagem quando a variavel i for resetada.
        }
}

/*PRONTO*/  /*Funcao de interrupcao relacionada a recepcao*/
ISR(USART_RX_vect){
    comando = UDR0; //introduz o caracter recebido na variavel comando
    comando_uart(comando); //comando ocorre sem variação de 1s, de forma mais rapida, apenas a mensagem irá ter a variacao
}

/*PRONTO*/ /*timer para contagem do tempo das mensagens da uart*/
ISR(TIMER0_COMPA_vect){
  
  if (cont_tempo != 10000){        //enquanto contador nao atingir 10000 (1.000.000*us) ele continuara a contagem das interrupcoes relacionadas ao temporizador
    cont_tempo ++;     
  }
  else{
        mensagem_uart(comando); //funcao de envio de mensagem para a UART
        cont_tempo = 0; //zera a contagem 
  }
  
  if (cont_distancia != 2000){ //enquanto o contador nao atingir 2000 looops, (200ms) a funcao é incrementada
  	cont_distancia ++; 
	PORTC &= 0xFE; //envio de sinal igual a zero
  }
  else{ //quando atingir o valor desejado
  	cont_distancia = 0;
    PORTC |= 0x01; //envio de sinal igual a um
  }
  
  if (aux_interrupcao_externa == 1){ //contagem de ciclos da para saber a distancia relacionada ao sensor ultrassonico
	tempo_distancia++; //incrementa a variavel de tempo para o sensor ultrassonico
  }

  pisca_led();
}



ISR(INT0_vect){ //interrupcao externa associada ao Pino 2
  
    if (aux_interrupcao_externa == 0){//estado zero
        tempo_distancia = 0; //zera a contagem de tempo para descobrir a distancia
        aux_interrupcao_externa = 1;  //vai para o estado 1
    }
    else {
        aux_interrupcao_externa = 0; //volta para o estado zero
      	distancia = (tempo_distancia*(3.43))/2; //calcula a distancia a partir do tempo obtido
        indicar_distancia(); //transforma a distancia para o padrao ASCII
    }
}

/*falta configurar o timer para contagem a partir da interrupcao de borda de subida e descida*/



 /*Timer para sinal PWM*/
  // Para configuração do prescaler temos 32, a partir disso F = 32*1/16M = 2*10^(-6)s
  // Com isso, esolhemos 250 para o valor de comparação com o contador, de forma que colocamos 49 em OCR2A
  // por fim, temos T = 2*10^(-3) -> 2kHz, logo
  // T = 32/16M*(249+1)
  // Logo temos a comparacao

  /*Rotina de interrupcao*/
 //Para a rotina de interrupcao, introduzimos a variavel de controle para poder incrementar o valor do duty cicle até seu valor máximo
ISR(TIMER2_COMPA_vect){ //interrupcao de comparacao 
  if (comando == '6'){ //se led está acendendo
    OCR2B = 149; //mascara para acender led da placa
  }
  else if (comando == '8'){ //se led está acendendo
    OCR2B = 199; //mascara para acender led da placa
  }
  else if (comando == '0'){ //se led está acendendo
    OCR2B = 249; //mascara para acender led da placa
  }
}

//OBS IMPORTANTE: o funcionamento correto do motor com duty cycle correto deixava o tinkercad mais lento pelo maior processamento
//dessa forma, coloquei ele pra fazer com overflow que terá um tempo mais alto
//caso seja necessario fazer por comparacao com OCR2A, mudar OCR2B para 0x0B


/*acabar de fazer funcoes*/
/*comandos uart - nao esta pronto - tem q fazer as funcoes*/
int comando_uart(int c){
    if ((c == 'w')&&(distancia>10)){ 
      frente(); //funcao para motor ir para frente
    }
    else if ((c == 'w')&&(distancia<=10)){
  		parar();
  	}
    else if (c == 's'){ 
        tras(); 
    }
    else if (c == 'a'){  
      anti_horario(); 
    }
    else if (c == 'd'){ 
      horario(); 
        }
    else if ((c == 'q')|| (c == '\0')){ 
      parar();
    }
    else if (c == 'e'){ 
  		//indicar_distancia();
    }
    else if (c == '6'){ 
  		//pwm 60% 
    	//a propria interrupcao ja faz a funcao
    }
    else if (c == '8'){ 
  		//pwm 80%
    }   
    else if (c == '0'){ 
  		//pwm 100%
    }
    else{ //loop para outros caracteres que nao sao validos 
        comando = aux_comando; //retorna a variavel anterior
        comando_uart(comando); //chama a funcao comando
    }
    aux_comando = comando; //guarda a variavel comando
    return 0;
}




/*MENSAGEM - AINDA NAO ESTA PRONTO*/
/*falta colocar DDDcm*/
int mensagem_uart(int c){
    if ((c == 'w')&&(distancia>10)){ //leitura do caracter contido em comando (ultimo caractere valido)
        strcpy(msg,msg_w); //o vetor de mensagens (msg) sera igual a msg_W
    }
  else if ((c == 'w')&&(distancia<=10)){ //caso a distancia seja menor que 10cm, a mensagem de obstaculo é enviada para a interface serial
  		strcpy(msg,msg_o);
  	}
  
    else if (c == 's'){ 
        strcpy(msg,msg_s); 
    }
    else if (c == 'a'){ 
        strcpy(msg,msg_a); 
    }
    else if (c == 'd'){ 
            strcpy(msg,msg_d); 
    }
    else if ((c == 'q')|| (c == '\0')){ // mensagem para carrinho parado
            strcpy(msg,msg_q); 
    }
    else if (c == 'e'){ //mensagem com a distancia do obstaculo
            strcpy(msg,msg_e);
            msg[0] = (char) distancia_1;
            msg[1] = (char) distancia_2;
            msg[2] = (char) distancia_3;
   
    }
    else if (c == '6'){ 
            strcpy(msg,msg_6);
      		         
    }
    else if (c == '8'){ 
            strcpy(msg,msg_8); 
    }   
    else if (c == '0'){ 
            strcpy(msg,msg_0); 
    }         
    contagem_interrupcao = 1; //reinicia a contagem na interrupcao para enviar os caracteres da mensagem
    UDR0 = msg[0]; //na primeira vez que ocorrer o loop, o registrador de indicacao de transmissao completa nao estara ativado, assim foi colocado esse primeiro caractere para que ele fosse setado e assim inicializando a interrupcao
 

    return 0;
}

int pisca_led(void){
velocidade_led = distancia*25;
  
  if (led < velocidade_led){
  led++;
    PORTC |= 0x20;
  }
  else if (led < 2*velocidade_led){
  led++;
    PORTC &= 0xDF;
  }
  else {
  led = 0;
  }
return 0;
}


int frente(){ //funcao para ir para frente
  PORTC &= 0xE1;
  PORTC|= 0x0A;
  return 0;
}

int tras(){ //funcao para ir para tras
  PORTC &= 0xE1;
  PORTC|= 0x14;
  return 0;
}

int anti_horario(){ //funcao para sentido anti-horario
  PORTC &= 0xE1;
  PORTC |= 0x12;
  return 0;
}

int horario(){ //funcao para sentido horario
  PORTC &= 0xE1;
  PORTC|= 0x0C;
  return 0;
}

int parar(){ //funcao para parar
  PORTC &= 0xE1;
  return 0;
}

int indicar_distancia(){ //funcao para indicar a distancia relacionada ao obstaculo a frente
    distancia_1 = 0.01*distancia;
    distancia_1 += 48;
    distancia_2 = 0.1*(distancia%100) ;
    distancia_2 += 48;
    distancia_3 = 1*(distancia%10);
    distancia_3 += 48;
    
}


int main(void) {
    
    setup(); //inicializacao dos registradores
    
    while(1){ //loop infinito
	
    }
    return 0;
}