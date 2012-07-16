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
  metaData(),
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
  exitLocation(fromWorld.exitLocation),
  metaData(fromWorld.metaData)
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
  exitLocation(fromWorld.exitLocation),
  metaData(fromWorld.metaData)
{
  // TODO: Make this work.
}


void
World::init()
{
  metaData.trampsNum = 0;

  std::string input_line;
  bool meta_data_start = false;
  while (std::cin) {
    getline(std::cin, input_line);
    if (input_line.length() > 0) {
      if (!meta_data_start) inputLine(input_line);
      else inputMeta(input_line);
    }
    else meta_data_start = true;
  }
  finalizeInput();
}

void
World::inputMeta(string input)
{
  // Trampoline A targets 1
  if (input.find("Trampoline")>=0) 
  {
    metaData.tramps[metaData.trampsNum][0] = input[11]; // Horrible code
    metaData.tramps[metaData.trampsNum][1] = input[21]; // Horrible code
    //cerr << metaData.tramps[metaData.trampsNum];
    metaData.trampsNum++;
  }
  // Water
  // Growth
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
    //cerr << "ROBOT" << robotLocation.first << " " << robotLocation.second << endl;
  }
  pos = input.find("L");
  if(string::npos != pos)
  {
    // Found the exit. Mark its location.
    exitLocation.first = pos + 1;
    exitLocation.second = height;
  }
  lambdasRemaining += std::count(input.begin(), input.end(), '\\');
  lambdasRemaining += std::count(input.begin(), input.end(), '@');
  height++;
}

