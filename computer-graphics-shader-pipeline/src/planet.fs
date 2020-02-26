// Generate a procedural planet and orbiting moon. Use layers of (improved)
// Perlin noise to generate planetary features such as vegetation, gaseous
// clouds, mountains, valleys, ice caps, rivers, oceans. Don't forget about the
// moon. Use `animation_seconds` in your noise input to create (periodic)
// temporal effects.
//
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
// Copy from bump.fs
  float light_dist = 4.0;
  float light_orbit_period = 8.0;

  vec4 light_pos = vec3(light_dist, light_dist, light_dist, 1.0);
  vec4 light_transform = rotate_about_y(-mod(animation_seconds, light_orbit_period) * 2 * PI / light_orbit_period);
  vec4 light = light_transform * light_pos;  

  vec3 ka, kd, ks;
  vec3 n, v, l;
  float p;

  vec3 bump_pos = bump_position(is_moon, normal_fs_in);
  float bump_h = bump_height(is_moon, normal_fs_in);

  // Generate noise
  float noise1 = 0.5 * improved_perlin_noise(sphere_fs_in);
  float noise2 = improved_perlin_noise(vec3(normal_fs_in.x, sphere_fs_in.y * cos(normal_fs_in.y), sin(sphere_fs_in.z)));
  float noise3 = 0.7 * improved_perlin_noise(7 * random_direction(vec3(sphere_fs_in.x + normal_fs_in.y, sin(normal_fs_in.y) + sphere_fs_in.z, random_direction(normal_fs_in + sphere_fs_in).z)));
  float noise4 = 0.2 * improved_perlin_noise(19 * noise3 * random_direction(light.xyz));
  float noise5 = 0.3 * improved_perlin_noise(noise2 * noise1 * random_direction(sphere_fs_in-normal_fs_in));
  float noise6 = 3 * bump_h + improved_perlin_noise(bump_pos);

  if (is_moon) {
    color = vec3(0.25 + noise1, 0.2 + noise4, 0.2 + noise5);
    ka = color * (0.1 + noise4);
    kd = color * (0.5 + noise2);
    ks = color * (0.2 + noise5 / 2.0);
    p = 100;
  } else {
    color = vec3(0.1 + noise5 * noise6, 0.25 + noise6 * 2, 0.6 + noise3 - noise6);
    ka = color * (0.1 + noise4);
    kd = color * (0.6 + noise5);
    ks = color * (0.7 + noise1);
    p = 1000;
  }

  vec3 T, B;
  tangent(normalize(normal_fs_in), T, B);
  float eps = 0.0001;

  vec3 v1 = (bump_pos * (normal_fs_in + eps * T) - bump_pos * normal_fs_in) / eps;
  vec3 v2 = (bump_pos * (normal_fs_in + eps * B) - bump_pos * normal_fs_in) / eps;
  n = normalize(cross(v1, v2));

  v = -normalize(view_pos_fs_in.xyz);
  l = normalize(light.xyz - view_pos_fs_in.xyz);

  color = blinn_phong(ka, kd, ks, p, n, v, l);
}
