#include <GL/glew.h> 
#include <GL/glut.h>  
#include <GLFW/glfw3.h>  
#include<iostream>
#include<SOIL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include<math.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include"milk_way.h"//星河
#include"creat_nebula.h"//星云

#define WIDTH 1000
#define HEIGHT 1000
#define M_PI 3.141592

unsigned int texture;
const int sun_texture_count = 5;
unsigned int sunTextures[sun_texture_count];
int currentSunTextureIndex = 0;
int nextSunTextureIndex = 1;

// 定义天体结构体
struct CelestialBody {
    float radius;  // 天体半径
    float orbitRadius;  // 轨道半径
    float orbitCenterX; // 轨道中心 X 坐标
    float orbitCenterY; // 轨道中心 Y 坐标
    float orbitCenterZ; // 轨道中心 Z 坐标
    bool isSelected;    // 是否被选中
};

// 射线结构体
struct Ray {
    float originX, originY, originZ;
    float directionX, directionY, directionZ;
};

// 初始化天体
CelestialBody earth = { 0.32f, 5.4f, 0.0f, 0.0f, 0.0f, false };
CelestialBody moon = { 0.06f, 0.55f, 0.0f, 0.0f, 0.0f, false };
CelestialBody mercury = { 0.15f, 2.8f, 0.0f, 0.0f, 0.0f, false };
CelestialBody venus = { 0.19f, 4.0f, 0.0f, 0.0f, 0.0f, false };
CelestialBody Saturn = { 0.47f, 9.5f, 0.0f, 0.0f, 0.0f, false };
CelestialBody mars = { 0.3f, 6.6f, 0.0f, 0.0f, 0.0f, false };
CelestialBody Jupiter = { 0.5f, 7.9f, 0.0f, 0.0f, 0.0f, false };
CelestialBody Neptune = { 0.3f, 11.9f, 0.0f, 0.0f, 0.0f, false };
CelestialBody Uranus = { 0.35f, 10.9f, 0.0f, 0.0f, 0.0f, false };

// 全局变量
const int STAR_COUNT = 100;  // 星星的数量
float starX[STAR_COUNT];     // 星星的 x 坐标
float starY[STAR_COUNT];     // 星星的 y 坐标
float starZ[STAR_COUNT];     // 星星的 z 坐标
float mouseX = 0.0f;         // 鼠标的 x 坐标
float mouseY = 0.0f;
float blendFactor = 0.0f;  // 混合因子，范围从 0 到 1
const float blendStep = 0.99f;  // 每次帧更新时混合因子的增量

static GLfloat angle1 = 0.0f, angle4, angle5, angle6, angle7, angle8; // 地球公转角度
static GLfloat angle2 = 0.0f; // 火星公转角度
static GLfloat angle3 = 0.0f; // 月球绕地球公转角度

// 相机视角参数
float cameraX = 0.0f;
float cameraY = 12.0f;
float cameraZ = -20.0f;
float centerX = 0.0f;
float centerY = 0.0f;
float centerZ = 0.0f;
float upX = 0.0f;
float upY = 10.5f;
float upZ = 0.0f;

// 鼠标上一位置
int lastMouseX = 0;
int lastMouseY = 0;

// 鼠标灵敏度
const float mouseSensitivity = 0.003f;

// 小行星带
const int ASTEROID_COUNT = 200;
float asteroidX[ASTEROID_COUNT];
float asteroidY[ASTEROID_COUNT];
float asteroidZ[ASTEROID_COUNT];
float asteroidSize[ASTEROID_COUNT];
float asteroidAngle[ASTEROID_COUNT];

Planet creat_planet;
Cycle cycle;
MilkWay milkyWay;
Nebula nebula;

