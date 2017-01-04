#ifndef globject_H
#define globject_H

#include "common.h"
//--------------------------------------------------------------------------------------------
// 								CLASS INTERFACE
//--------------------------------------------------------------------------------------------
class glObject
{

protected:

struct Material // material 
{
	char Name[1024];
	float Kd_R;
	float Kd_G;
	float Kd_B;		
};
//-------------------------------------------------------------
struct Vertex // wierzcholek
{
float x;
float y;
float z;
};
//-------------------------------------------------------------
struct Normal // normalna
{
float x;
float y;
float z;
};
//-------------------------------------------------------------
public:

glObject(); //  domyslny konstruktor
~glObject(); // domyslny destruktor 

// laduje geometrie z pliku obj
void LoadFromFile(char *OBJName, char *MTLName, char *ObjectName);
void LoadMaterial(char *MTLName, Material *M);

void CleanUp();
void SetColor(float r, float g, float b); // ustawia biezacy kolor dla grupy wierzcholkow

void Reload(); // laduje ponownie z pliku obj 

void SetNormal(Normal *N); // ustawia aktualna normalna 
void SetNormal(float _nx, float _ny, float _nz); // ustawia aktualna normalna 
void BeginObject(GLenum P,GLuint TextureId=0); // rozpoczyna generowanie nowego ksztaltu 

void EndObject(); // tworzy obiekt po wypelnieniu wierzcholkami 

void AddVertex(float x, float y, float z, float u=0.0, float v=0.0); // dodaje wierzcholek do listy ze wsp. tekstury 
void AddVertex(Vertex *v, float ut=0.0, float vt=0.0); // dodaje wierzcholek do listy ze wsp. tekstury 


void Draw(); // rysuje obiekt na scenie z u¿yciem zadanego prymitywu

void Draw(int cVAO); // rysuje obiekt na scenie z u¿yciem zadanej tablicy VAO

protected:

char OBJFileName[1024];
char MTLFileName[1024];
char ObjectName[1024];

//-------------------------------------------------------------

//-------------------------------------------------------------
struct Face // faseta 
{
int v1;
int v2;
int v3;
int n1;
int n2; 
int n3;
};


#define MAX_VAO 1000 // maksymalna liczba tablic VAO w obiekcie 
#define ATTRIB_PER_VERTEX 4 // liczba atrybutow skojarzonych z wierzcholkiem 

int lVAO; // liczba zdefiniowanych tablic VAO


GLuint VAO[MAX_VAO]; // id tablic wierzcholkow obiektow 
GLuint VBO[ATTRIB_PER_VERTEX*MAX_VAO]; // id buforow wierzcholkow

GLenum Primitives[MAX_VAO]; // tablica rodzajow prymitywow skojarzonych z VAO 
GLuint Textures[MAX_VAO]; // tablica identyfikatorow tekstur skojarzonych z VAO

int lCoords[MAX_VAO]; // liczba wspolrzednych skojarzona z kazda VAO

float *Coords; // tablica ze wspolrzednymi
float *Cols; // tablica z kolorami 
float *Normals; // tablica z normalnymi
float *TexCoords; // tablica ze wspolrzednymi tekstur

// ustawienia aktualnego koloru 
float col_r; 
float col_g;
float col_b;

// ustawienia aktualnej normalnej 
float nx; 
float ny;
float nz;


// komunikaty diagnostyczne 
char _msg[1024];

};

#endif

