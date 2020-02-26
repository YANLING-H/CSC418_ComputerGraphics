// Filter an input value to perform a smooth step. This function should be a
// cubic polynomial with smooth_step(0) = 0, smooth_step(1) = 1, and zero first
// derivatives at f=0 and f=1. 
//
// Inputs:
//   f  input value
// Returns filtered output value
float smooth_step( float f)
{
  float a, b, c, d;
  /* Cubic polynomial => f(x) = ax^3 + bx^2 + cx + d
   *                  => f'(x) = 3ax^2 + 2bx + c
   *
   * f(0) = 0   =>  d = 0
   * f'(0) = 0  =>  c = 0
   * f(1) = 1   =>  a + b = 1
   * f'(1) = 0  =>  3a + 2b = 0
   *
   * Therefore, a = -2, b = 3, c = 0, d = 0
   * Polynomial is -2x^3 + 3x^2
   */
   a = -2;
   b = 3;
   c = 0;
   d = 0;
   return a * pow(f, 3) + b * pow(f, 2) + c * f + d;
}

vec3 smooth_step( vec3 f)
{
  return vec3(smooth_step(f.x), smooth_step(f.y), smooth_step(f.z));
}
