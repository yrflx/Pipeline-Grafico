Pipeline Grafico 
================  

Este repositório se destina a apresentar os resultados da implementação de um pipeline gráfico. Sendo motivado pela "Tarefa 2" da disciplina de Introdução à Computação Gráfica na UFPB.Para tal, foi disponibilizado pelo prof. Christian Azambuja Pagot, um "Object Loader" com o objetivo de importar arquivos ".obj" para a aplicação desenvolvida em C++.

Desenvolvido por Yuri Félix R, aluno de ciência da computação, UFPB. 

## Conceito breve


Um pipeline gráfico se resume a passos sequenciais para transformar coordenadas de um ambiente tridimencional em coordenadas de 2D para a tela do dispositivo usado. Na entrada foi carregado, através do Loader fornecido, o arquivo "monkey_head2.obj" que é uma exportação do macaco Suzanne, mascote do Blender.Durante o processo, utilizamos a biblioteca "glm" para realizar manipulações em matrizes. Para a saída, os vertices obtivos foram rasterizados na tela atráves do algoritmo de rasterização desenvolvido no trabalho I da mesma disciplina. Os passos para o desenvolvido do projeto, então, foram os seguintes:

* Obj Loader: Carremento de um objeto 3D;
* Transformação: Espaço do Objeto para Espaço do Universo;
* Transformação: Espaço do Universo para Espaço da Câmera;
* Transformação: Espaço da Câmera → Espaço Projetivo ou de Recorte (Implementar);
* Transformação: Espaço de Recorte → Espaço “Canônico” (Implementar);
* Transformação: Espaço “Canônico” → Espaço de Tela (Implementar);
* Rasterização;



## Obj Loader: Carremento de um objeto 3D


Como dito, o script encarregado de fazer o carremento de objetos foi fornecido pelo professor, porém algumas coisas devem ser ditas:

- Estrutura do arquivo;
- Como usar;


### Estrutura do arquivo

O arquivo monkey_head2.obj contém uma sequência de vertices seguida de uma sequência de faces. A sequencia de vertices contém, em cada linha, as 3 coordenadas x, y e z do vertice e, a de faces, contém 3 referencias para os vertices, formando uma face triangular.

## Como usar;

Sendo J um indice de um vertice e I um indice de uma face, podemos receber as coordenadas de um vertice com:

```C#
obj_face *o = objData->faceList[i];
objData->vertexList[o->vertex_index[j]];
```

## Transformação: Espaço do Objeto para Espaço do Universo;

Inicialmente, estamos no espaço do objeto. Ou seja, um espaço onde temos apenas o objeto modelado. Nesse passo, geraremos uma matriz chamada matrizModel, contendo uma combinação de matrizes de rotação e escala. Ainda é possível gerar uma matriz de translação e shear, mas não utilizaremos. Nas matrizes desse processo será usada uma coordenada homôgenea W. Tendo assim, 4x4 elementos.

![matriz Escala](//escala)

![matriz Rotacao](//rotacao)

nossa matrizModel é então, a multiplicação entre a matriz de escala e a de rotação.
```C#
glm::mat4 matrizModel = matrizScala * matrizRotacao;
```





