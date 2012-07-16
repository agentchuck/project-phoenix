#include <world.hh>
#include <math.h>
#include <iostream>
#include <stdio.h>

World::World() :
  width(0),
  height(0),
  moves(0),
  lambdasRemaining(0),
  lambdasCollected(0),
  worldState(Initializing)
{
  robotLocation.first = 0;
  robotLocation.second = 0;
  exitLocation.first = 0;
  exitLocation.second = 0;
}

World::World(World const& fromWorld) :
  width(fromWorld.width),
  height(fromWorld.height),
  moves(fromWorld.moves),
  lambdasRemaining(fromWorld.lambdasRemaining),
  lambdasCollected(fromWorld.lambdasCollected),
  worldState(fromWorld.worldState),
  robotLocation(fromWorld.robotLocation),
  exitLocation(fromWorld.exitLocation)
{
  int i;
  for0n(i, height) {
    mine[i] = fromWorld.mine[i];
  }
}

World::World(World const& fromWorld, int fromI, int toI, int fromJ, int toJ) :
  width(fromWorld.width),
  height(fromWorld.height),
  moves(fromWorld.moves),
  lambdasRemaining(fromWorld.lambdasRemaining),
  lambdasCollected(fromWorld.lambdasCollected),
  worldState(fromWorld.worldState),
  robotLocation(fromWorld.robotLocation),
  exitLocation(fromWorld.exitLocation)
{
  // TODO: Make this work.
}


void
World::init()
{
  std::string input_line;
  while (std::cin) {
    getline(std::cin, input_line);
    if (input_line.length() > 0) {
      inputLine(input_line);
    }
  }
  finalizeInput();
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
  worldState = Running;
  //cerr << "Robot at: " << robotLocation.first << "x" << robotLocation.second << endl;
  //cerr << "Exit at: " << exitLocation.first << "x" << exitLocation.second << endl;
}

int
World::score()
{
  int theScore = 25 * lambdasCollected;
  theScore -= moves;
  if (worldState == Won) {
    theScore += 50 * lambdasCollected;
  } else if (worldState == Abort) {
    theScore += 25 * lambdasCollected;
  }

  return theScore;
}

World::HashType
World::hashMap()
{
  int size = height*width;
  int code;
  double addition = ( size % 101 ) / sqrt(3);
  double sum = 0;
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
    {
      code = (int)mine[j][i];
      sum += sin(code*0.001745329 + i*j/addition);
    }
  return modf(sum, &addition)*100000000; // not that I care about addition any longer
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
  // Reset the changed flag.
  changed = false;
  pii to = robotLocation;
  //cerr << "--------------------------" << endl;
  //cerr << "Move: " << move << endl;
  if (worldState != Running) {
    //cerr << "Invalid state. No more moves allowed." << endl;
    return;
  }
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
      if (worldState != Won) {
        worldState = Abort;
      }
      break;
    default:
      cerr << "Invalid input!!" << endl;
  }

  bool invalidMove = false;
  char movingInto = at(to);
  //cerr << "Moving into: " << movingInto << endl;

  if ((movingInto == '#') ||
      (movingInto == 'L')) {
    // Invalid move.
    invalidMove = true;
  } else if ((movingInto == '*') || (movingInto == '+')) {
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
  } else if (movingInto == 'O') {
    worldState = Won;
    //cerr << "YOU HAVE WON!" << endl;
  }
  if (at(to.first, to.second + 1) == '+') {
    // We've moved under a falling rock.
    if (worldState = Running) {
      worldState = Dead;
    }
  }
  if (invalidMove) {
    to = robotLocation;
    move = 'W';
  } else {
    update(robotLocation, ' ');
    update(to, 'R');
  }

  // Update the robot location.
  robotLocation = to;

  changed = (move != 'W');

  // Process gravity
  processGravity();

  if (move != 'A') {
    // Special case. Don't count the move for abort for scoring.
    moves++;
  }
  //dump();
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
      } else if ((thingAt == '*') || (thingAt == '+')) {
        char thingBelow = at(i, j-1);
        pii  deadCheckAt(0, 0);
        if (thingBelow == ' ') {
          // Drop down
          update(i, j, ' ');
          update(i, j-1, '+');
          deadCheckAt = make_pair(i, j-2);
        } else if ((thingBelow == '*') || (thingBelow == '+'))   {
          if ((at(i+1,j) == ' ') && (at(i+1,j-1) == ' ')) {
            update(i, j, ' ');
            update(i+1, j-1, '+');
            deadCheckAt = make_pair(i+1, j-2);
          } else if ((at(i-1,j) == ' ') && (at(i-1,j-1) == ' ')) {
            update(i, j, ' ');
            update(i-1, j-1, '+');
            deadCheckAt = make_pair(i-1, j-2);
          }
        } else if (thingBelow == '\\') {
          if ((at(i+1,j) == ' ') && (at(i+1,j-1) == ' ')) {
            update(i, j, ' ');
            update(i+1, j-1, '+');
            deadCheckAt = make_pair(i+1, j-2);
          }
        } else {
          // Change the rock to a stopped rock in case it was moving.
          update(i, j, '*');
	}
        if (at(deadCheckAt) == 'R') {
          if (worldState == Running) {
            worldState = Dead;
            //cerr << "YOU ARE DEAD!" << endl;
          }
        }
        if (deadCheckAt.first != 0) {
          changed = true;
        }
      }
    }
  }
}

void
World::dump()
{
  int i, j;
  for(j = height; j > 0; j--) {
    for(i = 1; i <= width; i++) {
      cerr << at(i, j);
    }
    cerr << endl;
  }
  cerr << "Score: " << score() << endl;
}
