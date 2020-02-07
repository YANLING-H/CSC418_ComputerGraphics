#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include <utility>

// Typedef to make typing easier
typedef std::pair<std::shared_ptr<Object>, double> queue_obj;

bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant)
{
  // For comparing the object priority pair
  struct comparator {
    bool operator() (const queue_obj& a, const queue_obj& b) const
    {
      return a.second > b.second;
    }
  };

  std::priority_queue<queue_obj, std::vector<queue_obj>, comparator> pq;  // Priority queue
  pq.push(std::make_pair(root, point_box_squared_distance(query, root->box)));  // Insert root

  while (!pq.empty()) {
    // Take top element
    queue_obj top = pq.top();
    pq.pop();

    double d = top.second;
    if (d < max_sqrd) {
      std::shared_ptr<AABBTree> aabb = std::dynamic_pointer_cast<AABBTree>(top.first);
      if (!aabb) {
        // node is a leaf object.
        if (d >= min_sqrd) {
          // First leaf object at >= min_sqrd
          // is the closest to the query point
          // as all remaining objects in pq
          // are at greater distances
          sqrd = d;
          descendant = top.first;
          return true;
        } else continue;  // Object is closer than min_sqrd so skip this object
      }

      // aabb is an AABBTree. Add children to pq if they
      // are under max_sqrd distance away
      if (aabb->left) {
        d = point_box_squared_distance(query, aabb->left->box);
        if (d < max_sqrd)
          pq.push(std::make_pair(aabb->left, d));
      }

      if (aabb->right) {
        d = point_box_squared_distance(query, aabb->right->box);
        if (d < max_sqrd)
          pq.push(std::make_pair(aabb->right, d));
      }
    } else {
      // If d is greater than max_sqrd, then there is no object
      // closer than max_sqrd as all remaining elements
      // in pq have larger distance
      return false;
    }
  }
  // pq empty before finding any object
  return false;
}
