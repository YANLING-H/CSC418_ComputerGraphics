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
  float light_dist = 6.0;
  float light_orbit_period = 8.0;
  float theta = -mod(animation_seconds, light_orbit_period) * 2 * M_PI / light_orbit_period;

  vec4 light_pos = vec4(light_dist, light_dist, light_dist, 1.0);
  mat4 light_transform = mat4(
  cos(theta), 0, -sin(theta), 0,
           0, 1,           0, 0,
  sin(theta), 0,  cos(theta), 0,
           0, 0,           0, 1);
  vec4 light = view * light_transform * light_pos;

  vec3 ka, ks, kd;
  float p;
  vec3 n, v, l;

  // Generate Noise

  // Two random vectors to influence seed in perlin noise
  vec3 v1 = vec3(2.0, 15.0, 10.0);
  vec3 v2 = vec3(22.0, 18.0, 1.5);


  float object_factor;  // Some value to differentiate earth and moon so they dont have the exact patterns
  if (is_moon)
    object_factor = 1.4;
  else
    object_factor = 0.9;

  float noise1 = improved_perlin_noise(v1 * object_factor * sphere_fs_in) + 0.1 * improved_perlin_noise(v2 * object_factor *  sphere_fs_in);
  float noise2 = improved_perlin_noise(v1 * object_factor *  sphere_fs_in) + 0.2 * improved_perlin_noise(v2 * object_factor *  sphere_fs_in);
  float noise3 = improved_perlin_noise(v1 * object_factor *  sphere_fs_in) + 0.3 * improved_perlin_noise(v2 * object_factor *  sphere_fs_in);

  float noise = object_factor * improved_perlin_noise(object_factor * vec3(noise1, noise2, noise3));
  float c_noise = improved_perlin_noise((1.0 + object_factor) * sphere_fs_in);

  float bh = 0.5 * bump_height(is_moon, sphere_fs_in);
  float shore = 0.02;
  float boundary = 0.0312;
  float valley = 0.038;
  float max = 0.07;

  if (is_moon) {
    ka = vec3(0.12, 0.1, 0.1);
    ks = vec3(0.0, 0.0, 0.0);
    kd = vec3(0.425, 0.4, 0.4);
    p = 1000;
  } else{
    if (bh < shore) {
      // Ocean/River
      float snow_threshold = cos(noise + M_PI / 4);
      float angle;
      if (sphere_fs_in.y > 0)
        angle = dot(normalize(sphere_fs_in), vec3(0, 1, 0));
      else
        angle = dot(normalize(sphere_fs_in), vec3(0, -1, 0));
      if (angle > snow_threshold) {
        ka = vec3(0.5, 0.5, 0.5);
        ks = vec3(0.8, 0.8, 0.8);
        kd = 10.0 * vec3(mix(0.5, 1.0, improved_smooth_step((bh - 0.0)/(shore - valley))), mix(0.5, 1.0, improved_smooth_step((bh - 0.0)/(shore - valley))), mix(0.5, 1.0, improved_smooth_step((bh - 0.0)/(shore - valley))));
        p = 300;
      } else {
      ka = vec3(0.1, 0.1, 0.2);
      kd = vec3(0.2, 0.2, 0.8);
      ks = vec3(0.4, 0.4, 0.8);
      p = 300;
      }
    } else if (bh < boundary) {
      // Shoreline
      ka = vec3(0.1, 0.1, 0.9);
      kd = vec3(mix(0.3, 0.6, improved_smooth_step((bh - shore)/(valley - shore))), mix(0.3, 0.6, improved_smooth_step((bh - shore)/(valley - shore))), 1.0);
      ks = vec3(0.7, 0.7, 1.0);
      p = 1000;
    } else if (bh < valley) {
      // Valley-shore boundary
      float snow_threshold = cos(noise + M_PI / 4);
      float angle;
      if (sphere_fs_in.y > 0)
        angle = dot(normalize(sphere_fs_in), vec3(0, 1, 0));
      else
        angle = dot(normalize(sphere_fs_in), vec3(0, -1, 0));
      if (angle > snow_threshold) {
        ka = vec3(0.1, 0.1, 0.1);
        ks = vec3(0.8, 0.8, 0.8);
        kd = 10.0 * vec3(mix(0.0, 1.0, improved_smooth_step((bh - valley)/(max - valley))), mix(0.0, 1.0, improved_smooth_step((bh - valley)/(max - valley))), mix(0.2, 1.0, improved_smooth_step((bh - valley)/(max - valley))));
        p = 100;
      } else {
        ka = vec3(0.1, 0.9, 0.1);
        kd = vec3(0.0, 0.4, 0.0);
        ks = vec3(0.0, 0.2, 0.0);
        p = 1000;
      }
    } else {
      // Land
      float snow_threshold = cos(noise + M_PI / 4);
      float angle;
      if (sphere_fs_in.y > 0)
        angle = dot(normalize(sphere_fs_in), vec3(0, 1, 0));
      else
        angle = dot(normalize(sphere_fs_in), vec3(0, -1, 0));
      if (angle > snow_threshold) {
        ka = vec3(0.1, 0.1, 0.1);
        ks = vec3(0.8, 0.8, 0.8);
        kd = 10.0 * vec3(mix(0.0, 1.0, improved_smooth_step((bh - valley)/(max - valley))), mix(0.0, 1.0, improved_smooth_step((bh - valley)/(max - valley))), mix(0.2, 1.0, improved_smooth_step((bh - valley)/(max - valley))));
        p = 100;
      } else {
        ka = vec3(0.2, 0.2, 0.1);
        kd = vec3(mix(0.1, 1.0, improved_smooth_step((bh - valley)/(max - valley))), mix(0.45, 0.6, improved_smooth_step((bh - valley)/(max - valley))), 0.1);
        //kd = vec3(mix(0.0, 1.0, sin(22.5 * bh)), mix(0.0, 1.0, cos(25.0 * bh)), 0.1);
        ks = vec3(0.2, 0.2, 0.1);
        p = 1000;
      }
    }
  }

  // Apply noise to values
  float noise_factor = 1.8 + object_factor * noise;
  if (noise_factor != 0) {
    kd *= noise_factor;
    ka *= noise_factor;
  }

  // Apply Bumping
  vec3 T, B;
  tangent(sphere_fs_in, T, B);
  vec3 bv1, bv2; // Bump vectors
  vec3 bp = bump_position(is_moon, sphere_fs_in); // Bump position of point
  float eps = 0.0001;
  bv1 = (bump_position(is_moon, sphere_fs_in + eps * T) - bp) / eps;
  bv2 = (bump_position(is_moon, sphere_fs_in + eps * B) - bp) / eps;

  n = normalize(cross(bv1, bv2));

  // If angle deviated greater than 90 degrees, then cross product was
  // conducted in the opposite direction, so fix it
  if (dot(n, sphere_fs_in) < 0)
    n *= -1;

  // Reapply normal transform
  // From snap_to_sphere
  mat4 model_view_transform = view * model(is_moon, animation_seconds);
  mat4 normal_transform = transpose(inverse(model_view_transform));

  n = normalize((normal_transform * vec4(n, 1.0)).xyz);
  v = -normalize(view_pos_fs_in.xyz);
  l = normalize(light.xyz - view_pos_fs_in.xyz);

  color = blinn_phong(ka, kd, ks, p, n, v, l);
  // Apply noise to color output
  color *= vec3(1.0 - c_noise);

  // Generate Clouds
  if (!is_moon){
    vec3 seed = 0.1 * vec3(10, 80, 10);
    float cloud_density = 1.5;
    float cloud_anim_freq = 0.2;
    float cloud_anim_amplitude = 0.75;
    float cloud_anim_val = cloud_anim_amplitude * sin(cloud_anim_freq * M_PI * animation_seconds);
    float cloud_rotation_period = 6.0;
    float cloud_threshold = 0.02;
    float clouds = 0.2 * improved_perlin_noise((cloud_density + cloud_anim_val) * seed * (rotate_about_y(mod(animation_seconds, cloud_rotation_period) * 2 * M_PI / cloud_rotation_period) * vec4(sphere_fs_in, 1)).xyz);
    if (clouds > cloud_threshold){
      ka = vec3(1.0) * clouds;
      kd = vec3(1.0) * clouds;
      ks = vec3(0.0);
      n = normalize(normal_fs_in);
      vec3 cloud_overlay = blinn_phong(ka, kd, ks, p, n, v, l);
      color += 10.0 * cloud_overlay;
    }
  }
}
