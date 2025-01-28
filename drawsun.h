#pragma once

void drawsun()
{
    // 定义太阳的材质并绘制太阳
    GLfloat sun_mat_ambient[] = { 1.0f, 0.6f, 0.2f, 1.0f }; 
    GLfloat sun_mat_diffuse[] = { 1.0f, 0.7f, 0.3f, 1.0f }; 
    GLfloat sun_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLfloat sun_mat_emission[] = { 0.9f, 0.6f, 0.2f, 1.0f }; 
    GLfloat sun_mat_shininess = 50.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

    glutSolidSphere(2, 40, 32);

    //设置光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightPosition[] = { 0.0f, 10.0f, 10.0f, 1.0f };  // 光源位置
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecluar[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // 设置透视投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.33, 1.0, 100.0);  // 设置透视投影
    glMatrixMode(GL_MODELVIEW);

}