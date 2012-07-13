#include <world.hh>

World::World() : width(0), height(0), score(0)
{
  robotLocation.first = 0;
  robotLocation.second = 0;
  exitLocation.first = 0;
  exitLocation.second = 0;
}

void
World::inputLine(string input)
{
  mine[height] = input;
  width = max(input.length(), width);

  string::size_type pos = input.find("R");
  if(string::npos != pos)
  {
    // Found the Robot. Mark its location.
    robotLocation.first = pos + 1;
    robotLocation.second = height;
  }
  pos = input.find("L");
  if(string::npos != pos)
  {
    // Found the exit. Mark its location.
    exitLocation.first = pos + 1;
    exitLocation.second = height;
  }
  height++;
}

void
World::finalizeInput()
{
  robotLocation.second = height - robotLocation.second;
  exitLocation.second = height - exitLocation.second;

  cerr << "Robot at: " << robotLocation.first << "x" << robotLocation.second << endl;
  cerr << "Exit at: " << exitLocation.first << "x" << exitLocation.second << endl;
}

char
World::at(int i, int j)
{
  if ((i < 1) || (i > width) ||
      (j < 1) || (j > height)) {
    return '#';
  }
  return mine[height - j][i - 1];
}

void
World::update(char move)
{
}
