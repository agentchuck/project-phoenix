#ifndef WORLD_HH
#define WORLD_HH

#include <includes.hh>

class World {
  private:
    // TODO: make better
    string mine[1000];

    // Input a line
    void inputLine(string input);
    // Finalize the input. This is called after all the input is read in.
    void finalizeInput();

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
    // Indicates if the last move changed anything.
    bool changed;

    pii robotLocation;
    pii exitLocation;

    World();
    // Copy constructor
    World(World const& fromWorld);
    // construct a world from a subsection of another one.
    World(World const& fromWorld, int fromI, int toI, int fromJ, int toJ);

    // Read a world state in from stdin.
    void init();

    // Calculate the current score.
    int score();

    // Hash the map
    long hashMap();
    
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