// 计算射线
Ray calculateRay(int mouseX, int mouseY, int windowWidth, int windowHeight) {

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLdouble nearPoint[3], farPoint[3];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    // 计算近平面上的点
    gluUnProject(mouseX, viewport[3] - mouseY, 0.0, modelview, projection, viewport, &nearPoint[0], &nearPoint[1], &nearPoint[2]);
    
    // 计算远平面上的点
    gluUnProject(mouseX, viewport[3] - mouseY, 1.0, modelview, projection, viewport, &farPoint[0], &farPoint[1], &farPoint[2]);
    Ray ray;
    ray.originX = nearPoint[0];
    ray.originY = nearPoint[1];
    ray.originZ = nearPoint[2];
    ray.directionX = farPoint[0] - nearPoint[0];
    ray.directionY = farPoint[1] - nearPoint[1];
    ray.directionZ = farPoint[2] - nearPoint[2];
    return ray;
}

// 射线与轨道相交检测
bool rayIntersectsOrbit(const Ray& ray, const CelestialBody& body) {
    
    float a = ray.directionX * ray.directionX + ray.directionZ * ray.directionZ;
    float b = 2 * (ray.directionX * (ray.originX - body.orbitCenterX) + ray.directionZ * (ray.originZ - body.orbitCenterZ));
    float c = (ray.originX - body.orbitCenterX) * (ray.originX - body.orbitCenterX) + (ray.originZ - body.orbitCenterZ) * (ray.originZ - body.orbitCenterZ) - body.orbitRadius * body.orbitRadius;

    float discriminant = b * b - 4 * a * c;
    return discriminant >= 0;
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Ray ray = calculateRay(x, y, WIDTH, HEIGHT);

        // 检测地球轨道
        earth.isSelected = rayIntersectsOrbit(ray, earth);

        // 检测月球轨道
        // 先移动到地球位置
        float earthX = earth.orbitRadius * cos(angle3 * M_PI / 180.0f);
        float earthZ = earth.orbitRadius * sin(angle3 * M_PI / 180.0f);
        moon.orbitCenterX = earthX;
        moon.orbitCenterZ = earthZ;
        moon.isSelected = rayIntersectsOrbit(ray, moon);

        mercury.isSelected = rayIntersectsOrbit(ray, mercury);
        venus.isSelected = rayIntersectsOrbit(ray, venus);
        Saturn.isSelected = rayIntersectsOrbit(ray, Saturn);
        mars.isSelected = rayIntersectsOrbit(ray, mars);
        Jupiter.isSelected = rayIntersectsOrbit(ray, Jupiter);
        Neptune.isSelected = rayIntersectsOrbit(ray, Neptune);
        Uranus.isSelected = rayIntersectsOrbit(ray, Uranus);
        
        glutPostRedisplay();
    }
}

void drawBlendedSun(float x, float y, float width, float height) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 绘制当前纹理
    glBindTexture(GL_TEXTURE_2D, sunTextures[currentSunTextureIndex]);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - blendFactor);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();
    // 绘制下一纹理
    glBindTexture(GL_TEXTURE_2D, sunTextures[nextSunTextureIndex]);
    glColor4f(1.0f, 1.0f, 1.0f, blendFactor);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void initTextures() {
    glGenTextures(sun_texture_count, sunTextures);

    const char* filenames[sun_texture_count] = { "4.jpg",  "5.jpg", "6.png","7.jpg" ,"3.jpg" };

    for (int i = 0; i < sun_texture_count; ++i) {
        glBindTexture(GL_TEXTURE_2D, sunTextures[i]);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 加载并生成纹理
        int sun_width, sun_height, nrChannels;
        // stbi_set_flip_vertically_on_load(true); // 翻转纹理的Y轴
        unsigned char* data = stbi_load(filenames[i], &sun_width, &sun_height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sun_width, sun_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            // std::cout << "Failed to load texture" << std::endl;
            std::cout << "Failed to load texture: " << filenames[i] << std::endl;

            exit(1); // 退出程序
        }
        stbi_image_free(data);
    }
}

