//=============================================================================
//  Physically-based Simulation in Computer Graphics
//  ETH Zurich
//=============================================================================

#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

#include "Scene.h"

// timers 
#if defined(__APPLE__)
#include <CoreServices/CoreServices.h>
#elif defined(_WIN32)
#include <Windows.h>
#else
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#endif

Scene *sc = nullptr;
unsigned int lastTick = 0;
unsigned int stepping = 10000;
double mspt;

// windows only
void timerInit()
{
#if defined(__linux__)
	// nothing to do...
#elif defined(_WIN32)
	LARGE_INTEGER tps_long;
	QueryPerformanceFrequency(&tps_long);
	mspt = 1e6 / (double)tps_long.QuadPart;
#endif
}

unsigned int getTime()
{
#if defined(__APPLE__) // OS X
	UnsignedWide uCur;
	Microseconds(&uCur);
	return uCur.lo;
#elif defined(__linux__) // Linux
	struct timeval tv;
	struct timezone tz;
	tz.tz_minuteswest = 0;
	tz.tz_dsttime = 0;
	gettimeofday(&tv, &tz);
	return (unsigned int)((tv.tv_sec * 1000) + (tv.tv_usec / 1000)) * 10000;
#else // Windows
	LARGE_INTEGER tick;
	QueryPerformanceCounter(&tick);
	return (unsigned int)(mspt*tick.QuadPart);
#endif
}

void display(void)
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	unsigned int tm = getTime();
	if (tm - lastTick > stepping)
	{
		lastTick += stepping;
		sc->Update();
	}
	sc->Render();

	// Draw everything to the screen
	glFlush();
	// Start drawing again
	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	sc = new Scene(argc, argv);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Exercise 1");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	timerInit();
	lastTick = getTime();

	glEnable(GL_LINE_SMOOTH);
	glutMainLoop();
	delete sc;

	return EXIT_SUCCESS;
}
