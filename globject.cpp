#include "globject.h"

int CalcNormal(float A[], float B[], float C[], float *N)
{
const int x = 0;
const int y = 1;
const int z = 2;
float U[3];
float V[3];
// oblicz wspolrzedne wektorow U oraz V
U[x] = A[x] - B[x];
U[y] = A[y] - B[y];
U[z] = A[z] - B[z];
V[x] = B[x] - C[x];
V[y] = B[y] - C[y];
V[z] = B[z] - C[z];
// wyznacz wspolrzedne normalnej
N[x] = U[y]*V[z] - U[z]*V[y];
N[y] = U[z]*V[x] - U[x]*V[z];
N[z] = U[x]*V[y] - U[y]*V[x];
return 1;
}

int Normalize(float *N)
{
const int x = 0;
const int y = 1;
const int z = 2;
// oblicz dlugosc wektora
float L = (float)sqrt(N[x]*N[x] + N[y]*N[y] + N[z]*N[z]);
if (L<0.01) L=0.01f;
// wyznacz wspolrzedne normalnej
N[x] /= L;
N[y] /= L;
N[z] /= L;
return 1;
}
//--------------------------------------------------------------------------------------------
// domyslny konstruktor 
glObject::glObject()
{			
	Coords = (float *) malloc(sizeof(float));	
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));
	TexCoords = (float *) malloc(sizeof(float));
	nx = 1.0;
	ny = 0.0;
	nz = 0.0;
	col_r = 0.0;
	col_g = 0.0;
	col_b = 0.0;
	lVAO = 0;
	OBJFileName[0] = 0x00;
	ObjectName[0] = 0x00;
	MTLFileName[0] = 0x00;
}
//--------------------------------------------------------------------------------------------
// domyslny destruktor 
glObject::~glObject()
{	
	if (Coords) free(Coords);
	if (Cols) free(Cols);		
	if (Normals) free(Normals);
	if (TexCoords) free(TexCoords);
}
//--------------------------------------------------------------------------------------------
// ustawia aktualny kolor rysowania prymitywu 
void glObject::SetColor(float r, float g, float b)
{
	col_r = r;
	col_g = g; 
	col_b = b;
}
//--------------------------------------------------------------------------------------------
 // ustawia aktualna normalna 
