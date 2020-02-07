#include "AABBTree.h"

// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const 
{
  if (ray_intersect_box(ray, this->box, min_t, max_t)) {
    // t values for left and right children
    double l_t, r_t;

    // Descendant values for left and right children
    std::shared_ptr<Object> l_d = this->left, r_d = this->right;

    // Whether ray hit left or right children
    bool l_h = this->left && this->left->ray_intersect(ray, min_t, max_t, l_t, l_d);
    bool r_h = this->right && this->right->ray_intersect(ray, min_t, max_t, r_t, r_d);

    if (l_h && r_h) {
      // If hit both children, take the closer hit
      if (l_t < r_t) {
        t = l_t;
        descendant = l_d;
      } else {
        t = r_t;
        descendant = r_d;
      }
    } else if (l_h) {
      // Hit left but not right
      t = l_t;
      descendant = l_d;
    } else if (r_h) {
      // Hit right but not left
      t = r_t;
      descendant = r_d;
    } else {
      // Hit neither of the children
      return false;
    }
    // Hit is true
    return true;
  }
  // Did not hit bounding box
  return false;
}

