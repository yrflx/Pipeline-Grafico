# Pipeline Grafico 


Este repositório se destina a apresentar os resultados da implementação de um pipeline gráfico. Sendo motivado pela "Tarefa 2" da disciplina de Introdução à Computação Gráfica na UFPB.Para tal, foi disponibilizado pelo prof. Christian Azambuja Pagot, um "Object Loader" com o objetivo de importar arquivos ".obj" para a aplicação desenvolvida em C++.

Desenvolvido por Yuri Félix R, aluno de ciência da computação, UFPB. 

## Conceito breve


Um pipeline gráfico se resume a passos sequenciais para transformar coordenadas de um ambiente tridimencional em coordenadas de 2D para a tela do dispositivo usado. Na entrada foi carregado, através do Loader fornecido, o arquivo "monkey_head2.obj" que é uma exportação do macaco Suzanne, mascote do Blender.Durante o processo, utilizamos a biblioteca "glm" para realizar manipulações em matrizes. Para a saída, os vertices obtivos foram rasterizados na tela atráves do algoritmo de rasterização desenvolvido no trabalho I da mesma disciplina. Os passos para o desenvolvido do projeto, então, foram os seguintes:

* Obj Loader: Carremento de um objeto 3D;
* Transformação: Espaço do Objeto para Espaço do Universo;
* Transformação: Espaço do Universo para Espaço da Câmera;
* Transformação: Espaço da Câmera para Espaço Projetivo ou de Recorte;
* Transformação: Espaço da Câmera para Espaço “Canônico”;
* Transformação: Espaço “Canônico” para Espaço de Tela;
* Rasterização;


## Obj Loader: Carremento de um objeto 3D

Como dito, o script encarregado de fazer o carremento de objetos foi fornecido pelo professor, porém algumas coisas devem ser ditas:

- Estrutura do arquivo;
- Como usar;

### Estrutura do arquivo

O arquivo monkey_head2.obj contém uma sequência de vertices seguida de uma sequência de faces. A sequencia de vertices contém, em cada linha, as 3 coordenadas x, y e z do vertice e, a de faces, contém 3 referencias para os vertices, formando uma face triangular.

### Como usar;

Sendo J um indice de um vertice e I um indice de uma face, podemos receber as coordenadas de um vertice com:

```C#
obj_face *o = objData->faceList[i];
objData->vertexList[o->vertex_index[j]];
```

## Transformação: Espaço do Objeto para Espaço do Universo;

Inicialmente, estamos no espaço do objeto. Ou seja, um espaço onde temos apenas o objeto modelado. Nesse passo, geraremos uma matriz chamada matrizModel, contendo uma combinação de matrizes de rotação e escala. Ainda é possível gerar uma matriz de translação e shear, mas não utilizaremos. Nas matrizes desse processo será usada uma coordenada homôgenea W. Tendo assim, 4x4 elementos.

![escala](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/escala.png)

![rotacao](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/rotacao.png)

nossa matrizModel é então, a multiplicação entre a matriz de escala e a de rotação.
```C#
glm::mat4 matrizModel = matrizScala * matrizRotacao;
```

## Transformação: Espaço do Universo para Espaço da Câmera;

Até então apenas temos um objeto posicionado, mas precisamos definir a posição da camera e transformar as coordenadas do universo para as coordenadas da camera. Isso será feito definindo um vetor de posição da camera, vetor up da camera(normalmente para cima) e vetor para onde a camera ta olhando. Utilizaremos as seguintes transformações:

 ![vetores Camera](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/vetoresCamera.png)

![matrizes](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/matrizes.png)

Com isso, podemos obter a matrizView
```C#
 glm::mat4 matrizView = bt* T;
```

## Transformação: Espaço da Câmera para Espaço Projetivo ou de Recorte;

O processo aqui consiste em gerar o efeito de perspectiva nos objetos, fazendo com que os objetos mais próximos fiquem maiores e, obviamente, os mais ao fundo diminuam. Para isso, definimos um ponto onde as imagens serão formadas - um viewPlane - a uma distancia D da camera.

 ![perspectiva](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/projecaoPerspectiva.png)

Assim, basta juntar as três matrizes que temos.
```C#
 glm::mat4 matrizModelViewProject = matrizModel * matrizView * matrizProjection;
```

## Transformação: Espaço da Câmera para Espaço “Canônico” e 

