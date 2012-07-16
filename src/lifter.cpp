#include <includes.hh>
#include <world.hh>
#include <signal.h>

ofstream debug("debug.txt", fstream::trunc);

//
// Add variables here.
//
class State {
  public:
    World *world;
    int score;
    string commands;

    State(World *worldRef, int scoreIn, string commandsIn);
};
State::State(World *worldRef, int scoreIn, string commandsIn) :
  world(worldRef),
  score(scoreIn),
  commands(commandsIn)
{}

typedef std::map<World::HashType, State*> StateMapType;
StateMapType stateMap;

World world;
string bestMove("A");
int bestScore = 0;
bool rxSIGINT = false;

list<State *> bfsList;
char moves[6] = {'L', 'R', 'U', 'D', 'W', 'A'};

void siginthandler(int param)
{
  printf("Out of time!\n");
  rxSIGINT = true;
}

int main (int argc, char **argv)
{
  signal(SIGINT, siginthandler);

  // Read the world
  world.init();

  // Create a starting state
  State* startState = new State(&world, 0, "");

  // Push the starting state to the bfs.
  bfsList.push_back(startState);

  int countdown = 1000000;
  while(!bfsList.empty() && !rxSIGINT && (countdown-- > 0)) {
    // TODO: Full of memory leaks...
    State* state = bfsList.front();

    World* stateWorld = new World(world);
    for (string::iterator sit = state->commands.begin();
         sit != state->commands.end();
         sit++) {
      stateWorld->makeStep(*sit);
    }
    int i;
    for0n(i, 6) {
      World* newWorld = new World(*stateWorld);
      if ((newWorld->makeStep(moves[i])) && (newWorld->changed)) {
        State* newState = new State(NULL,
            newWorld->score(),
            state->commands + moves[i]);
        if (newWorld->score() > bestScore) {
          bestScore = newWorld->score();
          bestMove = newState->commands;
        }
        if (newWorld->worldState == World::Running) {
          // Check if we've already had this state.
          // cerr << "Hash: " << newWorld->hashMap() << endl;
          StateMapType::iterator it = stateMap.find(newWorld->hashMap());
          if (it != stateMap.end()) {
            // cerr << "State Collision" << endl;
            // Check if the score is better.
            if (newWorld->score() > (*it->second).score) {
              stateMap[newWorld->hashMap()] = newState;
              bfsList.push_back(newState);
              //cerr << "New state kept" << endl;
            }
          } else {
            stateMap[newWorld->hashMap()] = newState;
            bfsList.push_back(newState);
          }
        } else if (newWorld->worldState == World::Won) {
          // TODO Bail out for now.
          //cerr << "WON!";
          rxSIGINT = true;
          free(newState);
          break;
        } else {
          free(newState);
        }
      }
      free(newWorld);
    }
    free(stateWorld);

    free(state);
    bfsList.pop_front();
  }

  // Output the best move
  cout << bestMove << endl;

  return 0;
}
