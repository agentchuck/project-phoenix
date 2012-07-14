#include <includes.hh>
#include <world.hh>

ofstream debug("debug.txt", fstream::trunc);

//
// Add variables here.
//
World world;

int main (int argc, char **argv)
{
  world.init();

  // Get the moves from a file
  ifstream inFile(argv[1]);
  string input_line;
  inFile >> input_line;

  // Parse moves.
  size_t num_moves = input_line.length();
  int move_counter;
  for0n(move_counter, num_moves) {
    world.update(input_line[move_counter]);
    if (world.worldState != World::Running) {
      break;
    }
  }

  return 0;
}
