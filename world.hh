#ifndef WORLD_HH
#define WORLD_HH

#include <includes.hh>

class World {
  private:
    // TODO: make better
    string mine[1000];

  public:
    size_t width;
    size_t height;
    int score;
    int lambdasRemaining;

    pii robotLocation;
    pii exitLocation;

    World();

    // Input a line
    void inputLine(string input);
    // Finalize the input. This is called after all the input is read in.
    void finalizeInput();

    // Find out what is at a set of co-ordinates.
    char at(int i, int j);
    // Make a move and update the world.
    void update(char move);
};

#endif
