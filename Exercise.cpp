//=============================================================================
//  Physically-based Simulation in Computer Graphics
//  ETH Zurich
//=============================================================================

#include "Utilities/Vector2T.h"
#include "Scene.h"

// Gravitational acceleration (9.81 m/s^2)
static const double g = 9.81;
const double x0 = -1.04509634385129060;
const double v0 = -0.82548303829779446;

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
	if (method == Scene::EULER) {
		double Fg      = - m * g;
		double Fspring = k * ((p1 - p2) - L);
		double Fdamp   = - d * v2;
		double F = Fg + Fspring + Fdamp;

		p2 += dt * v2;
		v2 += dt * F / m;
	}
	else if (method == Scene::ANALYTIC) {
		double tmp = d * d - 4 * k * m;
		double div = 2 * m;
		double b   = d / div;
		double c   = -g * m / k - L + p1;

		if (tmp < 0) {
			double a  = sqrt(-tmp) / div;
			double beta1 = x0 - c;
			double beta2 = -(v0 + beta1 * b)/a;
			double teta1 = v0;
			double teta2 = b * beta2 - a * beta1;

			p2 = exp(-b * dt) * (beta1 * cos(a * dt) + beta2 * sin(a * dt)) + c;
			v2 = exp(-b * dt) * (teta1 * cos(a * dt) + teta2 * sin(a * dt));
		}
		else {
			double a = sqrt(tmp) / div;
			double a2 = v0 + (a + b) * (x0 - c);
			double a1 = x0 - c - a2;

			p2 = exp(-b * dt) * ( a1 *           exp(-a * dt) + a2 *           exp(a * dt)) + c;
			v2 = exp(-b * dt) * (-a1 * (a + b) * exp(-a * dt) + a2 * (a - b) * exp(a * dt));
		}
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
