
///////////////////////////////////////

#include "main.h"
#include <math.h>
#include <glm/gtc/round.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp> // glm::pi
objLoader *objData;
float phi=0;
float ang = 1 * (PI/180.0f);


float i=1.0f;
int subindo = 1;
//-----------------------------------------------------------------------------
void MyGlDraw(void)
{
    tColor white;
    white.R = 255;
    white.G = 255;
    white.B = 255;


    ang+=0.008f;

    for (unsigned int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT ; i++)
	{
		FBptr[i*4]   = 0;
		FBptr[i*4+1] = 0;
		FBptr[i*4+2] = 0;
		FBptr[i*4+3] = 0;
	}




   //espaco objeto para espaco universo



    glm::mat4 matrizRotacao(
                glm::vec4 ( cos(ang) , 0 , sin(ang) , 0  ),
                glm::vec4 ( 0 , 1 , 0 , 0  ),
                glm::vec4 ( -sin(ang) , 0 , cos(ang) , 0  ),
                glm::vec4 ( 0 , 0 , 0 , 1  ));


    glm::mat4 matrizScala(
                glm::vec4 ( 3, 0 , 0 , 0  ),
                glm::vec4 ( 0 , 3 , 0 , 0  ),
                glm::vec4 ( 0 , 0 , 3, 0  ),
                glm::vec4 ( 0 , 0 , 0 , 1  ));


      glm::mat4 matrizModel = matrizScala * matrizRotacao;








    ////////////////////////////////////////////////////////////////////////////////
    // Calculo do sistema ortonormal gerado a partir dos parametros da camera
    ///////////////////////////////////////////////////////////////////////////////

    glm::vec3 camera_pos(0, 0, 20);
    glm::vec3 camera_lookat(0, 0, 0);
    glm::vec3 camera_up(0, 1, 0);



    glm::vec3 camera_Dir = camera_lookat - camera_pos;

    glm::vec3 z_camera = -glm::normalize(camera_Dir) ;
    glm::vec3 x_camera = glm::normalize(glm::cross(camera_up, z_camera));
    glm::vec3 y_camera =  glm::normalize(glm::cross(z_camera, x_camera));



   ////////////////////////////////////////////////////////////////////////////////
    // Calculo do sistema ortonormal gerado a partir dos parametros da camera
    ///////////////////////////////////////////////////////////////////////////////

    glm::mat4 bt(
		glm::vec4(x_camera[0], x_camera[1], x_camera[2], 0),
		glm::vec4(y_camera[0], y_camera[1], y_camera[2], 0),
        glm::vec4(z_camera[0], z_camera[1], z_camera[2], 0),
        glm::vec4(          0,           0,           0, 1));

    glm::mat4 T(
		glm::vec4(1,0,0,-(camera_pos[0])),
		glm::vec4(0,1,0,-(camera_pos[1])),
        glm::vec4(0,0,1,-(camera_pos[2])),
        glm::vec4(0,0,0,1));


    glm::mat4 matrizView = bt* T;


    /////////////////////// m projecao

    float d = 3.0f;


    glm::mat4 matrizProjection(
		glm::vec4(1,0,0,0),
		glm::vec4(0,1,0,0),
        glm::vec4(0,0,1,d),
        glm::vec4(0, 0, -(1/d), 0));

    //////////////////////Construcao da matriz Modelviewprojection

    glm::mat4 matrizModelViewProject = matrizModel * matrizView * matrizProjection;



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
     glm::mat4 matrizFim = matrizModelViewProject * viewport;

    for(int i=0;i<objData->faceCount;i++){
        obj_face *o = objData->faceList[i];

    }


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

    //clip to screen
    for(int i=0;i<objData->faceCount;i++){
        for(int j=0;j<3;j++){
            vScreen[i][j] =  glm::round(  vClip[i][j] * viewport);
        }
    }


     for(int i=0;i<matrizModelViewProject.length();i++){ //print
        for(int j=0;j<matrizModelViewProject.length();j++){
                for(int k=0;k<4;k++){
                    // printf("[%d][%d][%d]:%f\n", i,j,k, vScreen[i][j][k] );
                }


        }
        printf("\n");
    }  printf("\n\n\n\n");


    for(int i=0;i<objData->faceCount;i++){

        double x0 = vScreen[i][0][0];
        double y0 = vScreen[i][0][1];

        double x1 = vScreen[i][1][0];
        double y1 = vScreen[i][1][1];

        double x2 = vScreen[i][2][0];
        double y2 = vScreen[i][2][1];

        drawTriangleSimple(x0,y0,x1,y1,x2,y2,white);


    }





}

//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	// Inicializa��es.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a fun��o que chama as fun��es do mygl.h
	DrawFunc = MyGlDraw;
    objData = new objLoader();			// cria o objeto que carrega o modelo
	objData -> load("monkey_head2.obj");	// a carga do modelo � indicada atraves do nome do arquivo.


	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////
