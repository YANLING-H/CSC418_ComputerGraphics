#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"
// Hint: use a list as a queue
#include <list> 

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > & 
    leaf_pairs)
{
  leaf_pairs.clear();
  if (!box_box_intersect(rootA->box, rootB->box))
    return;
  std::list<std::pair<std::shared_ptr<Object>, std::shared_ptr<Object>>> queue;
  queue.push_back(std::make_pair(rootA, rootB));
  while (!queue.empty()) {
    auto nodes = queue.front();
    queue.pop_front();
    std::shared_ptr<AABBTree> nodeA = std::dynamic_pointer_cast<AABBTree>(nodes.first);
    std::shared_ptr<AABBTree> nodeB = std::dynamic_pointer_cast<AABBTree>(nodes.second);

    if (!nodeA && !nodeB) {
      leaf_pairs.push_back(std::make_pair(nodes.first, nodes.second));
    } else if (!nodeA) {
      if (nodeB->left && box_box_intersect(nodes.first->box, nodeB->left->box))
        queue.push_back(std::make_pair(nodes.first, nodeB->left));
      if (nodeB->right && box_box_intersect(nodes.first->box, nodeB->right->box))
        queue.push_back(std::make_pair(nodes.first, nodeB->right));
    } else if (!nodeB) {
      if (nodeA->left && box_box_intersect(nodes.second->box, nodeA->left->box))
        queue.push_back(std::make_pair(nodeA->left, nodes.second));
      if (nodeA->right && box_box_intersect(nodes.second->box, nodeA->right->box))
        queue.push_back(std::make_pair(nodeA->right, nodes.second));
    } else {
      if (nodeA->left && nodeB->left && box_box_intersect(nodeA->left->box, nodeB->left->box))
        queue.push_back(std::make_pair(nodeA->left, nodeB->left));
      if (nodeA->left && nodeB->right && box_box_intersect(nodeA->left->box, nodeB->right->box))
        queue.push_back(std::make_pair(nodeA->left, nodeB->right));
      if (nodeA->right && nodeB->left && box_box_intersect(nodeA->right->box, nodeB->left->box))
        queue.push_back(std::make_pair(nodeA->right, nodeB->left));
      if (nodeA->right && nodeB->right && box_box_intersect(nodeA->right->box, nodeB->right->box))
        queue.push_back(std::make_pair(nodeA->right, nodeB->right));
    }
  }
}
