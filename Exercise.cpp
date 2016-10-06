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
        // calculate new velocity
        v2 += dt * F / m;
        // calculate new location
        p2 += dt * v2;
	}
	else if (method == Scene::MIDPOINT) {
		//first get to t + 1/2 and compute f(t_n+1/2,y_n+1/2) then increment p2 and v2 with that

		//compute v at t + h/2
		double a1 = F / m;
		double k2 = v2 + dt / 2 * a1;

		//compute a at t + h/2 with x and v at t + h/2 ... so (p2 + v2 * dt /2) and k2
		F = k * (p1 - (p2 + v2 * dt / 2) - L) + Fg;
		double a2 = ( F - d * k2 ) / m;

		//calculate new location
		p2 += dt * k2;
		//calculate new velocity
		v2 += dt * a2;

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
    p1 += Vec2(1,1);
    p2 += Vec2(1,1);
    p3 += Vec2(1,1);
    
    // mapping Vec2-type parameters to Vector2T-type for ease of calculations
    Vector2T<Vec2> p1_local = p1;
    Vector2T<Vec2> p2_local = p2;
    Vector2T<Vec2> p3_local = p3;
    Vector2T<Vec2> v1_local = v1;
    Vector2T<Vec2> v2_local = v2;
    Vector2T<Vec2> v3_local = v3;

    // Calculate current forces
    Vector2T<double> F_weight;
    F_weight.x() = -m * g;
    F_weight.y() = -m * g;
    
    Vector2T<double> F_spring_p1p2 =  k * ((p1 - p2) - L);
    Vector2T<double> F_spring_p1p3 =  k * ((p1 - p2) - L);
    Vector2T<double> F_spring_p2p3 =  k * ((p1 - p2) - L);
    
    Vector2T<double> F_damp_p1p2 = -d * v2;
    Vector2T<double> F_damp_p1p3 = -d * v2;
    Vector2T<double> F_damp_p2p3 = -d * v2;
    
    Vector2T<double> F_net_p1p2 = F_weight + F_spring_p1p2 + F_damp_p1p2;
    Vector2T<double> F_net_p1p3 = F_weight + F_spring_p1p3 + F_damp_p1p3;
    Vector2T<double> F_net_p2p3 = F_weight + F_spring_p2p3 + F_damp_p2p3;
}