Aqui, obtemos os vetores do objeto e os multiplicamos pela matrizModelViewProject para aplicar as transformações aos vertices do objeto. Após isso, dividimos as coordenadas desses vetores pela coordenada homogenea w para nos situarmos no espaço canônico. Estruturamos nossa matriz atual como uma matriz do tipo matriz[faces][vertices] contendo os vetores. 

```C#
    //pegando vertices
    glm::vec4 vClip[objData->faceCount][3];
    glm::vec4 vScreen[objData->faceCount][3];

    for(int i=0;i<objData->faceCount;i++){

        obj_face *o = objData->faceList[i];
        for(int j=0;j<3;j++){
            float x = objData->vertexList[o->vertex_index[j]]->e[0];
            float y = objData->vertexList[o->vertex_index[j]]->e[1];
            float z = objData->vertexList[o->vertex_index[j]]->e[2];

            glm::vec4 vetor(x,y,z,1);
            vClip[i][j] = vetor * matrizModelViewProject;

            vClip[i][j] =  vClip[i][j]/ vClip[i][j][3];
        }
     }
```

## Transformação: Espaço “Canônico” para Espaço de Tela;

Agora, devemos gerar uma matriz cuja finalidade consiste em realizar uma transição da nossa imagem para os parametros da tela. Ou seja, um viewPort. A matriz final contém transições e escalas. Por fim devemos aplicar tal matriz na matriz dos vetores que já temos. 

```C#
     glm::mat4 viewportS1(
         glm::vec4( 1,  0, 0, 0),
         glm::vec4( 0, -1, 0, 0),
         glm::vec4( 0,  0, 1, 0),
         glm::vec4( 0,  0, 0, 1));


    glm::mat4 viewportT(
         glm::vec4( 1,  0, 0, 1),
         glm::vec4( 0,  1, 0, 1),
         glm::vec4( 0,  0, 1, 0),
         glm::vec4( 0,  0, 0, 1));

    glm::mat4 viewportS2(
         glm::vec4( 255,   0, 0, 0),
         glm::vec4(   0, 255, 0, 0),
         glm::vec4(   0,   0, 1, 0),
         glm::vec4(   0,   0, 0, 1));


     glm::mat4 viewport = viewportS1 * viewportT * viewportS2;

    //clip to screen
    for(int i=0;i<objData->faceCount;i++){
        for(int j=0;j<3;j++){
            vScreen[i][j] =  glm::round(  vClip[i][j] * viewport);
        }
    }
```

## Ultimo passo: a rasterização

Aqui é onde a gente consegue ver todo o processo (rezando pra funcionar).Sendo a visualização feita através da utilização do código desenvolvido no trabalho anterior para rasterização de linhas pelo algoritmo de Bresenham. Para isso, basta percorrer o array final face a face e obter seus três vetores com suas coordenadas 2D(x e y).Finalizando, basta passar tais vetores como paramentros no método DrawTriangleSimples;

```C#
    for(int i=0;i<objData->faceCount;i++){

        double x0 = vScreen[i][0][0];
        double y0 = vScreen[i][0][1];

        double x1 = vScreen[i][1][0];
        double y1 = vScreen[i][1][1];

        double x2 = vScreen[i][2][0];
        double y2 = vScreen[i][2][1];

        drawTriangleSimple(x0,y0,x1,y1,x2,y2,white);

    }
```

# RESULTADO FINAL

![final](https://github.com/yrflx/Pipeline-Grafico/blob/master/pipelineGrafico/prints/final.png)

O resultado final, pode ser visto nesse vídeo.
[Video Youtube](http://www.youtube.com/watch?v=fIWSNRIqg-Q "Pipeline")


## OBSERVAÇÕES

Houve um bug informado no "trabalho I" que "soltava" pixels aleatórios pela tela, esse bug fica mais evidente neste projeto, em vista da quantidade maior de rasterizações.

## DIFICULDADES

Foi um tanto quanto simples usar a GLM, no entanto, ela implementa a multiplicação de matrizes (Cross) de uma maneira inversa da feita no Octave. Sabemos que a ordem dos fatores tem influência no resultado final. Dito isto, foi necessário um certo tempo analísando os resultados para achar tal diferença.Uma outra dificuldade foi entender o funcionamento do obj Loader e, por consequencia, do arquivo ".obj".

## REFERENCIAS

* As imagens dispostas aqui foram obtidas dos slides de aula do professor.
* [Biblioteca matrizes](https://glm.g-truc.net/0.9.8/index.html)
* Exemplo usando OpenGL e script do pipeline no octave.
