#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include <utility>

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

  std::priority_queue<queue_obj, std::vector<queue_obj>, comparator> pq;
  pq.push(std::make_pair(root, point_box_squared_distance(query, root->box)));

  while (!pq.empty()) {
    queue_obj top = pq.top();
    pq.pop();

    double d = top.second;
    if (d < max_sqrd) {
      std::shared_ptr<AABBTree> aabb = std::dynamic_pointer_cast<AABBTree>(top.first);
      if (!aabb) {
        if (d >= min_sqrd) {
          sqrd = d;
          descendant = top.first;
          return true;
        } else continue;
      }

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
      return false;
    }
  }
  return false;
}
