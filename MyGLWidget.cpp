#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget()
{
}


bool MyGLWidget::checkPosition(glm::vec3 pos){
	bool valid = true;

  if(pos == tailPos) valid = false;
  if (pos.x >= 14 or pos.z >= 14 or pos.x <= -14 or pos.z <= -14)
    valid = false;

  int i = 0;
  while (valid and i < bodyPos.size()){
    if (bodyPos[i] == pos) 
      valid = false;
    ++i;
  }

	return valid;  
}


void MyGLWidget::keyPressEvent(QKeyEvent* event){
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
      direction = glm::vec3(0,0,-1);
      if (not cronometre)
        updateSnakeGame();

      break;
    }
    case Qt::Key_Down: {
      direction = glm::vec3(0,0,1);
      if (not cronometre)
        updateSnakeGame();

      break;
        }
    case Qt::Key_Left: { 
      direction = glm::vec3(-1,0,0);
      if (not cronometre)
        updateSnakeGame();

      break;
        }  
    case Qt::Key_Right: { 
      direction = glm::vec3(1,0,0);
      if (not cronometre)
        updateSnakeGame();

      break;
        }
    case Qt::Key_C: {
      D2 = !D2;
      viewTransform();
      projectTransform();
      break;
        }           
    case Qt::Key_R: { 
      initializeGL();
      iniEscena();
      iniCamera();
      projectTransform();
      break;
        }  
    case Qt::Key_T: {
      cronometre = !cronometre;
      if (cronometre)
        timer.start(200);
      else
        timer.stop();
      break;
    }
    default: event->ignore(); break;
  }
  update();
}


void MyGLWidget::paintGL(){
    // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);
  
  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  for (int i = 0; i < 4; ++i){
    for (int j = 0; j < 30; ++j){
      glBindVertexArray (VAO_models[PIPE]);
      if (i < 2){
        PipeTransform(glm::vec3(-14.5+j,0,-14.5+i*29));
      }
      if (i >= 2){
        PipeTransform(glm::vec3(-14.5+(i-2)*29,0,-14.5+j));
      }
      glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
    }
  }
  
  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}


void MyGLWidget::iniEscena(){
  radiEscena = sqrt(80);
  centreEscena = glm::vec3(0,0,0);

  headPos = glm::vec3(0,0,0);
  headAngle = 0;
  bodyPos.clear();	    
  bodyPos.push_back(glm::vec3(-1,0,0));    	
  tailPos = glm::vec3(-2,0,0);
  tailAngle = 0;

  marblePos = glm::vec3(10, 0, 0);

  direction = glm::vec3(1,0,0);
  psi = 0;
  theta = 45;
  cronometre = false;
  connect(&timer , SIGNAL (timeout()), this, SLOT (animar()));
}


void MyGLWidget::iniCamera(){
  FOVopt = float(M_PI*84.5)/180.0f;
  obs = glm::vec3(0, 8, 0);
  vrp = glm::vec3(0, -2, 0);
  up = glm::vec3(0, 0, -1);
  fov = FOVopt;
  znear =  1;
  zfar  = 60;
  D2 = false;
  left = bottom = -20;
  right = top = 20;
  factor_psi = 0;
  factor_theta = 0;
  viewTransform();
}


void MyGLWidget::viewTransform(){
  glm::mat4 View(1.0f);
  if (D2)
    View = glm::lookAt (obs, vrp, up);
  
  else {
    View = glm::translate(View,glm::vec3(0,0,-31.62));
    View = glm::rotate(View, glm::radians(theta)-factor_theta,glm::vec3(1,0,0));
    View = glm::rotate(View, glm::radians(psi)-factor_psi,glm::vec3(0,1,0));
    View = glm::translate(View,-vrp);
  }
  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}


void MyGLWidget::projectTransform(){
  glm::mat4 Proj(1.0f);
  if (D2){
    bottom = left = -20;
    right = top = 20;
    if (ra > 1){
      left *= ra;
      right *= ra;
    }
    else if (ra < 1){
      bottom /= ra;
      top /= ra;
    }
    Proj = glm::ortho(left,right,bottom,top,znear,zfar);
  }
  else
    Proj = glm::perspective (fov, ra, znear, zfar);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}


void MyGLWidget::PipeTransform(glm::vec3 pos){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE],escalaModels[PIPE]*3,escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::TerraTransform(){
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(3));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::MarbleTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]/2));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::SnakeHeadTransform(){
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, headPos);
  TG = glm::rotate(TG, glm::radians(headAngle)-glm::radians(90.0f), glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::SnakeTailTransform (){
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::rotate(TG,glm::radians(tailAngle),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e){
  makeCurrent();
  if (e->y() > yClick)
    factor_theta -= (e->y()-yClick)*factorAngleY;
  else if (e->y() < yClick)
    factor_theta += (yClick-e->y())*factorAngleY;
  if (e->x() > xClick)
    factor_psi -= (e->x()-xClick)*factorAngleX;
  else if (e->x() < xClick)
    factor_psi += (xClick-e->x())*factorAngleX;
  xClick = e->x();
  yClick = e->y();

  viewTransform();
  update ();
}


void MyGLWidget::resizeGL(int w, int h){
  #ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
  ra = float(ample)/float(alt);
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;

  if (ra < 1){
    fov = 2*atan(tan(FOVopt/2)/ra);
  }
  projectTransform();
}


void MyGLWidget::animar(){
  makeCurrent();
  updateSnakeGame();
  update();
}