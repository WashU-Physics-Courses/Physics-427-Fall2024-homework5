# Physics 427 Homework 5

__Due 11:59pm Wednesday 10/2/2023__

In this homework, the main goal is to put together a general-purpose ODE integrator for use in Project 1. We will implement the RK5(4) Dormand-Prince method, dense output, and custom stopping conditions. The end result should be a library that is somewhat equivalent to the default method in `scipy`'s `solve_ivp`, but more flexible since you have direct access to all the internal components.

## 1. Adaptive RK5(4) Dormand-Prince Method (20 points)

In this problem, we implement the adaptive 5-th order Dorman-Prince method we discussed in class this week. A skeleton code has been provided in `problem1.h` in the homework repository to get you started. Note that in the constructor `rk45_dormand_prince`, in addition to the number of equations, you need to specify the absolute and relative tolerances. All of the coefficients you need have already been declared in the class. Note that in place of the coefficients $b_i$ and $b_i^*$, I included a series of coefficients $e_i$, which are essentially the difference $e_i = b_i - b_i^*$. It should somewhat simplify your error calculations.

Your task is to implement the sections marked "TODO", where the comment should give you some idea how to proceed. If necessary, feel free to declare additional temporary variables or change code outside of the "TODO" blocks, as they are only for orienting you to the sections that are incomplete. Commit your updated `problem1.h` header file to the homework repository. 

## 2. Testing Your Code (15 points)

Write a C++ program that uses the Dormand-Prince solver that you implemented in Problem 1 to solve the following ODE:

$$
\frac{dy}{dx} = e^{-x^2/a^2}\frac{a^2 - 2x^2}{a^2}\omega \cos\left(e^{-x^2/a^2}\omega x\right)
$$

with $a = 4.0$, $\omega = 10.0$. Start with the initial condition that $x_0 = -20.0$ and $y(x_0) = 0.0$, integrate until $x_\mathrm{end} = 20.0$. Plot your solution in Python and commit the plot as `problem2a.png`. Also plot the evolution of the step size in Python, and commit the plot as `problem2b.png`. If you used the skeleton file given in Problem 1, the step sizes are saved in the vector `hs`, similar to the vectors `xs` and `result`.

Commit your C++ file as `problem2.cpp` and your Python plotting script as `problem2.py`. Remember not to commit any `csv` output files.

## 3. Dense Output for Dormand-Prince (10 points)

Implement dense output for the Dormand-Prince method you implemented in Problem 1. Again, I've provided a skeleton file, `problem3.h`. This is basically the same skeleton as `problem1.h` except with additional sections that are responsible for dense output. For the TODO sections that carry over from Problem 1, you can simply copy-paste your code over.

You will notice that the `integrate` method now takes two additional parameters, `dense_output` and `x_out`. After the integration is complete, I expect that if `dense_output` is turned on, the `xs` array in the class is filled with values exactly equal to the given `x_out` array (not the naturally found $x$ values from variable step sizes), and the `result` array populated with the corresponding $y$ values.

Commit your fully implemented `problem3.h` to the homework repository. You can modify your C++ program in Problem 2 to check whether your dense output works, but remember that we specifically want to see the variable steps in Problem 2, so don't use dense output for the plots in Problem 2. We will be testing your implementation by checking whether it compiles correctly, and whether it correctly reproduces our custom test case.

## 4. Custom Stopping Conditions (5 points)

Finally, we would like to have more general stopping conditions than a simple $x_\mathrm{end}$. This is especially relevant for Project 1, where you will need to terminate your integration using a combination of 3 custom stopping conditions when carrying out raytracing.

Modify your `problem3.h` to accommodate a custom stopping condition. This can be achieved as follows: instead of accepting an `x_end` parameter, you accept a general function `stop` in the same way that you accept the right hand side function `f`. You can assume the function has the same signature as `f`. The `stop` function should simply return a `true` or `false` value, depending on whether the integration should step. For example, the following `stop` function would be equivalent to an `x_end`:

``` c++
auto stop = [x_end] (double x, const std::vector<double>& y) {
    return x > x_end;
}
```

Such a general stopping condition can accommodate various scenarios such as one of the `y` values becoming smaller than some threshold, etc. Commit your edited code as `problem4.h`. At this point, the full Dormand-Prince solver is basically complete. You can rename `problem4.h` to `rk45_dormand_prince.h` and use it in your Project 1.
