#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include "Player.h"
#include "Bullet.h"
#include "DrawBresenhamLine.h"
#include "MainConst.h"

using namespace std;

Player player(50, 50, 0);
Bullet bullets[MAX_BULLET_ON_SCREEN];

typedef struct {
	int active;
	double  x, y, dx, dy, bullet_phi;
}  Bullet1;
 
static int shoot = 0;
static Bullet1 bullets1[MAX_BULLET_ON_SCREEN];

string buff;
string sincos_buff;
string bullet_buff;

void update(int value);

void KeyboardMove(int key, int _x, int _y){
	switch (key){
		case GLUT_KEY_LEFT:
			player.angle += ROTATE_SPEED;
			break;
		case GLUT_KEY_RIGHT:
			player.angle -= ROTATE_SPEED;
			break;
		case GLUT_KEY_DOWN:
			player.x = player.x + PLAYER_SPEED * sin(player.angle * M_PI / 180);
			player.y = player.y - PLAYER_SPEED * cos(player.angle * M_PI / 180);
			break;
		case GLUT_KEY_UP:
			player.x = player.x - PLAYER_SPEED * sin(player.angle * M_PI / 180);
			player.y = player.y + PLAYER_SPEED * cos(player.angle * M_PI / 180);
			break;
	}
}

void keyboardListener(unsigned char c, int x, int y) {
	switch (c) {
		case 'q':
			exit(0);
			break;
		case ' ':
			//bullets.active = 1;
			shoot = 1; //помутнение рассудка
			break;
	}
}

void output(GLfloat x, GLfloat y, string text){
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(0.08, 0.08, 0);
	for(int i = 0; i < text.length(); i++){
		glColor3d(1, 1, 1);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
	}
	glPopMatrix();
}

string LDToStr(double one){
	std::stringstream ss;
	ss << one;
	return ss.str();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	player.drawPlayer(player.x, player.y, player.angle);
	
	for (int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {
		if (bullets[i].active) {
			bullets[i].drawBullet();
		}
	}

	/*Debug on screen*/
	buff = "You looks good today";
	output(5, HEIGHT_D-10, buff);
	/*End debug on screen*/

	glutSwapBuffers();
}

void update(int value){
	player.update();

	if (shoot == 1){
		for(int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {
            if(bullets[i].active == 0) {
                bullets[i].active = 1;
                bullets[i].x = player.x;
                bullets[i].y = player.y;
                bullets[i].angle = player.angle;
                break;
            }
        }
		shoot = 0;
	}
	for (int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {
		bullets[i].update();
	}
	
	glutPostRedisplay();
	glutTimerFunc(33, update, 0);
}

void Initialize() {
	glClearColor(0, 0, 0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, WIDTH_D, 0.0, HEIGHT_D, -5.0, 5.0);
	//glutIdleFunc(update);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH_D, HEIGHT_D);
	glutCreateWindow("Window");
	Initialize();
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(KeyboardMove);
	glutTimerFunc(33, update, 0);
	glutMainLoop();
	return 0;
}