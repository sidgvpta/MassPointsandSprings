//=============================================================================
//  Physically-based Simulation in Computer Graphics
//  ETH Zurich
//=============================================================================

#include "Utilities/Vector2T.h"
#include "Scene.h"

// Gravitational acceleration (9.81 m/s^2)
static const double g = 9.81;

// Exercise 1
// Hanging mass point
/** @param: k		stiffness
  * @param: m		mass
  * @param: d		damping
  * @param: L		initial length
  * @param: dt		timestep/total time
  * @param: method	method used
  * @param: p1		position (fixed)
  * @param: v1		velocity (fixed)
  * @param: p2		position (relaxed)
  * @param: v2		velocity (relaxed)
  */
void AdvanceTimeStep1(double k, double m, double d, double L, double dt, int method, double p1, double v1, double& p2, double& v2)
{
	const double g = 9.81;
	
	if (method == Scene::EULER) {
		double l = abs(p2 - p1);
		double Fg = m * g;
		double Fspring = - k * (l - L) * ((p1 < p2) ? +1 : -1);
		double Fdamp = -d * v2;
		double F = Fg + Fspring;

		p2 += dt * v2;
		v2 += dt * (F + Fdamp) / m;
	}
	else if (method == Scene::ANALYTIC) {
		double a = (-sqrt(d*d - 4 * k*m) - d) / (2 * m);
		double b = ( sqrt(d*d - 4 * k*m) - d) / (2 * m);
		double c = g * m / k + L + p1;
		const double x0 = -1.04509634385129060;
		const double v0 = -0.82548303829779446;
		double a2 = (v0 - a * (x0 - c)) / (b - a);
		double a1 = (x0 - c - a2);

		p2 = a1 * exp(a * dt)	  + a2 * exp(b * dt)	  + c;
		v2 = a1 * exp(a * dt) * a + a2 * exp(b * dt) * b;
		printf("x: %.3f, v: %.3f\n", p2, v2);
	}

	else {
		// Remark: The parameter 'dt' is the duration of the time step, unless the analytic 
		//         solution is requested, in which case it is the absolute time.
		switch (method) {
		case Scene::LEAP_FROG:
			break;
		case Scene::MIDPOINT:
			break;
		case Scene::BACK_EULER:
			break;
		default:
			throw std::invalid_argument("Method chosen is invalid");
			break;
		}
	}
}

// Exercise 3
// Falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
                      Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
	p1 += Vec2(1,1);
}
