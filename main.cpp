/* 
 * File:   main.cpp
 * Author: Administrator
 *
 * Created on 25 mei 2012, 22:54
 */

#include <cstdlib>
#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Hl1DataManager.h"
#include "Tracer.h"
#include "Camera.h"
#include "vector3.h"
#include "uiFont.h"
#include "SunShadows.h"
#include <set>

using namespace std;

#define FOV 60.0f

namespace Movement
{
	enum eType
	{
		forward = 0,
		backward,
		left,
		right,
		turn
	};
}

static bool _movement[5] = { false };
static int _mouse[2] = { 0, 0, };
static int _windowSize[2] = { 800, 600, };
static Camera _camera;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_W) _movement[Movement::forward] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_S) _movement[Movement::backward] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_A) _movement[Movement::left] = action != GLFW_RELEASE;
    if (key == GLFW_KEY_D) _movement[Movement::right] = action != GLFW_RELEASE;

}

static void cursorpos_callback(GLFWwindow* window, double x, double y)
{
    if (_movement[Movement::turn])
    {
        _camera.rotateDegree(y-_mouse[1], x-_mouse[0]);
    }

    _mouse[0] = int(x);
    _mouse[1] = int(y);
}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1)
    {
        _movement[Movement::turn] = action != GLFW_RELEASE;
    }
}

static void windowsize_callback(GLFWwindow* window, int width, int height)
{
    _windowSize[0] = width;
    _windowSize[1] = height;
}

/*
sf::Event Event;
while (window.GetEvent(Event))
{
    if (Event.Type == sf::Event::Closed)
        window.Close();

    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
        window.Close();

    if (Event.Type == sf::Event::Resized)
    {
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);

        // Setup a perspective projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(FOV, float(Event.Size.Width) / float(Event.Size.Height), 1.0f, 5000.f);
    }

    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Space))
        enableTracing = !enableTracing;

    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::W)) movement[Movement::forward] = true;
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::S)) movement[Movement::backward] = true;
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::A)) movement[Movement::left] = true;
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::D)) movement[Movement::right] = true;

    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Up)) f += 1;
    if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Down)) f -= 1;

    if ((Event.Type == sf::Event::KeyReleased) && (Event.Key.Code == sf::Key::W)) movement[Movement::forward] = false;
    if ((Event.Type == sf::Event::KeyReleased) && (Event.Key.Code == sf::Key::S)) movement[Movement::backward] = false;
    if ((Event.Type == sf::Event::KeyReleased) && (Event.Key.Code == sf::Key::A)) movement[Movement::left] = false;
    if ((Event.Type == sf::Event::KeyReleased) && (Event.Key.Code == sf::Key::D)) movement[Movement::right] = false;

    if ((Event.Type == sf::Event::MouseButtonReleased) && (Event.MouseButton.Button == sf::Mouse::Left)) movement[Movement::turn] = false;
    if ((Event.Type == sf::Event::MouseButtonPressed) && (Event.MouseButton.Button == sf::Mouse::Left))
    {
        mouse[0] = Event.MouseButton.X;
        mouse[1] = Event.MouseButton.Y;
        movement[Movement::turn] = true;
    }
    if ((Event.Type == sf::Event::MouseMoved) && movement[Movement::turn])
    {
        camera.rotateDegree(Event.MouseMove.Y-mouse[1], Event.MouseMove.X-mouse[0]);
        mouse[0] = Event.MouseMove.X;
        mouse[1] = Event.MouseMove.Y;
    }
}*/

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
                  float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;
    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;
    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

