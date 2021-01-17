# EA871
Material do curso EA871 - Laboratório de programação básica de sistemas digitais - 2º Semestre de 2020
EA-871
Repositório com os códigos e enunciados para os laboratórios da disciplina EA871. Os arquivos são referentes à disciplina oferecida no segundo semestre de 2019.

Links úteis
Link para o ambiente de desenvolvimento Atmel Studio 7: https://www.microchip.com/mplab/avr-support/atmel-studio-7

Link para a descrição do montador AVR: https://www.microchip.com/webdoc/avrassembler/index.html

Link para a descrição da biblioteca Libc, que disponibiliza rotinas em linguagem C para a programação de microcontroladores da Atmel - arquitetura AVR - de 8 bits. https://www.microchip.com/webdoc/GUID-90493D45-DB42-4BB8-9627-BAA0225A71E3/index.html

Link para a descrição de como preparar arquivos de estímulo para simulações no Atmel Studio https://www.microchip.com/webdoc/simulator/simulator.wb_Simulator2_Stimuli.html

Procedimento de configuração do Atmel Studio para carregar programas no Arduino https://medium.com/jungletronics/how-to-load-programs-to-an-arduino-uno-from-atmel-studio-7-83c8dd8d175d

Tinkercad (pode ser usado para simular o circuito digitalmente): https://www.tinkercad.com/

Resumo objetivos labs
Lab 01: bit counter
Objetivo: contar número de bits iguais a 1 em um byte recebido como entrada

Lab 02: buffer circular
Objetivo: implementar um buffer circular com tamanho 5

O buffer circular é uma estrutura de dados que é usada para manejar filas de dados que são recebidos de algum lugar no seu programa e pode ser pensado como se o vetor de dados estivesse posicionado sobre um círculo

Lab 03: GPIO
Objetivo: desenvolver um programa que faça com que o microcontrolador ATMega328P conte quantas vezes um botão táctil (push-button) foi pressionado, exibindo o valor atual da contagem em um display de 7 segmentos.

Lab 04: Interrupções
Objetivo: desenvolver um programa que controle um LED RGB, sendo sensível a comandos externos dados por dois botões.

O estado do LED RGB será alterado de maneira periódica pelo microcontrolador, passando por uma sequência pré-definida de estados.

Um botão terá efeito sobre a velocidade de transição entre os estados (a cada vez que for pressionado, o tempo que o LED permanece aceso deve ser reduzido pela metade até um valor mínimo). O outro botão controlará o sentido da transição de cores.

Lab 05: USART
Objetivo: desenvolver um programa que utilize a USART para transmitir indefinidamente uma sequência de caracteres pré-definida ao computador.

Cada vez que a sequência inteira de caracteres for transmitida, o programa deve exibir uma mensagem de confirmação: " Mensagem transmitida com sucesso! ". Um atraso de 5 s deve ser introduzido entre as transmissões.

Lab 06: Transmissão e recepção serial com interrupções
Objetivo: desenvolver um programa que utilize o receptor da UART para criar um sistema de acionamento de um LED RGB a paritr de comandos do teclado.

Também utilizar o transmissor da UART para encaminhar mensagens de confirmação e status.

A rotina associada à recepção utiliza um buffer circular.

Lab 07: Temporizadores
Objetivo: desenvolver um programa que faça com que dois LEDs pisquem em diferentes frequências, ao mesmo tempo em que uma mensagem de texto é continuamente transmitida pela UART.

A solução utiliza a interrupção do timer 2 associada ao modo Clear Timer on Compare Match (CTC).

Lab 08: PWM
Objetivo: desenvolver um programa que faça com que a intensidade luminosa de um LED seja controlada por um sinal PWM.

Lab 09: Assembly
Objetivo: elaborar uma rotina em linguagem Assembly para o ATMega328P que introduza um atraso temporal Escrever um programa em Assembly que faça com que um LED pisque com um período de 1 segundo (0,5 s aceso e 0,5 s apagado)

Lab 10:Robô


