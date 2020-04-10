# Computação Gráfica - Renderização de uma imagem/cena utilizando NURBS

Trabalho apresentado ao Curso de Ciência da Computação da Universidade Federal
da Fronteira Sul - UFFS, Campus Chapecó, como requisito parcial para aprovação
no componente curricular Computação Gráfica - 2016.2, sob orientação do
professor Dr. José Carlos Bins Filho.

## Solicitação do trabalho

Implementar um programa (utilizando OpenGL e NURBS - _Non Uniform Rational Basis
Spline_) que realize a renderização de uma imagem/cena com:
- Pelo menos quatro objetos;
- Os objetos devem estar apoiados em alguma superfície (mesa, chão, etc.);
- Pelo menos um dos objetos deve ser transparente ou semi-transparente;
- Os objetos não podem ser simples (esferas, cones, etc.), ou seja, deve ser
realizada a utilização de NURBS;
- Deve haver iluminação.

Mais informações acerca da solicitação do trabalho podem ser encontradas no
arquivo "Description T2.pdf".

## Solução implementada

O código se encontra dividido em 3 arquivos: surface.cpp, surface.h e bmp.cpp
(arquivo do professor para textura). Detalhes acerca da cena:

- Foi realizada a renderização de uma cena com 04 objetos utilizando NURBS:
uma espátula, uma bandeja, uma forma de pudim e uma cuia;
- Apoio em um tampo (paralelepípedo) com textura de mármore;
- 02 dos objetos são transparentes/semi-transparentes;
- Existência de 02 fontes de luz;
- Visualização da cena em 16 pontos diferentes.

### Compilação e execução

Em ambiente Linux, estando na pasta onde os arquivos que compõem a resolução do
trabalho se encontram, executar o comando abaixo:

```
make run
```

### Utilização
Para a movimentação da câmera, pressione:
- As teclas de '0' a '7' para alternar entre diferentes posições de visualização;
- As teclas 'Y' e 'y' para alternar entre as alturas padrão e 0.0, respectivamente;
- 'Esc' para fechar a janela de visualização.

### Resultado final

![Renderização final](https://github.com/vladimirbelinski/T2-CG/blob/master/resultadoFinal.png)





