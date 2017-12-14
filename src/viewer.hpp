#pragma once

#include "GLFW/glfw3.h"

const int MAX_FRAMES = 10000;

struct Light {
	float position[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
};
const Light light0 = {
	{ 0.0, 0.0, 50.0, 0.0 },
	{ 0.1, 0.1, 0.1, 0.0 },
	{ 1.0, 1.0, 1.0, 0.0 },
	{ 1.0, 1.0, 1.0, 0.0 }
};
const Light light1 = {
	{ 0.0, 0.0, -50.0, 0.0 },
	{ 0.0, 0.0, 0.0, 0.0 },
	{ 1.0, 1.0, 1.0, 0.0 },
	{ 1.0, 1.0, 1.0, 0.0 }
};

const float light2_position[] = { 0.0, 0.0, 25.0, 0.0 };
const float light2_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
const float light2_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
const float light2_specular[] = { 1.0, 1.0, 1.0, 0.0 };

const float camera_position[] = { 0.0, 100.0, -400 };
const float camera_target[] = { 0.0, 0.0, 0.0 };
const float camera_up[] = {0.0, 1.0, 0.0 };

float camera_angle = 90;
float znear = 15;
float zfar = 35;
float DegToRad = 3.1415927 / 180;

sofa::simplegui::Camera camera;

class Viewer {
	public:
		Viewer() {
			if(!glfwInit()) {
				std::cerr << "Failed to initialize glfw." << std::endl;
			}

			window = glfwCreateWindow(800,600,"DepthMatch", nullptr, nullptr);
			if(!window) {
				std::cerr << "Failed to open window." << std::endl;
			}
			glfwMakeContextCurrent(window);
			glfwSwapInterval(0);

			if(glewInit() != GLEW_OK) {
				std::cerr << "Failed to initialize GLEW." << std::endl;
			}

			glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int w, int h){
					glfwMakeContextCurrent(win);
					glViewport (0, 0, (GLsizei) w, (GLsizei) h);

					sofa::simplegui::Camera cam;
					camera.setPerspective(camera_angle, (GLfloat) w/(GLfloat) h, 1, 10000);
					});

			init_gl();
		}

		void init_gl() {
			glClearColor (0.0, 0.0, 0.0, 0.0);

			glEnable(GL_DEPTH_TEST);

			glLightfv(GL_LIGHT0, GL_AMBIENT, light0.ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light0.diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light0.specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light0.position);

			glLightfv(GL_LIGHT1, GL_AMBIENT, light1.ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light1.diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light1.specular);
			glLightfv(GL_LIGHT1, GL_POSITION, light1.position);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);

			camera.setPerspective(camera_angle, (GLfloat) 800/(GLfloat) 600, 1, 10000);
			camera.setlookAt (
					camera_position[0],camera_position[1],camera_position[2],
					camera_target[0],camera_target[1],camera_target[2],
					camera_up[0], camera_up[1], camera_up[2]
					);
		}

		void set_scene(sofa::simplegui::SofaScene &sofa_scene) {
			scene_painter.reset(new sofa::simplegui::SofaGL(&sofa_scene));
		}

		void update() {
			glfwPollEvents();
			glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			camera.perspective();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			camera.lookAt();

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			scene_painter->draw();

			//draw_debug_triangle();

			glfwSwapBuffers(window);
		}

		void update_camera() {
			float xmin, xmax, ymin, ymax, zmin, zmax;
			scene_painter->getSceneBBox(&xmin,&ymin,&zmin, &xmax,&ymax,&zmax);

			std::cout << xmin << " " << ymin << " " << zmin << " " << std::endl;
			std::cout << xmax << " " << ymax << " " << zmax << " " << std::endl;
			camera.viewAll(xmin,ymin,zmin,xmax,ymax,zmax);
		}

		void draw_debug_triangle() {
			glColor3f (1.0, 0.0, 0.0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glBegin(GL_TRIANGLES);
			glVertex3f(-1.0,-1.0,0.0);
			glVertex3f(1.0,-1.0,0.0);
			glVertex3f(1.0,1.0,0.0);
			glEnd();
		}

	private:
		GLFWwindow *window;
		std::unique_ptr<sofa::simplegui::SofaGL> scene_painter;
};

