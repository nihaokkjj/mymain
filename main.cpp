#include <GL/glut.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include"milk_way.h"//星河
#include"creat_nebula.h"//星云

#define WIDTH 800
#define HEIGHT 800

static GLfloat angle1 = 0.0f; // 地球公转角度
static GLfloat angle2 = 0.0f; // 火星公转角度
static GLfloat angle3 = 0.0f; // 月球绕地球公转角度

Planet creat_planet;
Cycle cycle;
MilkWay milkyWay;
Nebula nebula;
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 创建透视效果视图
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0f, 1.0f, 1.0f, 20.0f); // 将所有数据数以 1e6
    // 1可视角大小;    2定义物体显示在画板上的x和y方向上的比例 ,设为1，会按实际反应长宽比
    //3这个值越大，表示观测点距离物体距离越远
    //4定义可观测到的物体的最远处截面相距相机的距离
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, -12.0, 0.0, 0.0, 0.0, 0.0, 10.5, 0.0);//前三个 表示相机在世界坐标系中的位置
    //中间三个定义相机正对着的世界坐标系中的点的位置坐标，成像后这一点会位于画板的中心位置
    //最后三个数定义相机本身的朝向。这三个坐标是在世界坐标系中的坐标点，可以理解为人站立在相机处头的朝向。这三个坐标是世界坐标系中的坐标点，不是相机坐标系的，只是用来定义方向，
    
    // 绘制太阳
    drawsun();
    //creat_planet.setupLights();
    // 定义地球的材质并绘制地球
    GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_shininess = 30.0f;

 
    glPushMatrix(); // 开始保存地球的变换状态
    {
        creat_planet.drawPlanet(0.5, 8.0, angle1, earth_mat_ambient, earth_mat_diffuse, earth_mat_specular, earth_mat_emission, earth_mat_shininess);

        // 定义月球的材质并绘制月球
        GLfloat moon_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_diffuse[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_emission[] = { 0.72f, 0.73f, 0.47f, 1.0f };//调颜色
        GLfloat moon_mat_shininess = 10.0f;

        // 地球自转的同时，让月球绕地球公转
        glPushMatrix(); // 保存当前变换矩阵，即地球的变换矩阵
        {
            glRotatef(angle1, 0.0f, 1.0f, 0.0f);// 绕Y轴正向旋转, 跟地球一个数据
            glTranslatef(8.0f, 0.0f, 0.0f);//先让月球绕着太阳跟地球一起公转
            glRotatef(angle3, 0.0f, 1.0f, 0.0f); // 公转之后绕地球旋转
            glTranslatef(1.2f, 0.0f, 0.0f); // 在地球旋转的半径
            glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);

            glutSolidSphere(0.06, 40, 32);
        }
        glPopMatrix(); // 恢复地球的变换矩阵
    }
    glPopMatrix(); // 恢复到没有地球变换的状态

    // 定义火星的材质并绘制火星
    GLfloat mars_mat_ambient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_shininess = 0.0f;

    creat_planet.drawPlanet(0.45f, 10.0f, angle2, mars_mat_ambient, mars_mat_diffuse, mars_mat_specular, mars_mat_emission, mars_mat_shininess);

    //绘制水星
    GLfloat mercury_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };//没有光照的时候: 浅灰色
    GLfloat mercury_mat_diffuse[] = { 0.4f, 0.4f, 0.35f, 1.0f };//漫反射: 浅灰色
    GLfloat mercury_mat_specular[] = { 0.0f, 0.1f, 0.1f, 1.0f };//镜面反色较弱
    GLfloat mercury_mat_emission[] = { 0.2f, 0.2f, 0.2f, 0.1f };//本身不发光
    GLfloat mercury_mat_shininess = 10.0f;//光泽度低
    creat_planet.drawPlanet(0.15f, 3.0f, angle2, mercury_mat_ambient, mercury_mat_diffuse, mercury_mat_specular, mercury_mat_emission, mercury_mat_shininess);

    //绘制金星
    GLfloat venus_mat_ambient[] = { 0.8f, 0.7f, 0.4f, 1.0f };//柔和黄色
    GLfloat venus_mat_diffuse[] = { 0.8f, 0.65f, 0.4f, 1.0f };//反射成为亮黄色
    GLfloat venus_mat_specular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    GLfloat venus_mat_emission[] = { 0.2f, 0.16f, 0.1f, 1.0f };
    GLfloat venus_mat_shininess = 10.0f;
    creat_planet.drawPlanet(0.23f, 5.0f, angle2, venus_mat_ambient, venus_mat_diffuse, venus_mat_specular, venus_mat_emission, venus_mat_shininess);

    //绘制木星
    GLfloat Jupiter_mat_ambient[] = { 0.6f, 0.4f, 0.2f, 1.0f };
    GLfloat Jupiter_mat_diffuse[] = { 0.8f, 0.6f, 0.3f, 1.0f };
    GLfloat Jupiter_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
    GLfloat Jupiter_mat_emission[] = { 0.16f, 0.12f, 0.06f, 1.0f };
    GLfloat Jupiter_mat_shininess = 10.0f;
    creat_planet.drawPlanet(0.55f, 7.0f, angle2, Jupiter_mat_ambient, Jupiter_mat_diffuse, Jupiter_mat_specular, Jupiter_mat_emission, Jupiter_mat_shininess);
    
    //木星的环
    glRotatef(angle2, 0.0f, 1.0f, 0.0f);
    glTranslatef(7.0, 0.0f, 0.0f);
    cycle.draw_cycle((float)0.65, (float)0.95, 100);//圈内, 圈外

    // 绘制银河系星点
    glPushMatrix();
    milkyWay.init();
    milkyWay.draw();
    glPopMatrix();

    //绘制星云
    glPushMatrix();
    nebula.initnebula();
    nebula.drawnebula();
    glPopMatrix();

    glutSwapBuffers();
}

void myIdle(void)
{
    angle1 += 0.03f;
    angle2 += 0.02f;
    angle3 += 0.02f;
    if (angle1 >= 360.0f)
        angle1 = 0.0f;
    if (angle2 >= 360.0f)
        angle2 = 0.0f;
    if (angle3 >= 360.0f)
        angle3 = 0.0f;
    glutPostRedisplay(); // 发送重绘请求
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(400, 200);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL光照演示");
    init();

    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);

    glutMainLoop();
    return 0;
}