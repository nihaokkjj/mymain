#include <GL/glut.h>
#include"init.h"
#include"drawsun.h"
#include"creat_planet.h"
#include"drawcycle.h"
#include <ctime>
#include <cstdlib>
#include<iostream>

#define WIDTH 800
#define HEIGHT 800
#define M_PI 3.141593

// 定义星点结构体
// 
//struct Star {
//    float x, y, z;  // 星点位置
//    float r, g, b;  // 星点自身发光颜色
//};
//
//// 星点集合类
//class MilkWay {
//public:
//    static const int NUM_STARS = 500;
//    Star stars[NUM_STARS];
//
//    // 初始化星点
//    void init() {
//        srand(static_cast<unsigned int>(time(nullptr)));
//        for (int i = 0; i < NUM_STARS; ++i) {
//            stars[i].x = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
//            stars[i].y = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
//            stars[i].z = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 20.0f;
//
//            // 随机生成星点自身发光颜色
//            stars[i].r = static_cast<float>(rand()) / RAND_MAX;
//            stars[i].g = static_cast<float>(rand()) / RAND_MAX;
//            stars[i].b = static_cast<float>(rand()) / RAND_MAX;
//        }
//    }
//
//    // 绘制星点
//    void draw() {
//        glBegin(GL_POINTS);
//        for (int i = 0; i < NUM_STARS; ++i) {
//            glColor3f(stars[i].r, stars[i].g, stars[i].b);
//            glVertex3f(stars[i].x, stars[i].y, stars[i].z);
//        }
//        glEnd();
//    }
//};
// 定义星点结构体
struct Star {
    float x, y, z;
    float r, g, b;  // 星点自身发光颜色
};
//定义银河的中心区域
float core_milk_big = 10.0f;
float core_milk_small = 3.0f;
// 定义银河系类
class MilkWay {
public:
    static const int NUM_STARS = 10000;
    Star stars[NUM_STARS];
    void init() {
        srand(time(0));
        for (int i = 0; i < NUM_STARS; i++) {
            stars[i].x = (rand() % 50 - 10) / 1.0f;
            stars[i].y = ((rand() % 10 - 5) / 5.0f);
            stars[i].z = (rand() % 50 - 10) / 1.0f;
            // 随机生成星点自身发光颜色
            stars[i].r = ((rand() % 255) / 255) * 1.0f;
            stars[i].g = ((rand() % 255) / 255) * 1.0f;
            stars[i].b = ((rand() % 255) / 255) * 1.0f;
        }
    }
    void draw() {
        glPointSize(1.0f);  // 设置星点大小
        glBegin(GL_POINTS);
        for (int i = 0; i < NUM_STARS; i++) {
           // float distance_milk = sqrt(stars[i].x * stars[i].x + stars[i].y * stars[i].y + stars[i].z * stars[i].z);
            glColor3f(stars[i].r, stars[i].g, stars[i].b);
            //if (distance_milk <= core_milk_big && distance_milk >= core_milk_small)
            //{
                glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            //}
            //else  if (distance_milk > core_milk_big)
            //{
            //    if (rand() % 10 < 4)
            //    glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            //}
            //else
            //{
            //    if (rand() % 10 < 4)
            //        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
            //}
        }
        glEnd();
    }
};