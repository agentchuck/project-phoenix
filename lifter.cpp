#include <includes.hh>
#include <world.hh>

ofstream debug("debug.txt", fstream::trunc);

//
// Add variables here.
//
World world;
string bestMove("A");

int main (int argc, char **argv)
{
  // Read the world
  world.init();

  World copyWorld(world);
  // Output the best move
  cout << bestMove << endl;

  return 0;
}