// Matrix will receive the calculated perspective matrix.
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
                      float znear, float zfar)
{
    float ymax, xmax;
    float temp, temp2, temp3, temp4;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    // ymin = -ymax;
    // xmin = -ymax * aspectRatio;
    xmax = ymax * aspectRatio;
    glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        return 0;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Triangle", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursorpos_callback);
    glfwSetMouseButtonCallback(window, mousebutton_callback);
    glfwSetWindowSizeCallback(window, windowsize_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

	char positionPrint[256] = { 0 };
	
	std::set<int> visibleFaces;
    ui::Font font;
    int f = 1;
    _camera.position = Vector3(500, -400, 300);

    Hl1DataManager data;
#ifdef WIN32
    glActiveTextureARB(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
	font.initializeFont("C:\\WINDOWS\\Fonts\\verdana.TTF", 32.0f);
	glActiveTextureARB(GL_TEXTURE1);
    if (data.load("C:/Games/Counter-Strike1.3/cstrike/maps/de_cbble.bsp") == false)
#else
	glActiveTextureARB(GL_TEXTURE0);
	font.initializeFont("/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-BI.ttf", 32.0f);
	glActiveTextureARB(GL_TEXTURE1);
	if (data.load("/media/data/Games/Counter-Strike1.3/cstrike/maps/de_cbble.bsp") == false)
#endif
	{
		cout << "Could not load Cbble" << endl;
		return 0;
	}
	
	Vector3 lightDir(85, -32, -61);
	SunShadows sun(&data, lightDir);
	
	Tracer* tracer = data.createTracer();
	Vector3 start;
	
	glClearColor(0.1f, 0.5f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glClearStencil(0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	
	glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    Shader* shader = Shader::createShader("assets/vertex.shader", "assets/fragment.shader");
	
	GLuint lightDirUniform = shader->getUniform("lightDir");

    while (!glfwWindowShouldClose(window))
    {
        glViewport(0, 0, _windowSize[0], _windowSize[1]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float matrix[16];
        glhPerspectivef2(matrix, FOV, float(_windowSize[0]) / float(_windowSize[1]), 1.0f, 5000.f);
        glLoadMatrixf(matrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        start = _camera.position;
        float dt = 15.0f;//(Clock.GetElapsedTime() - elapsed) * 500;
        if (_movement[Movement::forward] && !_movement[Movement::backward]) _camera.moveForward(-dt);
        if (!_movement[Movement::forward] && _movement[Movement::backward]) _camera.moveForward(dt);
        if (_movement[Movement::left] && !_movement[Movement::right]) _camera.moveLeft(-dt);
        if (!_movement[Movement::left] && _movement[Movement::right]) _camera.moveLeft(dt);
        data.setCurrentLeaf(_camera.position);
//		if (enableTracing)
//		{
            if (start != _camera.position)
                tracer->trace(start, _camera.position);
//		}

        _camera.update();

        glColor3f(1,1,1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glEnable(GL_DEPTH_TEST);
		
        glActiveTextureARB(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
		
        glActiveTextureARB(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
		
//		data.renderAllFaces();
        shader->setUniformVector(lightDirUniform, lightDir);
        shader->use();
        data.renderVisibleFaces();
        Shader::useNoShader();
/*
        glActiveTextureARB(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glColor4f(0, 0, 0, 0.1f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        sun.render(f);
        glDisable(GL_BLEND);
		
/*/
        glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
        glDisable(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);

        glEnable(GL_STENCIL_TEST);
        glColorMask(0, 0, 0, 0);
        glStencilFunc(GL_ALWAYS, 1, 0xffffffff);

        // first pass, stencil operation decreases stencil value
        glFrontFace(GL_CCW);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        sun.render(f);


        // second pass, stencil operation increases stencil value
        glFrontFace(GL_CW);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
        sun.render(f);


        glFrontFace(GL_CCW);
        glColorMask(1, 1, 1, 1);

        //draw a shadowing rectangle covering the entire screen
        glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glDisable(GL_CULL_FACE);
        glPushMatrix();
        glLoadIdentity();
        glBegin(GL_QUADS);
            glVertex3f(-_windowSize[0], -_windowSize[1], -10);
            glVertex3f(_windowSize[0], -_windowSize[1], -10);
            glVertex3f(_windowSize[0], _windowSize[1], -10);
            glVertex3f(-_windowSize[0], _windowSize[1], -10);
        glEnd();
        glPopMatrix();
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_TRUE);
        glEnable(GL_LIGHTING);
        glDisable(GL_STENCIL_TEST);
        glShadeModel(GL_SMOOTH);
        glPopAttrib();
//*/
		
		
/*
        glActiveTextureARB(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
		
        glActiveTextureARB(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
		
        glColor3f(1,1,1);
        data.renderAllLeafs();
        glColor3f(0,0,1);
        data.renderCurrentLeafWithFaces();
//*/

//*
        glDisable(GL_DEPTH_TEST);
        tracer->result.render();
        glEnable(GL_DEPTH_TEST);

        glDisable(GL_DEPTH_TEST);
        glColor3f(1,1,0);
        if (tracer->result.face >= 0)
            data.renderFace(tracer->result.face);
        glEnable(GL_DEPTH_TEST);
//*/
		
        /// 2D
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();

        glOrtho(0.0, (GLdouble) _windowSize[0], 0.0, (GLdouble) _windowSize[1], -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glActiveTextureARB(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        _camera.position.toString(positionPrint);
        font.drawText(50, 50, positionPrint, ui::RGBA(255, 255, 255));

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        /// END 2D

        glfwSwapBuffers(window);
        glfwPollEvents();
	}

    glfwTerminate();

	return 0;
}

