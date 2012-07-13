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
    int moves;
    int lambdasRemaining;
    int lambdasCollected;
    enum WorldState {
      Initializing = 0,
      Running,
      Won,
      Abort,
      Dead
    };
    WorldState worldState;

    pii robotLocation;
    pii exitLocation;

    World();

    // Input a line
    void inputLine(string input);
    // Finalize the input. This is called after all the input is read in.
    void finalizeInput();
    // Calculate the current score.
    int score();

    // Find out what is at a set of co-ordinates.
    char at(pii location);
    char at(int i, int j);
    // Change something at a co-ordinate.
    void update(pii location, char changeTo);
    void update(int i, int j, char changeTo);
    // Make a move and update the world.
    void update(char move);
    void processGravity();

    // Dump out the state
    void dump();
};

#endif
