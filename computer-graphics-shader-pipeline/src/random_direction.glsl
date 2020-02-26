// Generate a pseudorandom unit 3D vector
// 
// Inputs:
//   seed  3D seed
// Returns psuedorandom, unit 3D vector drawn from uniform distribution over
// the unit sphere (assuming random2 is uniform over [0,1]²).
//
// expects: random2.glsl, PI.glsl
vec3 random_direction( vec3 seed)
{
  vec2 rand_x = rand2(seed);
  vec2 rand_y = rand2(dot(vec3(rand_x, 1.0), seed) * seed);
  vec2 rand_z = rand2(dot(rand_x, rand_y) * seed);

  return normalize(vec3(rand_x.x, rand_y.y, rand_z.z));
}
