//--------------------------------------------------------------------------------------------
//
// File: 	scene.hpp
// Author:	P. Katarzynski (CCE)
//
// Description: Plik naglowkowy dla klasy OpenGL
//
//--------------------------------------------------------------------------------------------

#ifndef _scene_h

#define _scene_h

#include "common.h"
#include "globject.h"
#include "mesh.h"
#include "printer.h"
#include "texture.h"
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


//----------------------------------- DEFINES -------------------------------------------------
#define ESCAPE 27

//--------------------------------------------------------------------------------------------
// 								CLASS INTERFACE
//--------------------------------------------------------------------------------------------
class Scene
{

public:

Scene(int new_width,int new_height); //  domyslny konstruktor
~Scene(); // domyslny destruktor 

void Draw(int Selection = 0); // rysuje zawartosc sceny

void Resize(int new_width, int new_height); // zmienia rozmiar sceny 

void Init(); // procedura inicjujaca przetwarzanie

void Animate(); // przeprowadza animacje sceny 

void KeyPressed(unsigned char key, int x, int y); // obsluga klawiatury

void SaveAsBmp(char *filename);

void MouseDown(int x, int y);
void setLightUniformVec4(const char* property, int lightIndex, glm::vec4 value);
void setLightUniformVec3(const char* property, int lightIndex, glm::vec3 value);
void setLightUniformFloat(const char* property, int lightIndex,float value);
void createLights();

GLuint LoadShader(GLenum type,const char *file_name); // laduje program shadera z zewnetrznego pliku 


private:

float rot_x; //  obrot obiektu wzgledem X
float rot_y; //  obrot obiektu wzgledem Y

float Cam_angle; // kat obrotu kamery 
float Cam_r; // promien kamery 
float crankRot; // obrot walu
float crankRotDeg;
float yTop;
float yTop2;
float camPosY;
float camPosZ;


float LightAmbient;
float colorAlpha;

float crankRot2Deg;
float crankRot2;
float conRodRot;
float con2RodRot;
float conRotDeg;
float con2RotDeg;
float rodLength;
float crankRadius;
float valveMove;
float camshRotDeg;
float camshRot;
bool rotation;

struct Light {
	glm::vec4 position;
	glm::vec3 intensities; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
	float fConstant;
	float fLinear;
	float fExp;
};
std::vector<Light> gLights;
void boom(int index, boolean);
void dirLight(int index);
void animation();
Mesh *piston1;
Mesh *piston2;
Mesh *piston3;
Mesh *piston4;
Mesh *cylinderCase;
Mesh *engineCase;
Mesh *crankshaft;
Mesh *camshaft;
Mesh *belt;
Mesh *valve1;
Mesh *valve2;
Mesh *valve3;
Mesh *valve4;
Mesh *conRod1;
Mesh *conRod2;
Mesh *conRod3;
Mesh *conRod4;

glPrinter *Prn;
int ObjectSelected;
int numLights;
int turnOn;
bool anim;
void PreparePrograms(); // przygotowuje programy przetwarzania 
void PrepareObjects(); // przygotowuje obiekty do wyswietlenia 

// rozmiary sceny 
int width;
int height;

glm::mat4 mProjection;
//glm::mat4 mModelView;

// zasoby programu przetwarzania
GLuint program;
GLuint vertex_shader;
GLuint fragment_shader;

// komunikaty diagnostyczne 
char _msg[1024];
int err;
};

#endif
//------------------------------- KONIEC PLIKU -----------------------------------------------
