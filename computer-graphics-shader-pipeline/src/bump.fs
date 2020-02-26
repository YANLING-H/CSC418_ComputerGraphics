// Set the pixel color using Blinn-Phong shading (e.g., with constant blue and
// gray material color) with a bumpy texture.
// 
// Uniforms:
uniform mat4 view;
uniform mat4 proj;
uniform float animation_seconds;
uniform bool is_moon;
// Inputs:
//                     linearly interpolated from tessellation evaluation shader
//                     output
in vec3 sphere_fs_in;
in vec3 normal_fs_in;
in vec4 pos_fs_in; 
in vec4 view_pos_fs_in; 
// Outputs:
//               rgb color of this pixel
out vec3 color;
// expects: model, blinn_phong, bump_height, bump_position,
// improved_perlin_noise, tangent
void main()
{
  // Copy from procedural_color.fs
  float light_dist = 4.0;
  float light_orbit_period = 8.0;

  vec4 light_pos = vec3(light_dist, light_dist, light_dist, 1.0);
  vec4 light_transform = rotate_about_y(-mod(animation_seconds, light_orbit_period) * 2 * PI / light_orbit_period);
  vec4 light = light_transform * light_pos;  

  vec3 ka, kd, ks;
  vec3 n, v, l;
  float p;

  // Generate noise
  float noise1 = 0.5 * improved_perlin_noise(sphere_fs_in);
  float noise2 = improved_perlin_noise(vec3(normal_fs_in.x, sphere_fs_in.y * cos(normal_fs_in.y), sin(sphere_fs_in.z)));
  float noise3 = 0.7 * improved_perlin_noise(7 * random_direction(vec3(sphere_fs_in.x + normal_fs_in.y, sin(normal_fs_in.y) + sphere_fs_in.z, random_direction(normal_fs_in + sphere_fs_in).z)));
  float noise4 = 0.2 * improved_perlin_noise(19 * noise3 * random_direction(light.xyz));
  float noise5 = 0.3 * improved_perlin_noise(noise2 * noise1 * random_direction(sphere_fs_in-normal_fs_in));

  if (is_moon) {
    color = vec3(0.25 + noise1, 0.2 + noise4, 0.2 + noise5);
    ka = color * (0.1 + noise4);
    kd = color * (0.5 + noise2);
    ks = color * (0.2 + noise5 / 2.0);
    p = 100;
  } else {
    color = vec3(0.1 + noise4, 0.25 + noise1, 0.6 + noise3);
    ka = color * (0.1 + noise4);
    kd = color * (0.6 + noise5);
    ks = color * (0.7 + noise1);
    p = 1000;
  }

  vec3 T, B;
  tangent(normalize(normal_fs_in), T, B);
  vec3 bump_pos = bump_position(is_moon, normal_fs_in);
  float eps = 0.0001;

  vec3 v1 = (bump_pos * (normal_fs_in + eps * T) - bump_pos * normal_fs_in) / eps;
  vec3 v2 = (bump_pos * (normal_fs_in + eps * B) - bump_pos * normal_fs_in) / eps;
  n = normalize(cross(v1, v2));

  v = -normalize(view_pos_fs_in.xyz);
  l = normalize(light.xyz - view_pos_fs_in.xyz);

  color = blinn_phong(ka, kd, ks, p, n, v, l);
}
