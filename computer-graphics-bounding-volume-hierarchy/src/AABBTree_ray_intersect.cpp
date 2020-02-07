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
    double l_t, r_t;
    std::shared_ptr<Object> l_d = this->left, r_d = this->right;

    bool l_h = this->left && this->left->ray_intersect(ray, min_t, max_t, l_t, l_d);
    bool r_h = this->right && this->right->ray_intersect(ray, min_t, max_t, r_t, r_d);

    if (l_h && r_h) {
      if (l_t < r_t) {
        t = l_t;
        descendant = l_d;
      } else {
        t = r_t;
        descendant = r_d;
      }
    } else if (l_h) {
      t = l_t;
      descendant = l_d;
    } else if (r_h) {
      t = r_t;
      descendant = r_d;
    } else {
      return false;
    }
    return true;
  }
  return false;
}

