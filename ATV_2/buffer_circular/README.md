# Buffer Circular

Esta tarefa consiste em implementar as funções relacionadas a um buffer
circular.

Se você precisa entender o que é um buffer circular, por favor, veja o artigo
sobre
[buffer circular na Wikipedia](https://en.wikipedia.org/wiki/Circular_buffer),
que tem uma explicação bastante didática sobre o assunto.

## Outras anotações
O buffer circular que implementaremos tem obrigatoriamente tamanho 5, isto é,
cabem 5 elementos nele.

O programa receberá uma cadeia de caracteres, continuamente. Se o caractere
recebido for um dígito (0, 1, 2, 3, 4, 5, 6, 7, 8, ou 9), ele deverá ser
adicionado ao buffer. Se o caractere for uma letra (a-z ou A-Z), o elemento mais
antigo do buffer deverá ser removido. Se for um fim de linha (`\n`), o programa
deve encerrar.

Depois de qualquer adição ou remoção de elemento do buffer, seu conteúdo deverá
ser impresso inteiramente, do elemento mais antigo para o mais recente, com
espaço entre os elementos e um fim de linha (`\n`) ao final.

Tentativas de inserção em buffer cheio ou remoções em buffer vazio devem ser
ignoradas.

## Exemplo
Entrada:

`a123456bZf789\n`


Saída:

`\n`

`1\n`

`1 2\n`

`1 2 3\n`

`1 2 3 4\n`

`1 2 3 4 5\n`

`1 2 3 4 5\n`

`2 3 4 5\n`

`3 4 5\n`

`4 5\n`

`4 5 7\n`

`4 5 7 8\n`

`4 5 7 8 9\n`




## Instruções adicionais

1. Vá diretamente ao conjunto de testes para verificar como tratar casos
   limítrofes ou mal-definidos.
  1. Se desejar, proponha novos casos de teste.
