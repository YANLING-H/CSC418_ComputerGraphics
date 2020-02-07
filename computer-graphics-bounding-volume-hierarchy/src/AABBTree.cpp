#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <iostream>

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth): 
  depth(std::move(a_depth)), 
  num_leaves(objects.size())
{
  if (num_leaves <= 2) {
    // 1 or 2 objects. So children will be primitives
    this->left = objects[0];
    insert_box_into_box(objects[0]->box, this->box);
    if (num_leaves == 2) {
      // 2 children so right child exists
      this->right = objects[1];
      insert_box_into_box(objects[1]->box, this->box);
    } else this->right = nullptr;
  } else {
    // Get longest axis
    std::vector<double> axis_min(3, std::numeric_limits<double>::infinity()),
                        axis_max(3, -std::numeric_limits<double>::infinity());
    for (auto object : objects) {
      for (int i=0; i<3; ++i) {
        double min = object->box.min_corner[i];
        double max = object->box.max_corner[i];
        if (min < axis_min[i])
          axis_min[i] = min;
        if (max > axis_max[i])
          axis_max[i] = max;
      }
    }

    int axis = 0;
    for (int i=1; i<3; ++i) {
      if (axis_max[i] - axis_min[i] > axis_max[axis] - axis_min[axis])
        axis = i;
    }

    // axis now holds longest axis

    // Comparator to sort objects based on their bounding box centers along longest axis
    auto comparator = [&axis](const std::shared_ptr<Object> &a, const std::shared_ptr<Object> &b) {
      return a->box.center()[axis] < b->box.center()[axis];
    };

    // Sort objects based on longest axis
    std::vector<std::shared_ptr<Object>> obj(objects);
    std::sort(obj.begin(), obj.end(), comparator);

    // Put half the objects on left child and the rest on the right child.
    // Assuming reasonably distributed objects, this approximates division along
    // longest axis. This also fixes the issue where one child could be empty
    // and lead to infinitely many recursive calls.
    auto start = obj.begin();
    auto mid = obj.begin() + num_leaves / 2;
    auto end = obj.end();

    // Attach AABBTrees to children. This recursively calls the constructor.
    this->left = std::make_shared<AABBTree>(std::vector<std::shared_ptr<Object>>(start, mid), this->depth + 1);
    this->right = std::make_shared<AABBTree>(std::vector<std::shared_ptr<Object>>(mid, end), this->depth + 1);

    // Update bounding box
    insert_box_into_box(this->left->box, this->box);
    insert_box_into_box(this->right->box, this->box);
  }
}
