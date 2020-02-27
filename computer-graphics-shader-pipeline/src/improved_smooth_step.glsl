// Filter an input value to perform an even smoother step. This function should
// be a quintic polynomial with improved_smooth_step(0) = 0,
// improved_smooth_step(1) = 1, and zero first _and_ second derivatives at f=0
// and f=1. "Improving Noise" [Perlin 2002].
//
// Inputs:
//   f  input value
// Returns filtered output value
float improved_smooth_step( float f)
{
  /* Using the same mathematical steps as in the original smooth_step,
   * the polynomial obtained is 6x^5 - 15x^4 + 10x^3
   */
   if (f < 0.0)
     return 0.0;
   if (f > 1.0)
     return 1.0;
   return 6 * pow(f, 5) + 10 * pow(f, 3) - 15 * pow(f, 4);
}
vec3 improved_smooth_step( vec3 f)
{
  return vec3(improved_smooth_step(f.x), improved_smooth_step(f.y), improved_smooth_step(f.z));
}
