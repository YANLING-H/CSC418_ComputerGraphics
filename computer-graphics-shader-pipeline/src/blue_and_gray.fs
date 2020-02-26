// Set the pixel color to blue or gray depending on is_moon.
//
// Uniforms:
uniform bool is_moon;
// Outputs:
out vec3 color;
void main()
{
  vec3 blue = vec3(0.0, 0.0, 1.0);
  vec3 gray = vec3(0.2, 0.2, 0.2);
  
  if (is_moon)
    color = gray;
  else
    color = blue;
}