// 贴图位置
void drawTexturedSquare(float x, float y, float width, float height, unsigned int texId) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);

    glBegin(GL_QUADS);
    // 左下角
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(x, y);
    // 右下角
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(x + width, y);
    // 右上角
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(x + width, y + height);
    // 左上角
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(x, y + height);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void mouseMotion(int x, int y) {
    // 更新鼠标上一位置
    lastMouseX = x;
    lastMouseY = y;

    float aspect = (float)WIDTH / (float)HEIGHT;
    mouseX = (2.0f * x / WIDTH - 1.0f) * aspect;
    mouseY = 1.0f - 2.0f * y / HEIGHT;
}

//视角变动
void keyboard(unsigned char key, int x, int y) {
    const float moveSpeed = 0.1f;
    switch (key) {
    case 'w':
        cameraZ += moveSpeed;
        break;
    case 's':
        cameraZ -= moveSpeed;
        break;
    case 'a':
        cameraX -= moveSpeed;
        break;
    case 'd':
        cameraX += moveSpeed;
        break;
    case 'q':
        cameraY += moveSpeed;
        break;
    case 'e':
        cameraY -= moveSpeed;
        break;
    }
    glutPostRedisplay();
}

////
//void mouse(int button, int state, int x, int y) {
//    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//        // 这里可以添加点击检测逻辑，判断点击的是哪个天体
//        std::cout << "Mouse clicked at (" << x << ", " << y << ")" << std::endl;
//    }
//}

