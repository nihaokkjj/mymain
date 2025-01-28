#include"creat_planet.h"
//Planet drawPlanet;

   void Planet::setupLights() {
       glEnable(GL_LIGHTING);
       glEnable(GL_LIGHT0);

       GLfloat light_position[] = { 0.0f, 10.0f, 10.0f, 1.0f };  // 灯光位置
       GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };     // 环境光
       GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };     // 漫反射光
       GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // 镜面反射光

       glLightfv(GL_LIGHT0, GL_POSITION, light_position);
       glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
       glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
       glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   }
    void Planet::drawPlanet(GLfloat radius, GLfloat distance, GLfloat angle, GLfloat mat_ambient[], GLfloat mat_diffuse[], GLfloat mat_specular[], GLfloat mat_emission[], GLfloat mat_shininess) {
        glPushMatrix();
        {
            glRotatef(angle, 0.0f, 1.0f, 0.0f); // 绕Y轴正向旋转
            glTranslatef(distance, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

            glutSolidSphere(radius, 40, 32);
        }
        glPopMatrix();
    }