/*

///////////////////////////////////////////////

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
#include "objLoader.h"

// Ponteiro para o objeto que carregar� o modelo 3D (formato OBJ).
objLoader *objData;

unsigned int ViewPortWidth  = 512;
unsigned int ViewPortHeight = 512;

//-----------------------------------------------------------------------------
void display(void)
{
	glViewport(0, 0, ViewPortWidth, ViewPortHeight);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
	gluPerspective(	55.0f, 								// angulo de abertura da camera
					(float)ViewPortWidth/ViewPortWidth, // aspecto da imagem
					1.0f, 								// near plane
					50.0f);								// far plane

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(	0.0f, 0.0f, 4.0f,	// posi��o da c�mera
				0.0f, 0.0f, 0.0f,	// ponto para o qual a camera est� olhando
				0.0f, 1.0f, 0.0f);	// vetor "up"

	///////////////////////////////////////////////////////////////////////////
	// Desenha os eixos do sistema de coordenadas do universo
	// Vermelho: eixo X
	// Verde: eixo Y
	// Azul: eixo Z
	///////////////////////////////////////////////////////////////////////////

	glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);	// eixo X
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(2.0f, 0.0f, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);	// eixo Y
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 2.0f, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);	// eixo Z
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 2.0f);
	glEnd();

	///////////////////////////////////////////////////////////////////////////
	// Desenha o modelo carregado utilizando linhas brancas.
	// Cada volta do loop desenha um triangulo, formado por tres linhas.
	// Cada linha eh formada por 2 pontos (inicial e final).
	///////////////////////////////////////////////////////////////////////////

	glColor3f(1.0f, 1.0f, 1.0f);



	glBegin(GL_LINES);
		for(int i=0; i<objData->faceCount; i++)
		{
			obj_face *o = objData->faceList[i];

			glVertex3f(	objData->vertexList[o->vertex_index[0]]->e[0], // primeira linha
						objData->vertexList[o->vertex_index[0]]->e[1],
						objData->vertexList[o->vertex_index[0]]->e[2]);
			glVertex3f(	objData->vertexList[o->vertex_index[1]]->e[0],
						objData->vertexList[o->vertex_index[1]]->e[1],
						objData->vertexList[o->vertex_index[1]]->e[2]);

			glVertex3f(	objData ->vertexList[o->vertex_index[1]]->e[0],	// segunda linha
						objData->vertexList[o->vertex_index[1]]->e[1],
						objData->vertexList[o->vertex_index[1]]->e[2]);
			glVertex3f(	objData->vertexList[o->vertex_index[2]]->e[0],
						objData->vertexList[o->vertex_index[2]]->e[1],
						objData->vertexList[o->vertex_index[2]]->e[2]);

			glVertex3f(	objData->vertexList[o->vertex_index[2]]->e[0],	// terceira linha
						objData->vertexList[o->vertex_index[2]]->e[1],
						objData->vertexList[o->vertex_index[2]]->e[2]);
			glVertex3f(	objData->vertexList[o->vertex_index[0]]->e[0],
						objData->vertexList[o->vertex_index[0]]->e[1],
						objData->vertexList[o->vertex_index[0]]->e[2]);
            //////////////////////////






		}
	glEnd();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Funcao que imprime as coordenadas de um vertice.
// Pode ser utilizada para fazer debug de c�digo.
void printVector(obj_vector *v)
{
	printf("%.2f,", v->e[0] );
	printf("%.2f,", v->e[1] );
	printf("%.2f  ", v->e[2] );
}

//-----------------------------------------------------------------------------
// Esta funcao apenas imprime informacoes referentes ao modelo que foi carregado,
// tais como numero de vertices, normais, fontes de luz, etc.
void PrintModelInfo(objLoader* ptr)
{
	printf("Number of vertices: %i\n", ptr->vertexCount);
	printf("Number of vertex normals: %i\n", ptr->normalCount);
	printf("Number of texture coordinates: %i\n", ptr->textureCount);
	printf("\n");

	printf("Number of faces: %i\n", ptr->faceCount);
	for(int i=0; i<ptr->faceCount; i++)
	{
		obj_face *o = ptr->faceList[i];
		printf(" face ");
		for(int j=0; j<3; j++)
		{
			printVector(ptr->vertexList[ o->vertex_index[j] ]);
		}
		printf("\n");
	}

	printf("\n");

	printf("Number of spheres: %i\n", ptr->sphereCount);
	for(int i=0; i<ptr->sphereCount; i++)
	{
		obj_sphere *o = ptr->sphereList[i];
		printf(" sphere ");
		printVector(ptr->vertexList[ o->pos_index ]);
		printVector(ptr->normalList[ o->up_normal_index ]);
		printVector(ptr->normalList[ o->equator_normal_index ]);
		printf("\n");
	}

	printf("\n");

	printf("Number of planes: %i\n", ptr->planeCount);
	for(int i=0; i<ptr->planeCount; i++)
	{
		obj_plane *o = ptr->planeList[i];
		printf(" plane ");
		printVector(ptr->vertexList[ o->pos_index ]);
		printVector(ptr->normalList[ o->normal_index]);
		printVector(ptr->normalList[ o->rotation_normal_index]);
		printf("\n");
	}

	printf("\n");

	printf("Number of point lights: %i\n", ptr->lightPointCount);
	for(int i=0; i<ptr->lightPointCount; i++)
	{
		obj_light_point *o = ptr->lightPointList[i];
		printf(" plight ");
		printVector(ptr->vertexList[ o->pos_index ]);
		printf("\n");
	}

	printf("\n");

	printf("Number of disc lights: %i\n", ptr->lightDiscCount);
	for(int i=0; i<ptr->lightDiscCount; i++)
	{
		obj_light_disc *o = ptr->lightDiscList[i];
		printf(" dlight ");
		printVector(ptr->vertexList[ o->pos_index ]);
		printVector(ptr->normalList[ o->normal_index ]);
		printf("\n");
	}

	printf("\n");

	printf("Number of quad lights: %i\n", ptr->lightQuadCount);
	for(int i=0; i<ptr->lightQuadCount; i++)
	{
		obj_light_quad *o = ptr->lightQuadList[i];
		printf(" qlight ");
		printVector(ptr->vertexList[ o->vertex_index[0] ]);
		printVector(ptr->vertexList[ o->vertex_index[1] ]);
		printVector(ptr->vertexList[ o->vertex_index[2] ]);
		printVector(ptr->vertexList[ o->vertex_index[3] ]);
		printf("\n");
	}

	printf("\n");

	if(ptr->camera != NULL)
	{
		printf("Found a camera\n");
		printf(" position: ");
		printVector(ptr->vertexList[ ptr->camera->camera_pos_index ]);
		printf("\n looking at: ");
		printVector(ptr->vertexList[ ptr->camera->camera_look_point_index ]);
		printf("\n up normal: ");
		printVector(ptr->normalList[ ptr->camera->camera_up_norm_index ]);
		printf("\n");
	}

	printf("\n");

	printf("Number of materials: %i\n", ptr->materialCount);
	for(int i=0; i<ptr->materialCount; i++)
	{
		obj_material *mtl = ptr->materialList[i];
		printf(" name: %s", mtl->name);
		printf(" amb: %.2f ", mtl->amb[0]);
		printf("%.2f ", mtl->amb[1]);
		printf("%.2f\n", mtl->amb[2]);

		printf(" diff: %.2f ", mtl->diff[0]);
		printf("%.2f ", mtl->diff[1]);
		printf("%.2f\n", mtl->diff[2]);

		printf(" spec: %.2f ", mtl->spec[0]);
		printf("%.2f ", mtl->spec[1]);
		printf("%.2f\n", mtl->spec[2]);

		printf(" reflect: %.2f\n", mtl->reflect);
		printf(" trans: %.2f\n", mtl->trans);
		printf(" glossy: %i\n", mtl->glossy);
		printf(" shiny: %i\n", mtl->shiny);
		printf(" refact: %.2f\n", mtl->refract_index);

		printf(" texture: %s\n", mtl->texture_filename);
		printf("\n");
	}

	printf("\n");

	//vertex, normal, and texture test
	if(ptr->textureCount > 2 && ptr->normalCount > 2 && ptr->faceCount > 2)
	{
		printf("Detailed face data:\n");

		for(int i=0; i<3; i++)
		{
			obj_face *o = ptr->faceList[i];
			printf(" face ");
			for(int j=0; j<3; j++)
			{
				printf("%i/", o->vertex_index[j] );
				printf("%i/", o->texture_index[j] );
				printf("%i ", o->normal_index[j] );
			}
			printf("\n");
		}
	}
}

//-----------------------------------------------------------------------------
// Libera a memoria do objeto responsavel por guardar dados do modelo.
void FreeMemFunc(void)
{
	std::clog << "Exiting...\n";

	if (!objData)
		delete objData;
}

//-----------------------------------------------------------------------------
// Programa principal
int main(int argc, char **argv)
{
        objData = new objLoader();			// cria o objeto que carrega o modelo
        objData->load("monkey_head2");	// a carga do modelo � indicada atraves do nome do arquivo.
										// Neste caso, deve ser sempre do tipo OBJ.

	// Habilite esta fun��o se voc� deseja imprimir na tela dados do modelo
	// gerados durante a sua carga.
	PrintModelInfo(objData);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitWindowPosition(100,100);
	glutCreateWindow("OBJ Loader");

	glutDisplayFunc(display);

	///atexit(FreeMemFunc);

	glutMainLoop();

	return 0;
}
*/

