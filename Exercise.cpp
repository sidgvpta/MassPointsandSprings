//=============================================================================
//  Physically-based Simulation in Computer Graphics
//  ETH Zurich
//=============================================================================

#include "Utilities/Vector2T.h"
#include "Scene.h"

// Gravitational acceleration (9.81 m/s^2)
static const double g = 9.81;

#define PRINT_VALUES 1

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
	const static double x0 = p2;
	const static double v0 = v2;
#ifdef PRINT_VALUES
    static int entry = 0;
    const static double t0 = -dt;
    static double t = -dt;
    t = (method == Scene::ANALYTIC) ? t0 + dt : t + dt;
	static char filename[17];
#ifdef WIN32
	static FILE *file;
	const static int err1 = sprintf(filename, "exercise1_m%d.txt", method);
    const static int err2 = fopen_s(&file, filename, "w");
    fprintf_s(file, "%f\t%f\t%f\n", t, p2, v2);
#elif defined(__MACH__)
    static FILE *file = fopen(filename, "w");
	printf("entry:%5d\n", entry);
#endif
	if (t >= 40.0) {
		fclose(file);
		exit(0);
	}
#endif

	// Remark: The parameter 'dt' is the duration of the time step, unless the analytic 
	//         solution is requested, in which case it is the absolute time.
	
	// Calculate current forces
	double Fg		= -m * g;
	double Fspring	=  k * ((p1 - p2) - L);
	double Fdamp	= -d * v2;
	double F = Fg + Fspring + Fdamp;

	if (method == Scene::EULER) {
		// calculate new location
		p2 += dt * v2;
		// calculate new velocity
		v2 += dt * F / m;
	}
	else if (method == Scene::LEAP_FROG) {
		// calculate new location
		p2 += v2 * dt + 0.5 * F / m * dt * dt;
		// forces at next point
		double Fspring_next = k * ((p1 - p2) - L);
		double Fdamp_next = -d * v2;
		double F_next = Fg + Fspring_next + Fdamp_next;
		// calculate new velocity
		v2 += 0.5 * ((F + F_next) / m) * dt;
	}
	else if (method == Scene::MIDPOINT) {
		// velocity at next half point
		double v2_half = v2 + dt * F / (2.0 * m);
		// location of next half point
		double p2_half = p2 + dt * v2_half / 2.0;
		// forces at half point
		double Fspring_half = k * ((p1 - p2_half) - L);
		double Fdamp_half = -d * v2_half;
		// location of next point
		p2 += dt * v2_half;
		// velocity of next point
		v2 += dt * (Fg + Fspring_half + Fdamp_half) / m;
	}
	else if (method == Scene::BACK_EULER) {
		// calculate new velocity
		v2 += dt * F / m;
		// calculate location with new velocity
		p2 += dt * v2;
	}
	else if (method == Scene::ANALYTIC) {
		double tmp = d * d - 4 * k * m;
		double div = 2 * m;
		double b   = d / div;
		double c   = -g * m / k - L + p1;
		// normal case:
		if (tmp < 0) {
			// frequency for sin(..) and cos(..)
			double a  = sqrt(-tmp) / div;
			// constants for x(t)
			double beta1 = x0 - c;
			double beta2 = -(v0 + beta1 * b)/a;
			// constants for v(t)
			double teta1 = v0;
			double teta2 = b * beta2 - a * beta1;
			// calculate new location and velocity
			p2 = exp(-b * dt) * (beta1 * cos(a * dt) + beta2 * sin(a * dt)) + c;
			v2 = exp(-b * dt) * (teta1 * cos(a * dt) + teta2 * sin(a * dt));
		}
		// (maybe) divergent case:
		else {
			// exponent for exponential function
			double a = sqrt(tmp) / div;
			// constants for exponentials
			double a2 = v0 + (a + b) * (x0 - c);
			double a1 = x0 - c - a2;
			// calculate new location and velocity
			p2 = exp(-b * dt) * ( a1 *           exp(-a * dt) + a2 *           exp(a * dt)) + c;
			v2 = exp(-b * dt) * (-a1 * (a + b) * exp(-a * dt) + a2 * (a - b) * exp(a * dt));
		}
	}
	else {
		throw std::invalid_argument("Method chosen is invalid");
	}
}

// Exercise 3
// Falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
                      Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
	const static int bla = system("pause");
	// Gravity Force is constant
	const Vec2 Fg(0, -m * g);
	// current position value copies:
	const Vec2 pos[] = { p1, p2, p3 };
	const Vec2 vel[] = { v1, v2, v3 };

	// Compute force at each point:
	for (int i = 0; i < 3; i++) {
		Vec2 *a, b, c, *v;
		double penetration;
		Vec2 penalty = 0.0;
		// Select correct vertices
		if (i == 0) {
			a = &p1;
			b = p2;
			c = p3;
			v = &v1;
		}
		else if (i == 1) {
			a = &p2;
			b = pos[0];
			c = p3;
			v = &v2;
		}
		else {
			a = &p3;
			b = pos[0];
			c = pos[1];
			v = &v3;
		}
		// Compute Forces at given point
		Vec2 ab = (b - *a);
		Vec2 ac = (c - *a);
		double len_ab = ab.length();
		double len_ac = ac.length();
		Vec2 Fs_ab = k * (len_ab - L) * ab / len_ab;
		Vec2 Fs_ac = k * (len_ac - L) * ac / len_ac;
		Vec2 Fdamp = -d * *v;
		Vec2 F = Fg + Fs_ab + Fs_ac + Fdamp;
		// Apply penalty if needed
		if ((penetration = a->y()) <= -1) {
			const double bigK = 100;
			penalty = Vec2(0.0, -bigK * (penetration + 1));
			F += penalty;
		}
		// get added up Forces
//		printf("F%d: (%f,%f)\n", i, F.x(), F.y());
//		printf("- Fg =\t\t(%f,%f)\n- Fs_ab =\t(%f,%f)\n- Fs_ac =\t(%f,%f)\n- Fd =\t\t(%f,%f)\n- P =\t\t(%f,%f)\n",
//			Fg, Fs_ab, Fs_ac, Fdamp, penalty);

		// Compute new Location with method BACK_EULER
		*v += dt * F / m;
		*a += dt * *v;
//		printf("v: (%f,%f) -> (%f,%f)\n", vel[i], *v);
//		printf("x: (%f,%f) -> (%f,%f)\n", pos[i], *a);
	}
}
