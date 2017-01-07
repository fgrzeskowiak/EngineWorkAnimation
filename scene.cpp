//--------------------------------------------------------------------------------------------
//
// File: 	scene.cpp
// Author:	P. Katarzynski (CCE)
//
// Description: Implementacja klasy sceny OpenGL
//
//--------------------------------------------------------------------------------------------
//									ZALEZNOSI 
//--------------------------------------------------------------------------------------------

#include "scene.h"
#include <sstream>
//--------------------------------------------------------------------------------------------
// zglasza wyjatek z komunikatem do debuggowania 

//--------------------------------------------------------------------------------------------
Scene::Scene(int new_width,int new_height)
{	
	width = new_width;
	height = new_height;	
	rot_x = 0.0;
	rot_y = 0.0;
	yTop = 0;
	camPosY = 10;
	camPosZ = 4.0;
	//Obj1 = NULL;
	crankRot = 1.57;
	camshRot = 1.57;
	crankRot2Deg = 0;
	crankRot2 = 0;
	conRodRot = 0;
	rotation = true;
	conRotDeg = 0;
	rodLength = 4.1371;
	crankRadius = 1.71337;
	valveMove = 0;
	colorAlpha = 1.0f;
	//LightAmbient = 3.0f;
	turnOn = 0;

	Cam_angle = 55.0f;
	Cam_r = 39.5f;
	err = 1;	
}
//--------------------------------------------------------------------------------------------
// Domyslny destruktor 
Scene::~Scene()
{
	// usun program przetwarzania 
	if (glIsProgram(program)) glDeleteProgram(program);
	//if (Obj1) delete Obj1;
}
void Scene::createLights() {
	Light spotLight1;
	spotLight1.position = glm::vec4(0.00213f, 7.80893f, 14.4082f, 1.0f);
	spotLight1.intensities = glm::vec3(0.887f, 0.345f, 0.133f);
	spotLight1.ambientCoefficient = 7.0f;
	spotLight1.fConstant = 0.9f;
	spotLight1.fLinear = 2.0f;
	spotLight1.fExp = 1.0f;
	gLights.push_back(spotLight1);

	Light spotLight2;
	spotLight2.position = glm::vec4(0.00213, 7.80893, 9.52295, 1.0);
	spotLight2.intensities = glm::vec3(0.887, 0.345, 0.133);
	spotLight2.ambientCoefficient = 7.0;
	spotLight2.fConstant = 0.9;
	spotLight2.fLinear = 1.0;
	spotLight2.fExp = 1.0;
	gLights.push_back(spotLight2);


	Light spotLight3;
	spotLight3.position = glm::vec4(0.00213, 7.80893, 4.73236, 1.0);
	spotLight3.intensities = glm::vec3(0.887, 0.345, 0.133);
	spotLight3.ambientCoefficient = 3.0;
	spotLight3.fConstant = 0.9;
	spotLight3.fLinear = 1.0;
	spotLight3.fExp = 1.0;
	gLights.push_back(spotLight3);

	Light spotLight4;
	spotLight4.position = glm::vec4(0.00213, 7.80893, 0, 1.0);
	spotLight4.intensities = glm::vec3(0.887, 0.345, 0.133);
	spotLight4.ambientCoefficient = 3.0;
	spotLight4.fConstant = 0.9;
	spotLight4.fLinear = 1.0;
	spotLight4.fExp = 1.0;
	gLights.push_back(spotLight4);

	Light dirLight;
	dirLight.position = glm::vec4(0.00213, 7.80893, 4.73236, 0.0);
	dirLight.intensities = glm::vec3(1, 1, 1);
	dirLight.ambientCoefficient = 0.8f;
	dirLight.fConstant = 0.9;
	dirLight.fLinear = 1.0;
	dirLight.fExp = 1.0;
	gLights.push_back(dirLight);

	numLights = gLights.size();
}
//--------------------------------------------------------------------------------------------
// przygotowuje programy cienionwania 
void Scene::PreparePrograms()
{
	err = 0; // ustaw flage bledu
	program  = glCreateProgram();
	if (!glIsProgram(program)) {err = 1; ThrowException("Nie udalo sie utworzyc programu");}
	
	vertex_shader = LoadShader(GL_VERTEX_SHADER,"vs.glsl");
	glAttachShader(program,vertex_shader);

	fragment_shader = LoadShader(GL_FRAGMENT_SHADER,"fs.glsl");
	glAttachShader(program,fragment_shader);
	
	// linkowanie programu 
	glLinkProgram(program);

	GLint link_status; 
    glGetProgramiv( program, GL_LINK_STATUS, &link_status); 
    if( link_status == GL_FALSE ) 
    { 
        // pobranie i wyœwietlenie komunikatu b³êdu 
        GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
		err = 1;
		ThrowException("Blad linkowania programu");
    }
	else
		PrintLog("Program zlinkowany");

	// walidowanie programu 
	glValidateProgram(program); 
	GLint validate_status;
    // sprawdzenie poprawnoœci walidacji obiektu programu 
    glGetProgramiv( program, GL_VALIDATE_STATUS, &validate_status ); 
    if( validate_status == GL_FALSE ) 
    { 
        // pobranie i wyœwietlenie komunikatu b³êdu 
        GLint logLength; 
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetProgramInfoLog( program, logLength, NULL, log ); 
        PrintLog(log);
        delete[] log; 
		err = 1;
        ThrowException("Blad walidacji programu");
    } 
	else
		PrintLog("Program prawidlowy");
	
	glUseProgram(program);
}
//--------------------------------------------------------------------------------------------
void Scene::PrepareObjects()
{  
	

	createLights();

	piston1 = new Mesh();
	piston1->LoadMesh("piston1.obj");

	piston2 = new Mesh();
	piston2->LoadMesh("piston2.obj");

	piston3 = new Mesh();
	piston3->LoadMesh("piston3.obj");

	piston4 = new Mesh();
	piston4->LoadMesh("piston4.obj");

	crankshaft = new Mesh();
	crankshaft->LoadMesh("crankshaft.obj");

	camshaft = new Mesh();
	camshaft->LoadMesh("camshaft.obj");
	
	belt = new Mesh();
	belt->LoadMesh("belt2.obj");

	valve1 = new Mesh();
	valve1->LoadMesh("valve1.obj");

	valve2 = new Mesh();
	valve2->LoadMesh("valve2.obj");

	valve3 = new Mesh();
	valve3->LoadMesh("valve3.obj");

	valve4 = new Mesh();
	valve4->LoadMesh("valve4.obj");

	conRod1 = new Mesh();
	conRod1 -> LoadMesh("conRod1.obj");

	conRod2 = new Mesh();
	conRod2->LoadMesh("conRod2.obj");

	conRod3 = new Mesh();
	conRod3->LoadMesh("conRod3.obj");

	conRod4 = new Mesh();
	conRod4->LoadMesh("conRod4.obj");
	
	engineCase = new Mesh();
	engineCase->LoadMesh("engineCase.obj");

	cylinderCase = new Mesh();
	cylinderCase->LoadMesh("cylinderCase.obj");

	Prn = new glPrinter("tahoma.ttf", 24);
}
//--------------------------------------------------------------------------------------------
// Odpowiada za skalowanie sceny przy zmianach rozmiaru okna
void Scene::Resize(int new_width, int new_height)
{
	// przypisz nowe gabaryty do pol klasy 
	width = new_width;
	// uwzgledniaj obecnosc kontrolki wizualnej 
	height = new_height-100; 	
  	// rozszerz obszar renderowania do obszaru o wymiarach 'width' x 'height'
	glViewport(0, 100, width, height);	
	
	mProjection = glm::perspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
}
//--------------------------------------------------------------------------------------------
// laduje program shadera z zewnetrznego pliku 
GLuint Scene::LoadShader(GLenum type,const char *file_name)
{  
  // zmienna plikowa 
  FILE *fil = NULL; 
  // sproboj otworzyc plik 
  fil = fopen(file_name,"rb");
  // sprawdz, czy plik sie otworzyl  
  sprintf(_msg,"Nie mozna otworzyc %s",file_name);
  if (fil == NULL)  ThrowException(_msg);

  // okresl rozmiar pliku
  fseek( fil, 0, SEEK_END );
  long int file_size = ftell(fil); 
  // przewin na poczatek 
  rewind(fil); 
  // utworzenie bufora na kod zrodlowy programu shadera
  GLchar *srcBuf = new GLchar[(file_size+1)*sizeof(GLchar)];

  // przeczytanie kodu shadera z pliku 
  fread(srcBuf,1,file_size,fil);

  // zamknij plik 
  fclose(fil);

  // tekst programu MUSI miec NULL na koncu
  srcBuf[file_size] = 0x00;

  // utworzenie obiektu shadera
  GLuint shader = glCreateShader(type);

  // przypisanie zrodla do shadera 
  glShaderSource(shader,1,const_cast<const GLchar**>(&srcBuf),NULL);

  // sprzatanie 
  delete[] srcBuf;

  // proba skompilowania programu shadera 
  glCompileShader(shader);

  // sprawdzenie czy sie udalo 
  GLint compile_status;
  glGetShaderiv(shader,GL_COMPILE_STATUS,&compile_status);

  if (compile_status != GL_TRUE) // nie udalo sie 
  {
	    GLint logLength; 
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength ); 
        char *log = new char[logLength]; 
        glGetShaderInfoLog( shader, logLength, NULL, log ); 
		sprintf(_msg,"Blad kompilacji pliku shadera %s",file_name);
		PrintLog(_msg);
        PrintLog(log);
		ThrowException("Blad kompilacji shadera");
	    delete []log;
  }
  else
  {
	  sprintf(_msg,"Plik shadera %s skompilowany",file_name);
	  PrintLog(_msg);
  }

  return shader; // zwroc id shadera 
}
//--------------------------------------------------------------------------------------------
// inicjuje proces renderowania OpenGL
void Scene::Init()
{
	// inicjalizacja modu³u glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		sprintf(_msg, "GLew error: %s\n", glewGetErrorString(err));	
		err = 1;
		ThrowException(_msg);
	}
	
	// pobierz informacje o wersji openGL 
	sprintf(_msg,"OpenGL vendor: ");
	strcat(_msg,(const char*)glGetString( GL_VENDOR ));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL renderer: ");
	strcat(_msg,(const char*)glGetString( GL_RENDERER));
	PrintLog(_msg);

	sprintf(_msg,"OpenGL version: ");
	strcat(_msg,(const char*)glGetString( GL_VERSION));
	PrintLog(_msg);

	//  ustaw kolor tla sceny (RGB Z=1.0)
	glClearColor(0.41f, 0.41f, 0.41f, 0.0f);
	
	// przygotuj programy shaderow
	PreparePrograms();
	
	if (err) return; 

	// przygotuj obiekty do wyswietlenia 
	PrepareObjects();
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glEnable(GL_TEXTURE_2D);
}

