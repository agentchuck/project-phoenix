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

  while(!bfsList.empty() && !rxSIGINT) {
    // TODO: Full of memory leaks...
    State* state = bfsList.front();

    int i;
    for0n(i, 6) {
      World* newWorld = new World(*state->world);
      newWorld->update(moves[i]);
      if (newWorld->changed) {
        State* newState = new State(newWorld,
            newWorld->score(),
            state->commands + moves[i]);
        if (newWorld->score() > bestScore) {
          bestScore = newWorld->score();
          bestMove = newState->commands;
        }
        if (newWorld->worldState == World::Running) {
          // Check if we've already had this state.
          StateMapType::iterator it = stateMap.find(newWorld->hashMap());
          if (it != stateMap.end()) {
            // Check if the score is better.
            if (newWorld->score() > (*it->second).score) {
              stateMap[newWorld->hashMap()] = newState;
              bfsList.push_back(newState);
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
          free(newWorld);
          break;
        } else {
          free(newState);
          free(newWorld);
        }
      } else {
        free(newWorld);
      }
    }

    free(state);
    bfsList.pop_front();
  }

  // Output the best move
  cout << bestMove << endl;

  return 0;
}