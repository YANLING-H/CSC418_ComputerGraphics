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


  int floor_x = int(floor(st.x));
  int floor_y = int(floor(st.y));
  int floor_z = int(floor(st.z));
  vec3 stepped = improved_smooth_step(st - vec3(floor_x, floor_y, floor_z));
  float gammas[8];
  int count = 0;
  for (int i = floor_x; i <= floor_x + 1; i++){
    for (int j = floor_y; j <= floor_y + 1; j++){
      for (int k = floor_z; k <= floor_z + 1; k++){
        float u = st.x - float(i);
        float v = st.y - float(j);
        float w = st.z - float(k);
        vec3 rand_vec = random_direction(vec3(i, j, k));
        gammas[count] = dot(rand_vec, vec3(u, v, w));
        count++;
      }
    }
  }
  float temp1 = stepped.x * (gammas[4] - gammas[0]) + gammas[0];
  float temp2 = stepped.x * (gammas[5] - gammas[1]) + gammas[1];
  float temp3 = stepped.x * (gammas[6] - gammas[2]) + gammas[2];
  float temp4 = stepped.x * (gammas[7] - gammas[3]) + gammas[3];

  float temp5 = stepped.y * (temp3 - temp1) + temp1;
  float temp6 = stepped.y * (temp4 - temp2) + temp2;
  return 2 * (stepped.z * (temp6 - temp5) + temp5) / sqrt(3);





  /* 
   * Algorithm based on
   * https://flafla2.github.io/2014/08/09/perlinnoise.html
   */

  vec3 base = floor(st);
  vec3 top = ceil(st);
  vec3 frac = fract(st);

  // Create unit cube around seed point
  vec3 p_x0y0z0 = base;
  vec3 p_x1y0z0 = vec3(top.x, base.yz);
  vec3 p_x0y1z0 = vec3(base.x, top.y, base.z);
  vec3 p_x1y1z0 = vec3(top.x, top.y, base.z);
  vec3 p_x0y0z1 = vec3(base.xy, top.z);
  vec3 p_x1y0z1 = vec3(top.x, base.y, top.z);
  vec3 p_x0y1z1 = vec3(base.x, top.y, top.z);
  vec3 p_x1y1z1 = vec3(top.x, top.y, top.z);

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
  vec3 d_x0y0z0 = normalize(st - p_x0y0z0);
  vec3 d_x1y0z0 = normalize(st - p_x1y0z0);
  vec3 d_x0y1z0 = normalize(st - p_x0y1z0);
  vec3 d_x1y1z0 = normalize(st - p_x1y1z0);
  vec3 d_x0y0z1 = normalize(st - p_x0y0z1);
  vec3 d_x1y0z1 = normalize(st - p_x1y0z1);
  vec3 d_x0y1z1 = normalize(st - p_x0y1z1);
  vec3 d_x1y1z1 = normalize(st - p_x1y1z1);

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

  // Return value adjusted to correct range
  return ip_value / sqrt(3);
}