void
World::finalizeInput()
{
  robotLocation.second = height - robotLocation.second;
  exitLocation.second = height - exitLocation.second;
  //cerr << "ROBOT" << robotLocation.first << " " << robotLocation.second << endl;

  worldState = Running;
  //cerr << "Robot at: " << robotLocation.first << "x" << robotLocation.second << endl;
  //cerr << "Exit at: " << exitLocation.first << "x" << exitLocation.second << endl;
  // Trampoline
  int i,j;
  for1n(i, width) {
    for1n(j, height) {
      char thingAt = at(i, j);
      int code = thingAt;
      //if (thingAt == 'R') cerr << i << j << 'R' << endl;

      if ((code >= 49) && (code <= 57))
        for (int k=0; k<metaData.trampsNum; k++){
          if (metaData.tramps[k][1] == thingAt) metaData.trampsLoc[k][1] = make_pair(i,j);
          //cerr << i << " " << j << " " << thingAt << endl;
    }
      if ((code >= 65) && (code <= 73))
        for (int k=0; k<metaData.trampsNum; k++)
          if (metaData.tramps[k][0] == thingAt) metaData.trampsLoc[k][0] = make_pair(i,j);
        // ((code >= 49) && (code <= 57)))   1-9
        // ((code >= 65) && (code <= 73))    A-I
    }}

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

long
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

char
World::at(pii location, tMine const& curr_mine)
{
  return at(location.first, location.second, curr_mine);
}

char
World::at(int i, int j,  tMine const& curr_mine)
{
  if ((i < 1) || (i > width) ||
      (j < 1) || (j > height)) {
    return '#';
  }
  return curr_mine[height - j][i - 1];
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
  cerr << "--------------------------" << endl;
  cerr << "Move: " << move << endl;
  if (worldState != Running) {
    cerr << "Invalid state. No more moves allowed." << endl;
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
  cerr << "Moving into: " << movingInto << endl;
  int code = movingInto;

  if ((movingInto == '#') ||
      (movingInto == 'L') || ((code >= 49) && (code <= 57))) { // 49-57 are ASCII for 1-9
    // Invalid move.
    invalidMove = true;

  } else if ((movingInto == '*') || (movingInto == '@')) {
    if (!moveHorizontally) {
      // Invalid move.
      invalidMove = true;
    } else if (move == 'L') {
      if (at(to.first - 1, to.second) != ' ') {
        invalidMove = true;
      } else {
        update(to.first - 1, to.second, movingInto);
      }
    } else if (move == 'R') {
      if (at(to.first + 1, to.second) != ' ') {
        invalidMove = true;
      } else {
        update(to.first + 1, to.second, movingInto);
      }
    }
  } else if (movingInto == '\\') {
    lambdasCollected++;
    lambdasRemaining--;
  } else if (movingInto == 'O') {
    worldState = Won;
    cerr << "YOU HAVE WON!" << endl;
  } else if ((code >= 65) && (code <= 73)) // ASCII for A - I
  { // Trampolines
    for (int k = 0; k < metaData.trampsNum; k++){
      if (metaData.tramps[k][0] == movingInto){
        // Robot jumps!
        //cerr << "YES!" << metaData.tramps[k][0] << " " << metaData.tramps[k][1] << endl; // delete 
        //cerr << metaData.trampsLoc[k][1].first << " " << metaData.trampsLoc[k][1].second  << endl; // ???
        update(robotLocation, ' ');
        update(to, ' '); // trampline destroys
        //update(metaData.trampsLoc[k][0].first, metaData.trampsLoc[k][0].second, 'Z');
        //update(metaData.trampsLoc[k][1], 'R');

        robotLocation = metaData.trampsLoc[k][0];
        to = metaData.trampsLoc[k][1];
        // deletion of similar tramplines
        for (int z = 0; z < metaData.trampsNum; z++)
          if ((metaData.tramps[z][1] == metaData.tramps[k][1]) && (z != k))
            update(metaData.trampsLoc[z][0], ' ');
      }
    }
  }
  if (invalidMove) {
    to = robotLocation;
    move = 'W';
  } else {
    update(robotLocation, ' ');
    update(to, 'R');
  }
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

// Global variable: needs to be for every "gravity update" call.
tMine oldMine;

void
World::processGravity()
{
  //cerr << lambdasRemaining << "L" << endl;
  int i, j;  
  for1n(i, width) 
    for1n(j,height)
      oldMine[j,i] = mine[j,i];

  for1n(i, width) {
    for1n(j, height) {
      char thingAt = at(i, j, oldMine);
      if (thingAt == 'L') {
        if (lambdasRemaining == 0) {
          update(i, j, 'O');
        }
      } else if ((thingAt == '*') || (thingAt == '@')) {
        char thingBelow = at(i, j-1, oldMine);
        pii  deadCheckAt(0, 0);
        if (thingBelow == ' ') {
          // Drop down - FALL event
          // Horock Crash check:
          if ((thingAt == '@') && (at(i, j-2, oldMine) != ' ' )) thingAt = '\\';
          update(i, j, ' ');
          update(i, j-1, thingAt);
          deadCheckAt = make_pair(i, j-2);
        } else if (thingBelow == '*' || thingBelow == '@') {
          if ((at(i+1,j, oldMine) == ' ') && (at(i+1,j-1, oldMine) == ' ')) {
          //Horock Crash check: yes, valid as per comment
            if ((thingAt == '@') && (at(i+1, j-2, oldMine) != ' ' )) thingAt = '\\';
            update(i, j, ' ');
            update(i+1, j-1, thingAt);
            deadCheckAt = make_pair(i+1, j-2);
          } else if ((at(i-1,j, oldMine) == ' ') && (at(i-1,j-1, oldMine) == ' ')) {
          //Horock Crash check: yes, valid as per comment
            if ((thingAt == '@') && (at(i-1, j-2, oldMine) != ' ' )) {thingAt = '\\'; }
            update(i, j, ' ');
            update(i-1, j-1, thingAt);
            deadCheckAt = make_pair(i-1, j-2);
          }
        } else if (thingBelow == '\\') {
            if ((at(i+1,j, oldMine) == ' ') && (at(i+1,j-1, oldMine) == ' ')) {
            //Horock Crash check: yes, valid as per comment
            if ((thingAt == '@') && (at(i+1, j-2, oldMine) != ' ' )) thingAt = '\\';
            update(i, j, ' ');
            update(i+1, j-1, thingAt);
            deadCheckAt = make_pair(i+1, j-2);
          }
        }
        if (at(deadCheckAt, oldMine) == 'R') {
          if (worldState == Running) {
            worldState = Dead;
            cerr << "YOU ARE DEAD!" << endl;
          }
        }
        if (deadCheckAt.first != 0) {
          changed = true;
        }
      }
    }
  }
  //dump();
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
