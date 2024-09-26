#pragma once

#include <vector>
#include <cmath>
#include <iostream>

class rk45_dormand_prince {
public:
  rk45_dormand_prince(int num_equations, double tolerance_abs, double tolerance_rel) {
    // constructor, initialize the data members
    n_eq = num_equations;
    atol = tolerance_abs;
    rtol = tolerance_rel;
    y_tmp.resize(n_eq);
    y_err.resize(n_eq);

    k1.resize(n_eq);
    k2.resize(n_eq);
    k3.resize(n_eq);
    k4.resize(n_eq);
    k5.resize(n_eq);
    k6.resize(n_eq);
    k7.resize(n_eq);

    r1.resize(n_eq);
    r2.resize(n_eq);
    r3.resize(n_eq);
    r4.resize(n_eq);
    r5.resize(n_eq);
  }

  template <typename F, typename StopCond>
  std::vector<double> integrate(const F &f, const StopCond &stop, double x0, 
                                const std::vector<double> &y0,
                                bool dense_output = false,
                                const std::vector<double> &x_out = {}) {
    // clear the arrays so that we start fresh every time we call this function
    xs.clear();
    result.clear();

    // initial conditions
    double h = 0.1; // arbitrary initial step size
    double x = x0;
    std::vector<double> y = y0;
    double err = 1.0;
    // err_prev is for you to implement the more advanced step size control
    double err_prev = 1.0;

    // Always push the initial condition into the results
    xs.push_back(x);
    result.push_back(y);

    while (!stop(x, y)) {
      y = step(f, h, x, y);
      err = error(y);
      // If err is fortuitously too small, set it to some lower bound
      err = std::max(err, 1.0e-10);

      // Accept the step if the scalar error is below 1, otherwise reject it and
      // do not move forward
      if (err < 1.0) {
        if (dense_output) {
          // TODO: Carry out dense output here, in two steps. First, compute the
          // coefficients r1, r2, r3, r4, r5. Then, go through the x_out array, 
          // find the x_out values that are between x and x + h, interpolate
          // the corresponding y values using the coefficients. Push
          // the x values to the xs array and y values to the result array.

          // Note that you probably don't want to loop through the whole x_out
          // array every time you compute a step. You can keep track of the index
          // of the last x_out value you used and start from there next time. You
          // can assume that the x_out array is already sorted in ascending order.
          x += h;
        } else {
          x += h;
          // This is what we do when not using dense output
          hs.push_back(h);
          xs.push_back(x);
          result.push_back(y);
        }
      }

      // Adjust h as needed

      // TODO: implement this part where we adjust h. Increase h if err is small
      // and decrease it when err is large. Do not let h go below hmin or above
      // hmax. You can either use the simple step size control or the more
      // advanced one that uses err_prev.

      // Uncomment the following line to see the step size and error at each step
      // std::cout << "x = " << x << ", h = " << h << ", err = " << err << std::endl;
    }
    return y;
  }

  template <typename F>
  std::vector<double> step(const F& f, double h, double x, const std::vector<double> &y) {
    // Compute the next step in y, given x and y of the current step
    std::vector<double> y_next(n_eq);

    // TODO: Compute 5th order y_next using the Dormand-Prince method. You can
    // reuse k7 as the next k1 by storing it in an extra variable, which can
    // save you an evaluation of f(x, y), although this is not strictly
    // necessary.

    // TODO: Compute y_err using the difference between the two estimates of
    // DP45. Note that the coefficients e1, e2, ..., e7 are already defined for
    // you, which are the differences between b_i (coefficients of the 5th order
    // solution) and b_i^* (coefficients of the 4th order solution).

    return y_next;
  }

  double error(const std::vector<double> &y) {
    // TODO: compute a scalar error from the error estimate y_err and return it.
    // The is the err that we will compare to 1.0 to decide whether to accept
    // the step or not.
  }

  int n_eq;
  double atol, rtol;

  // We impose a minimum and maximum step size
  const double hmin = 1.0e-10;
  const double hmax = 1.0;

  // These are temporary variables used to store the coefficients. You are allowed to
  // define additional temporary variables if you need them.
  std::vector<double> k1, k2, k3, k4, k5, k6, k7, y_tmp, y_err;

  // vectors that store the results
  std::vector<double> hs;
  std::vector<double> xs;
  std::vector<std::vector<double>> result;

  // These are temporary variables used to store the coefficients
  // used in dense output
  std::vector<double> r1, r2, r3, r4, r5;

  // c1 is zero, c6 and c7 are 1.0
  const double c2 = 1.0 / 5.0;
  const double c3 = 3.0 / 10.0;
  const double c4 = 4.0 / 5.0;
  const double c5 = 8.0 / 9.0;

  const double a21 = 1.0 / 5.0;
  const double a31 = 3.0 / 40.0;
  const double a32 = 9.0 / 40.0;
  const double a41 = 44.0 / 45.0;
  const double a42 = -56.0 / 15.0;
  const double a43 = 32.0 / 9.0;
  const double a51 = 19372.0 / 6561.0;
  const double a52 = -25360.0 / 2187.0;
  const double a53 = 64448.0 / 6561.0;
  const double a54 = -212.0 / 729.0;
  const double a61 = 9017.0 / 3168.0;
  const double a62 = -355.0 / 33.0;
  const double a63 = 46732.0 / 5247.0;
  const double a64 = 49.0 / 176.0;
  const double a65 = -5103.0 / 18656.0;

  // Note that a71, a72, a73, a74, a75, a76 are essentially the coefficients of the 5th order solution
  const double a71 = 35.0 / 384.0;
  const double a72 = 0.0;
  const double a73 = 500.0 / 1113.0;
  const double a74 = 125.0 / 192.0;
  const double a75 = -2187.0 / 6784.0;
  const double a76 = 11.0 / 84.0;

  // These coefficients are used to estimate the error in the solution. They are essentially 
  // the coefficients of the 5th order solution minus the 4th order solution, i.e. b_i - b_i^*.
  const double e1 = 71.0 / 57600.0;
  const double e2 = 0.0;
  const double e3 = -71.0 / 16695.0;
  const double e4 = 71.0 / 1920.0;
  const double e5 = -17253.0 / 339200.0;
  const double e6 = 22.0 / 525.0;
  const double e7 = -1.0 / 40.0;

  // These are the coefficients for dense output
  const double d1 = -12715105075.0 / 11282082432.0;
  const double d2 = 0.0;
  const double d3 = 87487479700.0 / 32700410799.0;
  const double d4 = -10690763975.0 / 1880347072.0;
  const double d5 = 701980252875.0 / 199316789632.0;
  const double d6 = -1453857185.0 / 822651844.0;
  const double d7 = 69997945.0 / 29380423.0;
};
