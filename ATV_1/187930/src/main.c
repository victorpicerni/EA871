
/*VICTOR HUGO PICERNI PINTO FERREIRA
 * RA 187930
 *
 * Contador de bits
 * Este programa conta o numero de bits em um byte
 */

#include <stdio.h>
#define NUM_BITS 8 //essa linha foi inserida para nos dizer a quantidade de bits que será contabilizada, no caso um byte

int main() {

  /*definindo as variáveis*/
  unsigned char entrada;
  unsigned int tmp;
  unsigned int n_bits = 0;
  int teste;
  int i = 1;

  /* Ler entrada em hexadecimal */
  scanf("%x", &tmp); //le o que será escrito pelo teclado, em hexadecimal
  entrada = (unsigned int)tmp; //transforma o hexadecimal em um numero inteiro de zeros e uns
  
  
  /*contabilizar o valor em decimal*/
  while (i<= NUM_BITS) { //enquanto o valor de i for menor que o numero de bites ele continua o loop
  teste = 0; //zera a variavel que será testada
  teste = entrada % 2; //contabiliza o valor do bit caso o resto da divisao do numero por 2 resulte em 1

  /*incrementar caso o resto da divisão de zero*/
  if(teste == 1){ //se a variavel for um, o numero de bits iguais a 1 é incrementado
  n_bits++;
  }

  entrada = entrada / 2; //divide o valor da entrada por dois para o proximo loop
  i++; //incrementa a variável
  
  }  
  /* Escrever numero de bits */
  printf("%d\n", n_bits); //mostra na tela o numero total de bits

  return 0;
}
