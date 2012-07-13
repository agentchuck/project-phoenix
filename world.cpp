#include <world.hh>

World::World() :
  width(0),
  height(0),
  score(0),
  lambdasRemaining(0),
  lambdasCollected(0)
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
  lambdasRemaining += std::count(input.begin(), input.end(), '\\');
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
World::at(pii location)
{
  return at(location.first, location.second);
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
World::update(pii location, char changeTo)
{
  update(location.first, location.second, changeTo);
}

void
World::update(int i, int j, char changeTo)
{
  if ((i < 1) || (i > width) ||
      (j < 1) || (j > height)) {
    return;
  }
  mine[height - j][i - 1] = changeTo;
}

void
World::update(char move)
{
  pii to = robotLocation;
  cerr << "--------------------------" << endl;
  cerr << "Move: " << move << endl;
  bool moveHorizontally = false;
  bool moveVertically = false;

  switch (move) {
    case 'U':
      moveVertically = true;
      to.second++;
      break;
    case 'D':
      moveVertically = true;
      to.second--;
      break;
    case 'L':
      moveHorizontally = true;
      to.first--;
      break;
    case 'R':
      moveHorizontally = true;
      to.first++;
      break;
    case 'W':
      break;
    case 'A':
      break;
    default:
      cerr << "Invalid input!!" << endl;
  }

  bool invalidMove = false;
  char movingInto = at(to);
  cerr << "Moving into: " << movingInto << endl;

  if ((movingInto == '#') ||
      (movingInto == 'L')) {
    // Invalid move.
    invalidMove = true;
  } else if (movingInto == '*') {
    if (!moveHorizontally) {
      // Invalid move.
      invalidMove = true;
    } else if (move == 'L') {
      if (at(to.first - 1, to.second) != ' ') {
        invalidMove = true;
      } else {
        update(to.first - 1, to.second, '*');
      }
    } else if (move == 'R') {
      if (at(to.first + 1, to.second) != ' ') {
        invalidMove = true;
      } else {
        update(to.first + 1, to.second, '*');
      }
    }
  } else if (movingInto == '\\') {
    lambdasCollected++;
    lambdasRemaining--;
    score += 25;
  }
  if (invalidMove) {
    to = robotLocation;
    move = 'W';
  } else {
    update(robotLocation, ' ');
    update(to, 'R');
  }
  robotLocation = to;

  // Process gravity
  processGravity();

  score--;
  dump();
}

void
World::processGravity()
{
  int i, j;
  for1n(i, width) {
    for1n(j, height) {
      char thingAt = at(i, j);
      if (thingAt == 'L') {
        if (lambdasRemaining == 0) {
          update(i, j, 'O');
        }
      }
    }
  }

}

void
World::dump()
{
  cerr << "Score: " << score << endl;
  int i, j;
  for(j = height; j > 0; j--) {
    for(i = 1; i <= width; i++) {
      cerr << at(i, j);
    }
    cerr << endl;
  }
  cerr << "Score: " << score << endl;
}
