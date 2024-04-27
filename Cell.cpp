#include <iostream>

#include "Cell.h"

Cell* Cell::Up() {
  if (up != nullptr) {
    return up;
  }
  Cell* temp = new Cell{this->x, this->y-1};
  up = temp;
  Cell* prev;
  temp->down = this;
  while (temp->down->right != nullptr) {
    prev = temp;
    temp = new Cell{prev->x+1, prev->y};
    temp->left = prev;
    temp->down = prev->down->right;
    prev->right = temp;
    prev->down->right->up = temp;
  }

  temp = up;
  while (temp->down->left != nullptr) {
    prev = temp;
    temp = new Cell{prev->x-1, prev->y};
    temp->right = prev;
    temp->down = prev->down->left;
    prev->left = temp;
    prev->down->left->up = temp;
  }

  return up;
}

Cell* Cell::Right() {
  if (right != nullptr) {
    return right;
  }
  Cell* temp = new Cell{this->x+1, this->y};
  right = temp;
  Cell* prev;
  temp->left = this;
  while (temp->left->down != nullptr) {
    prev = temp;
    temp = new Cell{prev->x, prev->y+1};
    temp->up = prev;
    temp->left = prev->left->down;
    prev->down = temp;
    prev->left->down->right = temp;
  }

  temp = right;
  while (temp->left->up != nullptr) {
    prev = temp;
    temp = new Cell{prev->x, prev->y-1};
    temp->down = prev;
    temp->left = prev->left->up;
    prev->up = temp;
    prev->left->up->right = temp;
  }

  return right;
}

Cell* Cell::Down() {
  if (down != nullptr) {
    return down;
  }
  Cell* temp = new Cell{this->x, this->y+1};
  down = temp;
  Cell* prev;
  temp->up = this;
  while (temp->up->left != nullptr) {
    prev = temp;
    temp = new Cell{prev->x-1, prev->y};
    temp->right = prev;
    temp->up = prev->up->left;
    prev->left = temp;
    prev->up->left->down = temp;
  }

  temp = down;
  while (temp->up->right != nullptr) {
    prev = temp;
    temp = new Cell{prev->x+1, prev->y};
    temp->left = prev;
    temp->up = prev->up->right;
    prev->right = temp;
    prev->up->right->down = temp;
  }

  return down;
}

Cell* Cell::Left() {
  if (left != nullptr) {
    return left;
  }
  Cell* temp = new Cell{this->x-1, this->y};
  left = temp;
  Cell* prev;
  temp->right = this;
  while (temp->right->up != nullptr) {
    prev = temp;
    temp = new Cell{prev->x, prev->y-1};
    temp->down = prev;
    temp->right = prev->right->up;
    prev->up = temp;
    prev->right->up->left = temp;
  }

  temp = left;
  while (temp->right->down != nullptr) {
    prev = temp;
    temp = new Cell{prev->x, prev->y+1};
    temp->up = prev;
    temp->right = prev->right->down;
    prev->down = temp;
    prev->right->down->left = temp;
  }

  return left;
}
