#pragma once
void init() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // 定义太阳光源，它是一种白色的光源
    GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat sun_light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
}