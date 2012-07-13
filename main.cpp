#include <includes.hh>
#include <world.hh>

ofstream debug("debug.txt", fstream::trunc);


//
// Add variables here.
//
World world;

int main (int argc, char **argv)
{
  std::string input_line;
  while (std::cin) {
    getline(std::cin, input_line);
    if (input_line.length() > 0) {
      world.inputLine(input_line);
    }
  }
  cerr << world.width << "x" << world.height << endl;
  world.finalizeInput();
  world.dump();

  // Get the moves from a file
  ifstream inFile(argv[1]);
  inFile >> input_line;

  // Parse moves.
  size_t num_moves = input_line.length();
  int move_counter;
  for0n(move_counter, num_moves) {
    world.update(input_line[move_counter]);
  }


  return 0;
}
