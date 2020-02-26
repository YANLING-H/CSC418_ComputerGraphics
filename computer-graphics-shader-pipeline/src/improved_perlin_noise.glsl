// Given a 3d position as a seed, compute an even smoother procedural noise
// value. "Improving Noise" [Perlin 2002].
//
// Inputs:
//   st  3D seed
// Values between  -½ and ½ ?
//
// expects: random_direction, improved_smooth_step
float improved_perlin_noise( vec3 st) 
{
  /* 
   * Algorithm based on
   * https://flafla2.github.io/2014/08/09/perlinnoise.html
   */

  vec3 base = floor(st);
  vec3 frac = fract(st);

  // Create unit cube around seed point
  vec3 p_x0y0z0 = base;
  vec3 p_x1y0z0 = vec3(base.x + 1, base.yz);
  vec3 p_x0y1z0 = vec3(base.x, base.y + 1, base.z);
  vec3 p_x1y1z0 = vec3(base.x + 1, base.y + 1, base.z);
  vec3 p_x0y0z1 = vec3(base.xy, base.z + 1);
  vec3 p_x1y0z1 = vec3(base.x + 1, base.y, base.z + 1);
  vec3 p_x0y1z1 = vec3(base.x, base.y + 1, base.z + 1);
  vec3 p_x1y1z1 = vec3(base.x + 1, base.y + 1, base.z + 1);

  // Create pseudo-random gradient at each vertex
  vec3 g_x0y0z0 = random_direction(p_x0y0z0);
  vec3 g_x1y0z0 = random_direction(p_x1y0z0);
  vec3 g_x0y1z0 = random_direction(p_x0y1z0);
  vec3 g_x1y1z0 = random_direction(p_x1y1z0);
  vec3 g_x0y0z1 = random_direction(p_x0y0z1);
  vec3 g_x1y0z1 = random_direction(p_x1y0z1);
  vec3 g_x0y1z1 = random_direction(p_x0y1z1);
  vec3 g_x1y1z1 = random_direction(p_x1y1z1);

  // Distance vectors (Distance from cube points to query point)
  vec3 d_x0y0z0 = frac;
  vec3 d_x1y0z0 = vec3(1 - frac.x, frac.yz);
  vec3 d_x0y1z0 = vec3(frac.x, 1 - frac.y, frac.z);
  vec3 d_x1y1z0 = vec3(1 - frac.x, 1 - frac.y, frac.z);
  vec3 d_x0y0z1 = vec3(frac.xy, 1 - frac.z)
  vec3 d_x1y0z1 = vec3(1 - frac.x, frac.y, 1 - frac.z);
  vec3 d_x0y1z1 = vec3(frac.x, 1 - frac.y, 1 - frac.z);
  vec3 d_x1y1z1 = vec3(1 - frac.x, 1 - frac.y, 1 - frac.z);

  // Influence values at each vertex
  float i_x0y0z0 = dot(g_x0y0z0, d_x0y0z0);
  float i_x1y0z0 = dot(g_x1y0z0, d_x1y0z0);
  float i_x0y1z0 = dot(g_x0y1z0, d_x0y1z0);
  float i_x1y1z0 = dot(g_x1y1z0, d_x1y1z0);
  float i_x0y0z1 = dot(g_x0y0z1, d_x0y0z1);
  float i_x1y0z1 = dot(g_x1y0z1, d_x1y0z1);
  float i_x0y1z1 = dot(g_x0y1z1, d_x0y1z1);
  float i_x1y1z1 = dot(g_x1y1z1, d_x1y1z1);

  // Interpolation
  vec3 iw = improved_smooth_step(frac);  // Smoothed interpolation weights

  float ip_x1, ip_x2, ip_y1, ip_y2, ip_value;
  // Interpolate on first face
  ip_x1 = mix(i_x0y0z0, i_x1y0z0, iw.x);
  ip_x2 = mix(i_x0y1z0, i_x1y1z0, iw.x);
  ip_y1 = mix(ip_x1, ip_x2, iw.y);

  // Interpolate on second face
  ip_x1 = mix(i_x0y0z1, i_x1y0z1, iw.x);
  ip_x2 = mix(i_x0y1z1, i_x1y1z1, iw.x);
  ip_y2 = mix(ip_x1, ip_x2, iw.y);

  // Interpolate between the two face interpolated values
  // to get the final perlin noise value
  ip_value = mix(ip_y1, ip_y2, iw.z);

  // Value is in range [0, 1], so shift it to range [-1, 1]
  return 2 * ip_value - 1;
}

