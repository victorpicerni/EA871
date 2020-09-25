 /*Victor Hugo Picerni 
 * RA 187930
 *
 *  Teste de um buffer circular
 * Este programa serve para implementar e testar uma implementacao de buffer
 * circular
 */

#include <stdio.h>
#include <ctype.h>

char buffer[4];          //Definindo o tamanho do buffer, lembrando que seu indice começa em 0
int i = 0;               //Definindo indice que aponta para a próxima posição lire no vetor
int j = 0;                //Definindo indice que aponta para a posição mais antiga do vetor
int num_elementos = 0;    //Definindo numero de elementos que estão dentro do vetor

void adicionar_buffer(char c) {  //Função para adicionar um caractere ao vetor
if (num_elementos < 5){           // o numero de elementos pode ser no máximo 5, dessa forma, a funçao irá funcionar apenas para uma quantidade de elementos menor que 5 dentro do vetor 
  buffer[i] = c;                  //Injeta o caractere no buffer de posição i
  i = ((i + 1) % 5);              //permite que i esteja entre 0 e 4
  num_elementos++;                //incrementa o numero de elementos no vetor
  }

}

void remover_buffer() {         //Função para remover um caractere ao vetor
if (num_elementos > 0) {        //o numero minimo de elementos é zero, logo a função só irá funcionar para vetores com numero positivo de caracteres
  buffer[j] = '0';              //retira o valor da ultima posição
  j = (j + 1) % 5;              //permite que j esteja entre 0 e 4
  num_elementos--;              //decrementa o numero de elementos do vetor
  }
}

void imprimir_buffer() {        //função para imprimir os caracteres do vetor do mais antigo para o mais novo
int aux = j;                    //variável auxiliar para percorrer o vetor da posição mais antiga para a mais nova

  for (int k = 0; k < num_elementos; k++){ //inicia o loop para imprimir cada caracter que consta no vetor

      if (k == (num_elementos-1)){        //Imprime o ultimo caractere (sem espaço), seguido de \n mais adiante
      printf("%c", buffer[aux]);
      } 
      if (k < (num_elementos-1))          //Imprime os demais caracteres(com espaço), menos o ultimo
      {
         printf("%c ", buffer[aux]);
      }
      aux = (aux+1)%5;                    //Incrementa a variavel auxiliar, que tem limites entre 0 e 4
      
    
    
  }
  printf("\n");                           //Segue para a próxima linha
}

int main() {


  char c;                   //Define uma variável que irá receber os caracteres do teclado
  
  do {
    scanf("%c", &c);        //recebe os caracteres do teclado
    if (c == '\n') break;   //Termina o programa ao pressionar "Enter"
    if (isalpha(c)==0) {    //Se a função retornar zero, é um numero, logo chamamos a função de Adicionar o valor ao buffer
			adicionar_buffer(c);
		}
		else {                  //Caso contrário chamamos a função de remover o valor do buffer
			remover_buffer();
		}		
		imprimir_buffer();      //em qualquer um dos casos, é necessário imprimir o buffer

  } while (1);              //loop infinito, que irá terminar com o "Enter" no teclado

  return 0;
}
