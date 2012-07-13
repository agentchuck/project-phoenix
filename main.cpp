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
      cerr << input_line << endl;
    }
  }
  cerr << world.width << "x" << world.height << endl;
  world.finalizeInput();

  // Get the moves from a file
  ifstream inFile(argv[1]);
  inFile >> input_line;
  cerr << input_line << endl;

  cerr << world.at(3, 2) << world.at(10, 17);
  cerr << world.at(0, 0) << world.at(10, 17);
  cerr << endl;
  // Parse moves.
  size_t num_moves = input_line.length();
  int move_counter;
  for0n(move_counter, num_moves) {
    world.update(input_line[move_counter]);
  }


  return 0;
}
