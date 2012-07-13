#ifndef WORLD_HH
#define WORLD_HH

#include <includes.hh>

class World {
  public:
    size_t width;
    size_t height;
    int score;
    World();
    void update(char move);
};

#endif
