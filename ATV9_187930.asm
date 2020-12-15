;Nome: Victor Hugo Picerni
;RA 187930

.org 0x0000 ;
jmp main

.org 0x0004 ; interrupcao associada a INT1
rjmp rsi ;
        
.org 0x0034 ; 
main: 

cli ;
    /*Configurando o endereco do stacker pointer*/
ldi r16, 0xFF; carregando valor FF em R16 para enviar para SPL
sts 0x5D, r16; enviando o valor de R16 para o registrador stack pointer low SPL
ldi r16, 0x08; carregando o valor 08 em R16 para enviar para SPH
sts 0x5E, r16; enviando o valor de R16 para o registrador stack pointer high SPH


    /*Configurando EICRA*/

    ldi r16, 0x08 ; carregando o valor 8 em R16 para enviar para EICRA 
    sts EICRA, r16; enviando o valor de R16 para EICRA (lembrando o endereco de EICRA = 0x69)
             ;Configuração para interrupcao na borda de descida
                     
    /*EIMSK*/
    sbi EIMSK, 1 ;EIMSK ira habilitar a geracao de interrupcao a partir de INT1 
    
    /*configurando as portas de entrada e saida*/
    sbi DDRB,5 ; configurando o bit 5 como saida
    cbi PORTB,0 ; Iniciando led apagado
            
    cbi DDRD,3 ; configurando porta 3 como entrada
    sbi PORTD,3 ; configurando resistor pullup

    
    sei ; ativa as interrupcoes
    
    /*loop infinito*/
    loop_infinito:
    ldi r17, 1; 
    call led;
    call atraso1 ; chamando a subrotina de atraso
    ldi r17, 2;
    call led; 
    call atraso1 ; chamando a subrotina de atraso
    rjmp loop_infinito ; utilizada para retornar ao inicio do loop
    
            
    /*rotina de servico de interrupcao*/
    ;utilizamos na rotina de interrupcao uma subrotina de tal forma que ela seja executada 3 vezes, além de uma quarta vez apagando o led
    
    rsi:
    cbi 0x05, 5 ; apaga o led
    call atraso2 ; rotina de atraso para 100ms
    sbi 0x05, 5 ; acende o led
    call atraso2; rotina de atraso para 100ms
    cbi 0x05, 5 ; apaga o led
    call atraso2; rotina de atraso para 100ms
    sbi 0x05, 5 ; acende o led
    call atraso2; rotina de atraso para 100ms
    cbi 0x05, 5 ; apaga o led
    call atraso2; rotina de atraso para 100ms
    sbi 0x05, 5 ; acende o led
    call atraso2; rotina de atraso para 100ms
    cbi 0x05, 5 ; apaga o led
    call atraso2; rotina de atraso para 100ms   
    sbi EIFR, 1; zerando EIFR para debouce
    reti; retorna ao programa original, saindo da interrupcao
    
    /*rotina para acender e apagar o led*/
    
    led:
    cpi r17,1; comparacao com o registrador de estado do led
    brbs 1, acender_led; acender led de acordo com o valor do r17
    cpi r17,2; comparacao com o registrador de estado do led
    brbs 1, apagar_led; apagar led de acordo com o valor do r17
    ret; retornar ao programa corrente
    
    acender_led:
    sbi 0x05, 5 ; acende o led
    ret; retorna ao programa corrente
    
    apagar_led:
    cbi 0x05,  5; apaga o led
    ret; retorna ao programa corrente
    
    
   

/*rotina de atraso para 100ms*/
; A rotina utilizada em ambos os atrasos foi igual, mudando apenas os parametros e dispensando os comentarios. (a explicação estara mais abaixo, na rotina de atraso para 1s)
;a unica mudanca foram os parametros utilizados para que os valores fossem correspondentes aos exigidos
; utilizamos agora outra maneira de fazer a conta, de forma que os 3 loops tivessem a mesma quantidade de loops        
;temos entao : 5x^3 +6*x^2 + 6*x = 1600000 => 68

atraso2:
    clr r21 ;
    clr r22 ;
    clr r23 ;
parte2_2:
    inc r21 ;
    cpi r21,68 ;
    brbs 1, parte3_2 ;
    rjmp parte2_2 ;
parte3_2:
    clr r21;
    inc r22 ;
    cpi r22,68 ;
    brbs 1, parte4_2; 
    rjmp parte2_2 ;
parte4_2:
    clr r22;
    inc r23 ;
    cpi r23,68 ;
    brbs 1, final_2 ;
    rjmp parte2_2 ;

final_2:
    ret ;

    
     /*rotina de atraso para led piscando com frequencia de 2Hz*/
; vamos utilizar 3 registradores com um maximo de 256 posicoes cada, para contagem dos loops e consequentemente do tempo de execucao 
; faremos 3 loops para contagem, de forma que a segunda parte sera incrementada quando a quantidade de loops da primeira parte chegar no valor estipulado, que sera calculado a seguir
; para calcular os valores, foi tomado por base a quantidade de ciclos que cada instrucao consome, conforme o manual. tais ciclos se encontram a frente de cada instrucao
; para a primeira parte consideramos o valor 200, ja para a segunda consideramos 150
; por fim, o ultimo valor sera 5*200*150*x +(6+7)*150*x + 6*x = 16M
; realizando essa conta chagamos em um valor de 105 loops no final.
;utilizaremos o ultimo loop para explicar o funcionamento dessa parte, visto que os loops sao iguais

atraso1:

    clr r18 ; zera os registradores que serao utilizados
    clr r19 ;
    clr r20 ;
    
parte1:
    inc r18 ; (1 ciclo)
    cpi r18,200 ; (1 ciclo)
    brbs 1, parte2 ; (1 ciclo para falso, 2 ciclos para verdadeiro)
    rjmp parte1 ; (2 ciclos)
; total 5 ciclos considerados

parte2:
    call led; (media de 7 ciclos) subrotina inserida para retornar o estado do led caso ocorra uma interrupcao
    clr r18; (1 ciclo)
    inc r19 ;(1 ciclo)
    cpi r19,150 ;(1 ciclo)
    brbs 1, parte3; (1 ciclo para falso, 2 ciclos para verdadeiro)
    rjmp parte1 ; (2 ciclos)
;total 6 ciclos considerados

parte3:
    clr r19; zera o registrador de comparacao anterior, visto que foi pelo valor dele que chegamos nesse loop e ele precisa ser zerado para a proxima contagem
    inc r20 ; incrementa o registrador associado a contagem do loop atual
    cpi r20,105 ; compara o registrador ao valor calculado
    brbs 1, final ; caso a flag (Z) esteja setada, o loop termina.
    rjmp parte1 ; caso contrário, o programa entrara no proximo loop iniciando pela primeira parte
;total 6 ciclos considerados

final:
    ret ;