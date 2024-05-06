#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);


    virtual void paintGL();

    virtual void iniEscena();
    virtual void iniCamera();
    virtual void viewTransform();
    virtual void projectTransform();

    virtual void PipeTransform(glm::vec3 pos);
    virtual void TerraTransform();
    virtual void MarbleTransform();
    virtual void SnakeHeadTransform();
    virtual void SnakeTailTransform();
    
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void resizeGL(int w, int h);

  public slots:
    void animar();

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    float psi, theta;
    float factor_psi, factor_theta;
    bool D2, cronometre;
    float FOVopt;
    float left,right,bottom,top;
    QTimer timer;
};