void Scene::animation() {
	turnOn++;
	if (turnOn % 2 == 0)
		anim = true;
	else
		anim = false;
}
//--------------------------------------------------------------------------------------------
// przeprowadza animacje sceny 
void Scene::Animate()
{
	if (!anim) {
		crankRot += 0.07;
		camshRot += 0.035;
	}
	
}
//--------------------------------------------------------------------------------------------
// obsluga klikania mysza
void Scene::MouseDown(int x, int y)
{		
	ObjectSelected = 0;
	Draw(1);
	BYTE bArray[4]; 
	int _x = x;
	int _y = height - y +100;
	glReadPixels(_x, _y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
	
	ObjectSelected = bArray[0];

	sprintf(_msg,"Selected: %d",ObjectSelected);
	PrintLog(_msg);
}
//--------------------------------------------------------------------------------------------
// kontrola naciskania klawiszy klawiatury
void Scene::KeyPressed(unsigned char key, int x, int y) 
{            
	if (key == ESCAPE) ThrowException("Zatrzymaj program");
	sprintf(_msg, "%d", key);
	switch(key)
	{
		case 49: {Cam_r = 23.0f; Cam_angle = 90.0f; camPosY = 8.5f; camPosZ = 0; break; }
		case 50: {Cam_r = 11.5f; Cam_angle = 825.0f; camPosY = 13.5f; camPosZ = 15.0f; break; }
		case 51: {Cam_r = 39.5f; Cam_angle = 55.0f; camPosY = 10.0f; camPosZ = 0.0f; break; }
		//case 37: {rot_y -= 5.0f; break;}
		//case 38: {rot_x -= 5.0f; break;}
		//case 39: {rot_y += 5.0f; break;}
		//case 40: {rot_x += 5.0f; break;}		
		case 112: {LightAmbient += 0.1f; break;} // F1		
		case 113: {LightAmbient -= 0.1f; break;} //F2		
		
	

		//case 116: { Obj->Reload(); break;} //F5		
		case 117: {
					  crankRot += 0.1;
					  camshRot += 0.05;
					  
					  break; } //F6		
		case 32: {animation(); break; }
		case 38: {camPosY += 0.5f; break; } //F7
		case 40: {camPosY -= 0.5f; break; } //F7
		case 114: {camPosZ += 0.5f; break; }
		case 115: {camPosZ -= 0.5f; break; }
		case 52: {colorAlpha -= 0.1f; break; }
		case 53: {colorAlpha += 0.1f; break; }
		case 87: {Cam_r -= 0.5f; break;} //W
		case 83: {Cam_r += 0.5f; break;} //S		
		case 65: {Cam_angle -= 5.0f; break;} //A
		case 68: {Cam_angle += 5.0f; break;} //D
	}
	
}
void Scene::setLightUniformVec4(const char* property, int lightIndex, glm::vec4 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << property;
	std::string uniformName = ss.str();

	glUniform4fv(glGetUniformLocation(program, uniformName.c_str()), 1, glm::value_ptr(value));
}
void Scene::setLightUniformVec3(const char* property, int lightIndex, glm::vec3 value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << property;
	std::string uniformName = ss.str();

	glUniform3fv(glGetUniformLocation(program, uniformName.c_str()), 1, glm::value_ptr(value));
}
void Scene::setLightUniformFloat(const char* property, int lightIndex, float value) {
	std::ostringstream ss;
	ss << "allLights[" << lightIndex << "]." << property;
	std::string uniformName = ss.str();

	glUniform1f(glGetUniformLocation(program, uniformName.c_str()), value);
}
void Scene::boom(int index, boolean ok) {
	if (ok) {
		gLights[index].ambientCoefficient = 2000.0f;
		gLights[index].fConstant = 0.9f;
		gLights[index].fLinear = 200.0f;
		gLights[index].fExp = 200.0f;
		setLightUniformVec4("position", index, gLights[index].position);
		setLightUniformVec3("intensities", index, gLights[index].intensities);
		//setLightUniformFloat("attenuation", i, gLights[i].attenuation);
		setLightUniformFloat("ambientCoefficient", index, gLights[index].ambientCoefficient);
		setLightUniformFloat("fConstant", index, gLights[index].fConstant);
		setLightUniformFloat("fLinear", index, gLights[index].fLinear);
		setLightUniformFloat("fExp", index, gLights[index].fExp);
	}
	else {
		gLights[index].ambientCoefficient = 1.0f;
		gLights[index].fConstant = 0.9f;
		gLights[index].fLinear = 10.0f;
		gLights[index].fExp = 10.0f;
		setLightUniformVec4("position", index, gLights[index].position);
		setLightUniformVec3("intensities", index, gLights[index].intensities);
		//setLightUniformFloat("attenuation", i, gLights[i].attenuation);
		setLightUniformFloat("ambientCoefficient", index, gLights[index].ambientCoefficient);
		setLightUniformFloat("fConstant", index, gLights[index].fConstant);
		setLightUniformFloat("fLinear", index, gLights[index].fLinear);
		setLightUniformFloat("fExp", index, gLights[index].fExp);
	}
}

void Scene::dirLight(int index) {
	gLights[index].ambientCoefficient = 1.2f;
	gLights[index].fConstant = 0.9f;
	gLights[index].fLinear = 1.0f;
	gLights[index].fExp = 1.0f;
	setLightUniformVec4("position", index, gLights[index].position);
	setLightUniformVec3("intensities", index, gLights[index].intensities);
	//setLightUniformFloat("attenuation", i, gLights[i].attenuation);
	setLightUniformFloat("ambientCoefficient", index, gLights[index].ambientCoefficient);
	setLightUniformFloat("fConstant", index, gLights[index].fConstant);
	setLightUniformFloat("fLinear", index, gLights[index].fLinear);
	setLightUniformFloat("fExp", index, gLights[index].fExp);
}

//--------------------------------------------------------------------------------------------
// rysuje scene OpenGL 
void Scene::Draw(int Selection)
{	
	if (err) return; // sprawdz flage bledu (np. kompilacja shadera)

	// czyscimy bufor kolorow
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int _View = glGetUniformLocation(program, "viewMatrix");
	int _Model = glGetUniformLocation(program, "modelMatrix");
	int _Projection = glGetUniformLocation(program, "projectionMatrix");
	int _LightDirection = glGetUniformLocation(program, "LightDirection");
	int _LightColor = glGetUniformLocation(program, "LightColor");
	//int _LightAmbient = glGetUniformLocation(program, "LightAmbient");
	//int _fConstant = glGetUniformLocation(program, "fConstantAtt");
	//int _fLinear = glGetUniformLocation(program, "fLinearAtt");
	//int _fExp = glGetUniformLocation(program, "fExpAtt");
	int _Sampler = glGetUniformLocation(program, "gSampler");
	int _numLights = glGetUniformLocation(program, "numLights");
	int _NormalMatrix = glGetUniformLocation(program, "normalMatrix");
	int _LightPosition= glGetUniformLocation(program, "LightPosition");
	int _colorAlpha = glGetUniformLocation(program, "colorAlpha");
	int _ObjectId = glGetUniformLocation(program,"ObjectId");
	int _SelectionMode = glGetUniformLocation(program,"SelectionMode");

	glUniform1i(_Sampler, 0);
	// ustaw macierz projekcji na perspektywiczna
	glUniformMatrix4fv(_Projection, 1, GL_FALSE, glm::value_ptr(mProjection));
	
	// inicjuj macierz MV z polozeniem obserwatora

	glm::mat4 mModel = glm::mat4(1.0f);
	glm::mat4 mView = glm::lookAt(glm::vec3(Cam_r*cos(PI*Cam_angle/180.0), camPosY, Cam_r*sin(PI*Cam_angle/180.0)), 
										glm::vec3(0.0f, camPosY-5, camPosZ),
										glm::vec3(0.0f, 1.0f, 0.0f));
	// wyslij MV do shadera
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(mView));
	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModel));

	

	glUniform1i(_numLights, numLights);



	// wyslij macierz normlanych 	
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, 
			glm::value_ptr(glm::transpose(glm::inverse(mModel * mView))));	
	
	belt->Render();
	dirLight(4);
	// ustaw i wyslij kierunek swiatla
	glm::vec3 LightDirection = glm::vec3(0.0, -1.0, 0.0);
	glUniform3fv(_LightDirection, 1, glm::value_ptr(LightDirection));

	if (camshRot > 1.57 && camshRot < 2.07) {
		boom(0, true);
		boom(3, true);
	}
	else {
		boom(0, false);
		boom(3, false);
	}
	

	if (camshRot > 6.22 && camshRot < 6.52) {
		boom(1, true);
		boom(2, true);
	}
	else {
		boom(1, false);
		boom(2, false);
	}
		
	glUniform1f(_colorAlpha, colorAlpha);
	
	
	// nadaj ID obiektu
	
	if (crankRot > 7.854)
		crankRot = 1.57;
	if (camshRot > 7.854)
		camshRot = 1.57;
	crankRotDeg = crankRot * 57.2957795f;
	camshRotDeg = camshRot * 57.2957795f;
	glm::mat4 mTransform = glm::mat4(1.0);
	glm::mat4 mModelLoc = mModel;
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	//belt->Render();
	////////////////////////////////////PISTON 1/////////////////////////////////////////////////
	
	
	yTop = crankRadius * sin(crankRot) + sqrt(rodLength * rodLength - crankRadius * crankRadius * (cos(crankRot)*(cos(crankRot))));
	
	mModelLoc = glm::translate(mModel, glm::vec3(0.0, yTop, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mModelLoc*mTransform * mView))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc*mTransform));
	piston1->Render();
	piston4 -> Render();
	////////////////////////////////////PISTON 2/////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);
	
	yTop2 = crankRadius * -sin(crankRot) - sqrt(rodLength * rodLength - crankRadius * crankRadius * (-cos(crankRot)*(-cos(crankRot))));

	mModelLoc = glm::translate(mModel, glm::vec3(0.0, 7.53126, 0.0));
	mModelLoc = glm::translate(mModelLoc, glm::vec3(0.0, yTop2, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mModelLoc * mView))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc));
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(mView));
	piston2->Render();
	piston3->Render();
	//Obj11->Draw();
	///////////////////////////////////CRANKSHAFT//////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);
	mTransform = glm::rotate(glm::mat4(1.0), crankRotDeg, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModel * mTransform));
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(mView));
	crankshaft->Render();
	////////////////////////////////////CONNECTION ROD 1////////////////////////////////////////////////////////
	
	conRotDeg = -24.47 + (asin((1.71337*cos(crankRot)) / 4.1371) * 57.2957795f);
	
	mTransform = glm::mat4(1.0);
	mModelLoc = glm::translate(mModel, glm::vec3(0.0, 1.71337 * sin(crankRot), 0.0));
	mTransform = glm::translate(mTransform, glm::vec3(0.0, 3.76565, 0.0));
		
	mTransform = glm::rotate(glm::mat4(1.0), conRotDeg, glm::vec3(0.0f, 0.0f, 1.0f));
	mTransform = glm::translate(mTransform, glm::vec3(0.0, -3.76565, 0.0));
	mModelLoc = glm::translate(mModelLoc, glm::vec3(0.0, 3.76565, 0.0));

	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc * mTransform));
	conRod1->Render();
	conRod4->Render();
	////////////////////////////////////CONNECTION ROD 2////////////////////////////////////////////////////////
	con2RotDeg = -(-24.47 + (asin((1.71337*cos(crankRot)) / 4.1371) * 57.2957795f));

	mTransform = glm::mat4(1.0);
	mModelLoc = glm::translate(mModel, glm::vec3(0.0, 1.71337 * -sin(crankRot), 0.0));
	mTransform = glm::translate(mTransform, glm::vec3(0.0, 3.76565, 0.0));
	mTransform = glm::rotate(glm::mat4(1.0), con2RotDeg, glm::vec3(0.0f, 0.0f, 1.0f));
	mTransform = glm::translate(mTransform, glm::vec3(0.0, -3.76565, 0.0));
	mModelLoc = glm::translate(mModelLoc, glm::vec3(0.0, 3.76565, 0.0));

	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc * mTransform));
	conRod2->Render();
	conRod3->Render();
	///////////////////////////////CAMSHAFT//////////////////////////////////////////////////////
		mTransform = glm::mat4(1.0);
		mModelLoc = glm::mat4(1.0);
		mTransform = glm::translate(mTransform, glm::vec3(0.0, 11.63319, 0.0));
		mTransform = glm::rotate(glm::mat4(1.0), camshRotDeg, glm::vec3(0.0f, 0.0f, 1.0f));
		mTransform = glm::translate(mTransform, glm::vec3(0.0, -11.63319, 0.0));
		mModelLoc = glm::translate(mModel, glm::vec3(0.0, 11.63319, 0.0));
		glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

		glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc * mTransform));
		camshaft->Render();
	///////////////////////////////VALVE1//////////////////////////////////////////////////////
	
	mTransform = glm::mat4(1.0);
	
	if (camshRotDeg < 163.3 || camshRotDeg > 283)
		valveMove = 0;
	else if (camshRotDeg < 221.25)
		valveMove = -(camshRot - 2.87) / 2;
	else if (camshRotDeg < 296.3)
		valveMove = -0.45 + (camshRot - 4.088) / 2;

	mModelLoc = glm::translate(mModel, glm::vec3(0.0, valveMove, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc*mTransform));
	valve1->Render();
		
	///////////////////////////////VALVE2//////////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);
	valveMove = 0;
	if (camshRotDeg < 253.6)
		valveMove = 0;
	else if (camshRotDeg < 321.55)
		valveMove = -(camshRot - 4.426) / 2;
	else if (camshRotDeg < 369.85)
		valveMove = -1.1 + (camshRot - 4.04) / 2;

	mModelLoc = glm::translate(mModel, glm::vec3(0.0, valveMove, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc*mTransform));
	valve2->Render();

	///////////////////////////////VALVE3//////////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);

	if (camshRotDeg < 130.14)
		valveMove = -0.15 - (camshRot-1.57) / 2;
	else if (camshRotDeg < 170.84)
		valveMove = -0.47 + (camshRot - 2.27) / 2;
	else if (camshRotDeg < 421.6)
		valveMove = 0;
	
	mModelLoc = glm::translate(mModel, glm::vec3(0.0, valveMove, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc*mTransform));
	valve3->Render();

	///////////////////////////////VALVE4//////////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);

	if (camshRotDeg < 158.7 || camshRotDeg > 290)
		valveMove = 0;
	else if (camshRotDeg < 225)
		valveMove = -(camshRot - 2.77) / 2;
	else if (camshRotDeg < 279)
		valveMove = -0.45 + (camshRot - 3.93) / 2;

	mModelLoc = glm::translate(mModel, glm::vec3(0.0, valveMove, 0.0));
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mTransform * mModelLoc))));

	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModelLoc*mTransform));
	valve4->Render();
	
	///////////////////////////////ENGINE CASE//////////////////////////////////////////////////////
	mTransform = glm::mat4(1.0);
	glUniformMatrix4fv(_NormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mModel * mTransform))));
	glUniformMatrix4fv(_Model, 1, GL_FALSE, glm::value_ptr(mModel));
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(mView));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	engineCase->Render();
	cylinderCase->Render();
	glDisable(GL_BLEND);
	

	//--------------------------------------------------
	// Rysowanie w trybie ortogonalnym
	//--------------------------------------------------

	
}	
//------------------------------- KONIEC PLIKU -----------------------------------------------