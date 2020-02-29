// Create a bumpy surface by using procedural noise to generate a height (
// displacement in normal direction).
//
// Inputs:
//   is_moon  whether we're looking at the moon or centre planet
//   s  3D position of seed for noise generation
// Returns elevation adjust along normal (values between -0.1 and 0.1 are
//   reasonable.
float bump_height( bool is_moon, vec3 s)
{
  vec3 object_factor; // Differentiate between earth and moon to give them different appearances
  if (is_moon)
    object_factor = vec3(9.2, 9.5, -10.1);  // Moon is bumpier than earth
  else
    object_factor = vec3(1.2, -1.8, 0.9);
  float height = improved_perlin_noise(vec3(improved_perlin_noise(object_factor * s)));

  float height_factor;
  if (is_moon)
    height_factor = 0.8;
  else
    height_factor = 0.3;
  return height_factor * smooth_heaviside(height, length(object_factor));
}