//显示行星轨道
void drawOrbit(float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float angle = 2.0f * M_PI * (float)i / (float)segments;
        float x = radius * std::cos(angle);
        float z = radius * std::sin(angle);
        glVertex3f(x, 0.0f, z);
    }
    glEnd();
}
void myDisplay(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 创建透视效果视图
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(120.0f, 1.0f, 1.0f, 20.0f); // 将所有数据数以 1e6
    // 1可视角大小;    2定义物体显示在画板上的x和y方向上的比例 ,设为1，会按实际反应长宽比
    //3这个值越大，表示观测点距离物体距离越远
    //4定义可观测到的物体的最远处截面相距相机的距离
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, centerX, centerY, centerZ, upX, upY, upZ);//前三个 表示相机在世界坐标系中的位置
    //中间三个定义相机正对着的世界坐标系中的点的位置坐标，成像后这一点会位于画板的中心位置
    //最后三个数定义相机
    // 本身的朝向。这三个坐标是在世界坐标系中的坐标点，可以理解为人站立在相机处头的朝向。这三个坐标是世界坐标系中的坐标点，不是相机坐标系的，只是用来定义方向，

    //绘制太阳
    drawsun();

    // 绘制太阳光线
    drawSunRays();

    //光晕
    drawSunHalo();
    // 绘制太阳(贴图方式)
    //glPushMatrix();
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, sunTextures[currentSunTextureIndex]);
    //float x = -1.15f;
    //float y = -1.15f;
    //float squareWidth = 2.38f;
    //float squareHeight = 2.38f;

    //drawBlendedSun(x, y, squareWidth, squareHeight);
    ////drawTexturedSquare(x, y, squareWidth, squareHeight, sunTextures[currentSunTextureIndex]);
    ////drawsun();

    //glDisable(GL_TEXTURE_2D);
    //glPopMatrix();

    // 定义地球的材质并绘制地球
    GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_shininess = 30.0f;

    glPushMatrix(); // 开始保存地球的变换状态
    {
        creat_planet.drawPlanet(0.32f, 5.4f, angle3, earth_mat_ambient, earth_mat_diffuse, earth_mat_specular, earth_mat_emission, earth_mat_shininess);

        // 绘制地球轨道
        glPushMatrix();
        if (earth.isSelected) {
            glColor3f(0.0f, 0.1f, 0.9f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(earth.orbitRadius, 100);
        glPopMatrix();

        // 定义月球的材质并绘制月球
        GLfloat moon_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_diffuse[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_emission[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_shininess = 10.0f;

        // 地球自转的同时，让月球绕地球公转
        glPushMatrix(); // 保存当前变换矩阵，即地球的变换矩阵
        {
            glRotatef(angle3, 0.0f, 1.0f, 0.0f);
            glTranslatef(5.4f, 0.0f, 0.0f);
            glRotatef(angle2, 0.0f, 1.0f, 0.0f);
            glTranslatef(0.55f, 0.0f, 0.0f);
            glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);
            glutSolidSphere(0.06f, 40, 32);
        }
        glPopMatrix(); // 恢复地球的变换矩阵

        // 定义火星的材质并绘制火星
        GLfloat mars_mat_ambient[] = { 0.3f, 0.1f, 0.0f, 1.0f };
        GLfloat mars_mat_diffuse[] = { 0.8f, 0.4f, 0.2f, 1.0f };
        GLfloat mars_mat_specular[] = { 0.3f, 0.1f, 0.05f, 1.0f };
        GLfloat mars_mat_emission[] = { 0.05f, 0.02f, 0.01f, 1.0f };
        GLfloat mars_mat_shininess = 15.0f;

        creat_planet.drawPlanet(0.3f, 6.6f, angle4, mars_mat_ambient, mars_mat_diffuse, mars_mat_specular, mars_mat_emission, mars_mat_shininess);

        // 绘制火星轨道
        glPushMatrix();
        if (mars.isSelected) {
            glColor3f(0.3f, 0.1f, 0.0f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(mars.orbitRadius, 100);
        glPopMatrix();

        //绘制水星
        GLfloat mercury_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };//没有光照的时候: 浅灰色
        GLfloat mercury_mat_diffuse[] = { 0.4f, 0.4f, 0.35f, 1.0f };//漫反射: 浅灰色
        GLfloat mercury_mat_specular[] = { 0.0f, 0.1f, 0.1f, 1.0f };//镜面反色较弱
        GLfloat mercury_mat_emission[] = { 0.2f, 0.2f, 0.2f, 0.1f };//本身不发光
        GLfloat mercury_mat_shininess = 5.0f;//光泽度低

        creat_planet.drawPlanet(0.15f, 2.8f, angle1, mercury_mat_ambient, mercury_mat_diffuse, mercury_mat_specular, mercury_mat_emission, mercury_mat_shininess);

        // 绘制水星轨道
        glPushMatrix();
        if (mercury.isSelected) {
            glColor3f(0.25f, 0.25f, 0.25f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(mercury.orbitRadius, 100);
        glPopMatrix();

        //绘制金星
        GLfloat venus_mat_ambient[] = { 0.8f, 0.7f, 0.4f, 1.0f };//柔和黄色
        GLfloat venus_mat_diffuse[] = { 0.8f, 0.65f, 0.4f, 1.0f };//反射成为亮黄色
        GLfloat venus_mat_specular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
        GLfloat venus_mat_emission[] = { 0.2f, 0.16f, 0.1f, 1.0f };
        GLfloat venus_mat_shininess = 10.0f;

        creat_planet.drawPlanet(0.19f, 4.0f, angle2, venus_mat_ambient, venus_mat_diffuse, venus_mat_specular, venus_mat_emission, venus_mat_shininess);

        // 绘制金星轨道
        glPushMatrix();
        if (venus.isSelected) {
            glColor3f( 0.8f, 0.7f, 0.4f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(venus.orbitRadius, 100);
        glPopMatrix();

        // 绘制土星
        GLfloat Saturn_mat_ambient[] = { 0.5f, 0.5f, 0.0f, 1.0f };
        GLfloat Saturn_mat_diffuse[] = { 0.8f, 0.81f, 0.27f, 1.0f };
        GLfloat Saturn_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
        GLfloat Saturn_mat_emission[] = { 0.1f, 0.1f, 0.0f, 1.0f };
        GLfloat Saturn_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.47f, 9.5f, angle6, Saturn_mat_ambient, Saturn_mat_diffuse, Saturn_mat_specular, Saturn_mat_emission, Saturn_mat_shininess);

        // 绘制土星轨道
        glPushMatrix();
        if (Saturn.isSelected) {
            glColor3f(0.5f, 0.5f, 0.0f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(Saturn.orbitRadius, 100);
        glPopMatrix();

        // 土星的环
        glPushMatrix();
        glRotatef(angle6, 0.0f, 1.0f, 0.0f);
        glTranslatef(9.5f, 0.0f, 0.0f);
        cycle.draw_cycle(0.6f, 0.9f, 100);//圈内, 圈外
        glPopMatrix();

        // 绘制木星
        GLfloat Jupiter_mat_ambient[] = { 0.6f, 0.6f, 0.0f, 1.0f };
        GLfloat Jupiter_mat_diffuse[] = { 0.82f, 0.87f, 0.27f, 1.0f };
        GLfloat Jupiter_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
        GLfloat Jupiter_mat_emission[] = { 0.1f, 0.1f, 0.0f, 1.0f };
        GLfloat Jupiter_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.5f, 7.9f, angle5, Jupiter_mat_ambient, Jupiter_mat_diffuse, Jupiter_mat_specular, Jupiter_mat_emission, Jupiter_mat_shininess);

        // 绘制木星轨道
        if (Jupiter.isSelected) {
            glColor3f(0.6f, 0.6f, 0.0f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(Jupiter.orbitRadius, 100);

        // 绘制天王星
        GLfloat Uranus_mat_ambient[] = { 0.488f, 0.95f, 0.786f, 1.0f };
        GLfloat Uranus_mat_diffuse[] = { 0.388f, 0.9f, 0.686f, 1.0f };
        GLfloat Uranus_mat_specular[] = { 0.088f, 0.2f, 0.186f, 1.0f };
        GLfloat Uranus_mat_emission[] = { 0.1f, 0.2f, 0.15f, 1.0f };
        GLfloat Uranus_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.35f, 10.9f, angle7, Uranus_mat_ambient, Uranus_mat_diffuse, Uranus_mat_specular, Uranus_mat_emission, Uranus_mat_shininess);

        //// 绘制天王星轨道
        if (Uranus.isSelected) {
            glColor3f(0.488f, 0.95f, 0.786f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit (Uranus.orbitRadius, 100);

        //绘制海王星
        GLfloat Neptune_mat_ambient[] = { 0.5f, 0.8f, 0.95f, 1.0f };
        GLfloat Neptune_mat_diffuse[] = { 0.458f, 0.756f, 0.921f, 1.0f };
        GLfloat Neptune_mat_specular[] = { 0.1f, 0.15f, 0.2f, 1.0f };
        GLfloat Neptune_mat_emission[] = { 0.1f, 0.1f, 0.2f, 1.0f };
        GLfloat Neptune_mat_shininess = 5.0f;

        creat_planet.drawPlanet(0.3f, 11.9f, angle8, Neptune_mat_ambient, Neptune_mat_diffuse, Neptune_mat_specular, Neptune_mat_emission, Neptune_mat_shininess);

        // 绘制sea王星轨道
        glPushMatrix();
        if (Neptune.isSelected) {
            glColor3f(0.5f, 0.8f, 0.95f);
        }
        else {
            glColor3f(0.0f, 0.0f, 1.0f); // 未选中时轨道颜色为蓝色
        }
        drawOrbit(Neptune.orbitRadius, 100);
        glPopMatrix();

    }
    glPopMatrix();

    // 绘制银河系星点
    glPushMatrix();
    milkyWay.init();
    milkyWay.draw();
    glPopMatrix();

    // 绘制星云
    glPushMatrix();
    nebula.initnebula();
    nebula.drawnebula();
    glPopMatrix();

    // 绘制星星
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < STAR_COUNT; ++i) {

        float size = 2.0f - (starZ[i] + 20.0f) / 40.0f;
        if (size < 0.5f) size = 0.2f; // 确保最小大小

        float alpha = 1.0f - (starZ[i] + 20.0f) / 40.0f;
        if (alpha < 0.2f) alpha = 0.2f;

        glColor4f(1.0f, 1.0f, 1.0f, alpha);
        glPointSize(size);
        glBegin(GL_POINTS);
        glVertex3f(starX[i], starY[i], starZ[i]);
        glEnd();
    }
    glEnd();

    // 绘制小行星带
    glColor3f(0.6f, 0.6f, 0.6f); // 设置小行星的颜色为灰色
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        glPushMatrix();
        glTranslatef(asteroidX[i], asteroidY[i], asteroidZ[i]);
        glRotatef(asteroidAngle[i], 0.0f, 1.0f, 0.0f); // 绕 Y 轴旋转
        glutSolidSphere(asteroidSize[i], 10, 10); // 绘制小行星
        glPopMatrix();
    }
    glutSwapBuffers();
}

void myIdle(void) {
    angle1 += 0.02f;
    angle2 += 0.0146f;
    angle3 += 0.0124f;
    angle4 += 0.01f;
    angle5 += 0.0054f;
    angle6 += 0.004f;
    angle7 += 0.0028f;
    angle8 += 0.0022f;

    if (angle1 >= 360.0f) angle1 = 0.0f;
    if (angle2 >= 360.0f) angle2 = 0.0f;
    if (angle3 >= 360.0f) angle3 = 0.0f;
    // 更新星星的位置，让它们向鼠标位置聚集
    for (int i = 0; i < STAR_COUNT; ++i) {
        float dx = mouseX - starX[i];
        float dy = mouseY - starY[i];
        float dz = 0.0f - starZ[i];
        float speedFactor = 0.00001f + (starZ[i] + 20.0f) / 40.0f * 0.001f;

        starX[i] += dx * speedFactor;
        starY[i] += dy * speedFactor;
        starZ[i] += dz * speedFactor;
    }

    // 更新小行星带的旋转角度
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        asteroidAngle[i] += 0.05f; // 调整旋转速度
        if (asteroidAngle[i] >= 360.0f) asteroidAngle[i] = 0.0f;
    }

    // 更新太阳纹理索引
    //currentSunTextureIndex = (currentSunTextureIndex + 1) % sun_texture_count;
    // 更新混合因子
    blendFactor += blendStep;
    if (blendFactor >= 1.0f) {
        blendFactor = 0.0f;
        currentSunTextureIndex = nextSunTextureIndex;
        nextSunTextureIndex = (nextSunTextureIndex + 1) % sun_texture_count;
    }

    glutPostRedisplay(); // 发送重绘请求
}

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowPosition(400, 200);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL光照演示");

    glewInit();
    init();
    initTextures();

    // 初始化星星的位置
    for (int i = 0; i < STAR_COUNT; ++i) {
        // 随机生成星星的位置，范围可以根据需要调整
        starX[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        starY[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
        starZ[i] = (float)(rand() % 200 - 100) / 100.0f * 10.0f;
    }

    // 初始化小行星带
    for (int i = 0; i < ASTEROID_COUNT; ++i) {
        // 随机生成小行星的角度
        float angle = (float)(rand() % 360) * M_PI / 180.0f;
        // 随机生成小行星到中心的距离，模拟环形分布
        float distance = (float)(rand() % 100) / 100.0f * 1.0f + 6.7f; // 距离范围在 6.6到 7.9 之间,火星与木星
        asteroidX[i] = cos(angle) * distance;
        asteroidY[i] = 0.0f; // 假设小行星带在 XY 平面上
        asteroidZ[i] = sin(angle) * distance;

        // 随机生成小行星的大小
        asteroidSize[i] = (float)(rand() % 10) / 1000.0f * 5.0f + 0.001f; // 大小范围在 0.001 到 0.006 之间

        // 初始化旋转角度
        asteroidAngle[i] = 0.0f;
    }

    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);

    //星星在鼠标附近聚集
    glutPassiveMotionFunc(mouseMotion);

    //视角跟随键盘
    glutKeyboardFunc(keyboard);

    //跟随鼠标移动查看轨道
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
