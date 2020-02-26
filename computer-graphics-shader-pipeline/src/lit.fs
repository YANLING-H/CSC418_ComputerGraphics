// Add (hard code) an orbiting (point or directional) light to the scene. Light
// the scene using the Blinn-Phong Lighting Model.
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
// expects: PI, blinn_phong
void main()
{
  float light_dist = 4.0;
  float light_orbit_period = 8.0;

  vec4 light_pos = vec3(light_dist, light_dist, light_dist, 1.0);
  vec4 light_transform = rotate_about_y(-mod(animation_seconds, light_orbit_period) * 2 * PI / light_orbit_period);
  vec4 light = light_transform * light_pos;  

  vec3 ka, kd, ks;
  vec3 n, v, l;
  float p;

  if (is_moon) {
    color = vec3(0.2, 0.2, 0.2);
    ka = color * 0.1;
    kd = color * 0.6;
    ks = color * 0.2;
    p = 100;
  } else {
    color = vec3(0.0, 0.0, 1.0);
    ka = color * 0.1;
    kd = color * 0.6;
    ks = color * 0.8;
    p = 1000;
  }

  n = normalize(normal_fs_in);
  v = -normalize(view_pos_fs_in.xyz);
  l = normalize(light.xyz - view_pos_fs_in.xyz);

  color = blinn_phong(ka, kd, ks, p, n, v, l);

}