void glObject::SetNormal(Normal *N)
{
	SetNormal(N->x,N->y,N->z);
}
//--------------------------------------------------------------------------------------------
// ustawia aktualna normalna 
void glObject::SetNormal(float _nx, float _ny, float _nz)
{
	nx = _nx;
	ny = _ny; 
	nz = _nz;
}
//--------------------------------------------------------------------------------------------
// usuwa alokowane atrybuty
void glObject::CleanUp()
{
	lVAO = 0; 
}
//--------------------------------------------------------------------------------------------
// laduje ponownie obiekt z pliku obj 
void glObject::Reload()
{
	CleanUp();
	LoadFromFile(OBJFileName,MTLFileName,ObjectName);
}
//--------------------------------------------------------------------------------------------
// laduje definicje materialu z pliku 
void glObject::LoadMaterial(char *MTLName, Material *M)
{
FILE *fil = NULL;
fil = fopen(MTLName,"r");
char line[1024];

char dummy1[200];
char dummy2[200];

int found = 0;

	while (!feof(fil))
	{
		fgets(line,1024,fil);
		sscanf(line,"%s",dummy1);
		
		if (strstr(dummy1,"newmtl"))
		{
			sscanf(line,"%s %s",dummy1,dummy2);
			if (strstr(dummy2,M->Name))
				found = 1;
			else
				found = 0;
		} // condition for newmtl found 
				
		if (found == 0) continue;
		
		if ((line[0] == 'K')&&(line[1] == 'd')) sscanf(line,"%s %f %f %f",dummy1,&M->Kd_R,&M->Kd_G,&M->Kd_B);

	}
	PrintLog("material found");
if (fil) fclose(fil);
}
//--------------------------------------------------------------------------------------------
// laduje geometrie obiektu z pliku obj
void glObject::LoadFromFile(char *OBJName, char *MTLName, char *ObjectName)
{
	
// skopiuj informacje o polozeniu plikow 
if (strlen(OBJFileName) == 0) strncpy(OBJFileName,OBJName,strlen(OBJName)+1);
if (strlen(MTLFileName) == 0) strncpy(MTLFileName,MTLName,strlen(MTLName)+1);
if (strlen(ObjectName) == 0) strncpy(ObjectName,ObjectName,strlen(ObjectName)+1);


FILE *fil = NULL;
fil = fopen(OBJName,"r");
if (fil== NULL)
{
 sprintf(_msg,"Nie udalo siê otworzyc pliku %s\n",OBJName);
 ThrowException(_msg);
}
	
int lV = 0; // liczba wierzcholkow
int lF = 0; // liczba faset
int lN = 0; // liczba normalnych 
char line[1024]; // pojedyncza linia z pliku 
int found = 0;  // flaga odnalezienia (lokalna)
int was_found = 0;  // flaga odnalezienia (globalna)
char dummy[20]; // pomocniczy bufor
char name[200]; // nazwa obiektu 
	
// policz wystapienia 
while (!feof(fil))
{
	line[0] = 0x00;
	fgets(line,1024,fil);
	sprintf(_msg, "line is: %s", line);
	PrintLog(_msg);
		
	if ((line[0] == 'o')&&(line[1]==' ')) // odnaleziono obiekt 
	{
		sprintf(_msg, "znaleziono obiekt");
		PrintLog(_msg);
		sscanf(line,"%s %s",dummy,name); // pobierz nazwe obiektu 
		if (strstr(name,ObjectName))  // porownaj z poszukiwana
			found = 1;
		else
			found = 0;
	}
		
	sprintf(_msg, "Zaladowano %s->%s : %d wierzcholkow %d trojkatow", OBJName, ObjectName, lV, lF);
	PrintLog(_msg);

	if (found == 0) continue;
	if ((line[0] == 'v')&&(line[1]==' ')) lV++;
	if ((line[0] == 'f')&&(line[1]==' ')) lF++;
	if ((line[0] == 'v')&&(line[1]=='n')&&(line[2]==' ')) lN++;		
}
//sprintf(_msg, "hello.\n");

//ThrowException(_msg);
// tablica z wierzcholkami 
Vertex *V = new Vertex[lV];
// tablica z normalnymi
Normal *N = new Normal[lN];
// tablica z fasetami 
Face *F = new Face[lF];

// definicja materialu dla obiektu 
Material MAT;

// przewin plik do poczatku 
rewind(fil);
	
lV = 0;
lF = 0;
lN = 0;
	
found = 0;
while (!feof(fil))
	{
	line[0] = 0x00;
	fgets(line,1024,fil);
		

	// new object
	if ((line[0] == 'o')&&(line[1]==' '))
	{
		sscanf(line,"%s %s",dummy,name);
		// compare object's name with given pattern
		if (strstr(name,ObjectName)) 
			found = 1;
		else
			found = 0;
	}
		
	// the object is not found 
	if (found == 0) continue;
		
	// new material
		
	sscanf(line,"%s",dummy);
	if (strstr(dummy,"usemtl"))
	{
		sscanf(line,"%s %s",dummy,MAT.Name);
		LoadMaterial(MTLName,&MAT);
	}
		
	// new vertex
	if ((line[0] == 'v')&&(line[1]==' '))
	{	
		sscanf(line,"%s %f %f %f",dummy,&V[lV].x,&V[lV].y,&V[lV].z);
		lV++;
	}
		
	// new normal
	if ((line[0] == 'v')&&(line[1]=='n')&&(line[2]==' '))
	{				
		sscanf(line,"%s %f %f %f",dummy,&N[lN].x,&N[lN].y,&N[lN].z);
		lN++;			
	}		
	
	// new face 
	if ((line[0] == 'f')&&(line[1]==' '))
	{	
		sscanf(line,"%s %d//%d %d//%d %d//%d",dummy,&F[lF].v1,&F[lF].n1,&F[lF].v2,&F[lF].n2,&F[lF].v3,&F[lF].n3);			
		lF++;			
	}		
		
} // while feof 2-nd pass
	
	if (fil) fclose(fil);	
	
	// zbuduj geometrie obiektu 
	BeginObject(GL_TRIANGLES);

	SetColor(MAT.Kd_R,MAT.Kd_G,MAT.Kd_B);	
	
	for (int i = 0; i < lF; i++)
	{	
		SetNormal(&N[F[i].n1-1]);
		AddVertex(&V[F[i].v1-1]);
		SetNormal(&N[F[i].n2-1]);
		AddVertex(&V[F[i].v2-1]);
		SetNormal(&N[F[i].n3-1]);
		AddVertex(&V[F[i].v3-1]);		
	}
	
	EndObject();

	sprintf(_msg,"Zaladowano %s->%s : %d wierzcholkow %d trojkatow",OBJName,ObjectName,lV,lF);
	PrintLog(_msg);

	if (V) delete []V;
	if (F) delete []F;
	if (N) delete []N;


}

