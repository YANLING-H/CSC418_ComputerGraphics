// Input:
//   N  3D unit normal vector
// Outputs:
//   T  3D unit tangent vector
//   B  3D unit bitangent vector
void tangent(in vec3 N, out vec3 T, out vec3 B)
{
  float theta = atan(N.y / N.x);
  float phi = acos(N.z);

  float sin_phi = sin(phi);
  float sin_theta = sin(theta);
  float cos_phi = cos(phi);
  float cos_theta = cos(theta);

  T = vec3(cos_phi * cos_theta, cos_phi * sin_theta, sin_phi);
  B = vec3(-sin_theta, cos_theta, 0);
}
