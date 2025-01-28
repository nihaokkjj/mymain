#include <GL/glut.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include"milk_way.h"//�Ǻ�
#include"creat_nebula.h"//����

#define WIDTH 800
#define HEIGHT 800

static GLfloat angle1 = 0.0f; // ����ת�Ƕ�
static GLfloat angle2 = 0.0f; // ���ǹ�ת�Ƕ�
static GLfloat angle3 = 0.0f; // �����Ƶ���ת�Ƕ�

Planet creat_planet;
Cycle cycle;
MilkWay milkyWay;
Nebula nebula;
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ����͸��Ч����ͼ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0f, 1.0f, 1.0f, 20.0f); // �������������� 1e6
    // 1���ӽǴ�С;    2����������ʾ�ڻ����ϵ�x��y�����ϵı��� ,��Ϊ1���ᰴʵ�ʷ�Ӧ�����
    //3���ֵԽ�󣬱�ʾ�۲������������ԽԶ
    //4����ɹ۲⵽���������Զ�������������ľ���
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, -12.0, 0.0, 0.0, 0.0, 0.0, 10.5, 0.0);//ǰ���� ��ʾ�������������ϵ�е�λ��
    //�м�����������������ŵ���������ϵ�еĵ��λ�����꣬�������һ���λ�ڻ��������λ��
    //��������������������ĳ�������������������������ϵ�е�����㣬�������Ϊ��վ���������ͷ�ĳ�����������������������ϵ�е�����㣬�����������ϵ�ģ�ֻ���������巽��
    
    // ����̫��
    drawsun();
    //creat_planet.setupLights();
    // �������Ĳ��ʲ����Ƶ���
    GLfloat earth_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat earth_mat_diffuse[] = { 0.0f, 0.0f, 0.5f, 1.0f };
    GLfloat earth_mat_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 1.0f, 1.0f };
    GLfloat earth_mat_shininess = 30.0f;

 
    glPushMatrix(); // ��ʼ�������ı任״̬
    {
        creat_planet.drawPlanet(0.5, 8.0, angle1, earth_mat_ambient, earth_mat_diffuse, earth_mat_specular, earth_mat_emission, earth_mat_shininess);

        // ��������Ĳ��ʲ���������
        GLfloat moon_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_diffuse[] = { 0.72f, 0.73f, 0.47f, 1.0f };
        GLfloat moon_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        GLfloat moon_mat_emission[] = { 0.72f, 0.73f, 0.47f, 1.0f };//����ɫ
        GLfloat moon_mat_shininess = 10.0f;

        // ������ת��ͬʱ���������Ƶ���ת
        glPushMatrix(); // ���浱ǰ�任���󣬼�����ı任����
        {
            glRotatef(angle1, 0.0f, 1.0f, 0.0f);// ��Y��������ת, ������һ������
            glTranslatef(8.0f, 0.0f, 0.0f);//������������̫��������һ��ת
            glRotatef(angle3, 0.0f, 1.0f, 0.0f); // ��ת֮���Ƶ�����ת
            glTranslatef(1.2f, 0.0f, 0.0f); // �ڵ�����ת�İ뾶
            glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
            glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
            glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);

            glutSolidSphere(0.06, 40, 32);
        }
        glPopMatrix(); // �ָ�����ı任����
    }
    glPopMatrix(); // �ָ���û�е���任��״̬

    // ������ǵĲ��ʲ����ƻ���
    GLfloat mars_mat_ambient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_specular[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat mars_mat_shininess = 0.0f;

    creat_planet.drawPlanet(0.45f, 10.0f, angle2, mars_mat_ambient, mars_mat_diffuse, mars_mat_specular, mars_mat_emission, mars_mat_shininess);

    //����ˮ��
    GLfloat mercury_mat_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };//û�й��յ�ʱ��: ǳ��ɫ
    GLfloat mercury_mat_diffuse[] = { 0.4f, 0.4f, 0.35f, 1.0f };//������: ǳ��ɫ
    GLfloat mercury_mat_specular[] = { 0.0f, 0.1f, 0.1f, 1.0f };//���淴ɫ����
    GLfloat mercury_mat_emission[] = { 0.2f, 0.2f, 0.2f, 0.1f };//��������
    GLfloat mercury_mat_shininess = 10.0f;//����ȵ�
    creat_planet.drawPlanet(0.15f, 3.0f, angle2, mercury_mat_ambient, mercury_mat_diffuse, mercury_mat_specular, mercury_mat_emission, mercury_mat_shininess);

    //���ƽ���
    GLfloat venus_mat_ambient[] = { 0.8f, 0.7f, 0.4f, 1.0f };//��ͻ�ɫ
    GLfloat venus_mat_diffuse[] = { 0.8f, 0.65f, 0.4f, 1.0f };//�����Ϊ����ɫ
    GLfloat venus_mat_specular[] = { 0.3f, 0.2f, 0.1f, 1.0f };
    GLfloat venus_mat_emission[] = { 0.2f, 0.16f, 0.1f, 1.0f };
    GLfloat venus_mat_shininess = 10.0f;
    creat_planet.drawPlanet(0.23f, 5.0f, angle2, venus_mat_ambient, venus_mat_diffuse, venus_mat_specular, venus_mat_emission, venus_mat_shininess);

    //����ľ��
    GLfloat Jupiter_mat_ambient[] = { 0.6f, 0.4f, 0.2f, 1.0f };
    GLfloat Jupiter_mat_diffuse[] = { 0.8f, 0.6f, 0.3f, 1.0f };
    GLfloat Jupiter_mat_specular[] = { 0.2f, 0.15f, 0.1f, 1.0f };
    GLfloat Jupiter_mat_emission[] = { 0.16f, 0.12f, 0.06f, 1.0f };
    GLfloat Jupiter_mat_shininess = 10.0f;
    creat_planet.drawPlanet(0.55f, 7.0f, angle2, Jupiter_mat_ambient, Jupiter_mat_diffuse, Jupiter_mat_specular, Jupiter_mat_emission, Jupiter_mat_shininess);
    
    //ľ�ǵĻ�
    glRotatef(angle2, 0.0f, 1.0f, 0.0f);
    glTranslatef(7.0, 0.0f, 0.0f);
    cycle.draw_cycle((float)0.65, (float)0.95, 100);//Ȧ��, Ȧ��

    // ��������ϵ�ǵ�
    glPushMatrix();
    milkyWay.init();
    milkyWay.draw();
    glPopMatrix();

    //��������
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
    glutPostRedisplay(); // �����ػ�����
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutInitWindowPosition(400, 200);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL������ʾ");
    init();

    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);

    glutMainLoop();
    return 0;
}