//--------------------------------------------------------------------------------------------
// rozpoczyna tworzenie tablicy VAO dla danego prymitywu
void glObject::BeginObject(GLenum P,GLuint TextureId)
{	
	
	lVAO++;
	// przypisz rodzaj prymitywu do narysowania VAO
	Primitives[lVAO-1] = P;

	// przypisz Id tekstury do narysowania VAO
	Textures[lVAO-1] = TextureId;

	// wyzeruj licznik wspolrzednych 
	lCoords[lVAO-1] = 0;
	Coords = (float *) malloc(sizeof(float));
	Cols = (float *) malloc(sizeof(float));
	Normals = (float *) malloc(sizeof(float));

	if (lVAO > MAX_VAO) ThrowException("Przekroczono maksymalna liczbe VAO w glObject");
	
	GLuint VAO_id[1];	
	// przygotuj tablice VAO
	glGenVertexArrays(1, VAO_id); 
	VAO[lVAO-1] = VAO_id[0];

	glBindVertexArray(VAO[lVAO-1]);

	GLuint VBO_id[4];	
	// przygotuj bufory VBO
	glGenBuffers(4, VBO_id);
	
	VBO[4*lVAO-4] = VBO_id[0];
	VBO[4*lVAO-3] = VBO_id[1];
	VBO[4*lVAO-2] = VBO_id[2];
	VBO[4*lVAO-1] = VBO_id[3];
	
}
//--------------------------------------------------------------------------------------------
// dodaje wierzcholek do listy ze wsp. tekstury 
// wrapper dla argumentu
void glObject::AddVertex(Vertex *v, float ut, float vt)
{
	AddVertex(v->x,v->y,v->z,ut,vt);
}
//--------------------------------------------------------------------------------------------
// dodaje wierzcholek do listy ze wsp. tekstury 
void glObject::AddVertex(float x, float y, float z, float u, float v)
{
	lCoords[lVAO-1] +=3;
	Coords = (float *) realloc(Coords, lCoords[lVAO-1]*sizeof(float));
	if (Coords == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Coords[lCoords[lVAO-1]-3] = x;
	Coords[lCoords[lVAO-1]-2] = y;
	Coords[lCoords[lVAO-1]-1] = z;

	Cols = (float *) realloc(Cols, lCoords[lVAO-1]*sizeof(float));
	if (Cols == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Cols[lCoords[lVAO-1]-3] = col_r;
	Cols[lCoords[lVAO-1]-2] = col_g;
	Cols[lCoords[lVAO-1]-1] = col_b;	

	Normals = (float *) realloc(Normals, lCoords[lVAO-1]*sizeof(float));
	if (Normals == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	Normals[lCoords[lVAO-1]-3] = nx;
	Normals[lCoords[lVAO-1]-2] = ny;
	Normals[lCoords[lVAO-1]-1] = nz;	

	TexCoords = (float *) realloc(TexCoords, lCoords[lVAO-1]*sizeof(float));
	if (TexCoords == NULL) ThrowException("glObject:: Blad realokacji pamieci");
	TexCoords[lCoords[lVAO-1]-3] = u;
	TexCoords[lCoords[lVAO-1]-2] = v;	
	TexCoords[lCoords[lVAO-1]-1] = 0.0;	
}
//--------------------------------------------------------------------------------------------
void glObject::EndObject()
{
	// podlacz pierwszy obiekt z VAOs
	glBindVertexArray(VAO[lVAO-1]);
	// podlacz pierwszy bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-4]);	
	// wypelnij bufor wspolrzednymi wierzcholka 

	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Coords, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 0 (wskazany w shaderze)
	glEnableVertexAttribArray(0); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz drugi bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-3]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Cols, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 1 (wskazany w shaderze)
	glEnableVertexAttribArray(1); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz trzeci bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-2]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), Normals, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 2 (wskazany w shaderze)
	glEnableVertexAttribArray(2); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	// podlacz czwarty bufor VBOs
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4*lVAO-1]);	
	// wypelnij bufor kolorami wierzcholka
	glBufferData(GL_ARRAY_BUFFER, lCoords[lVAO-1]*sizeof(float), TexCoords, GL_STATIC_DRAW);
	// wybierz atrybut indeksie 3 (wskazany w shaderze)
	glEnableVertexAttribArray(3); 
	// powiaz dane z bufora ze wskazanym atrybutem
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);	

	glBindVertexArray(0);
}
//--------------------------------------------------------------------------------------------
void glObject::Draw()
{
	for (int i = 0; i < lVAO; i++)
	{
		if (Textures[i] == 0)
		{
			glDisable(GL_TEXTURE_2D);
		}
			else
		{
				glBindTexture(GL_TEXTURE_2D, Textures[i]);
				glEnable(GL_TEXTURE_2D);
		}		
		glBindVertexArray(VAO[i]);
		glDrawArrays(Primitives[i],0,lCoords[i]/3);		
		glBindVertexArray(0);
	}	
}
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
// the end