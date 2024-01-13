#pragma config(StandardModel, "EV3_REMBOT")
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*	Variable Declaration	*/

// Variables for screen size:
const int screenWidth = 177;
const int screenHeight = 127;

// Variables for Maze division:
const int totalRows = 4;
const int totalColumns = 6;

// Variables for pathing declaration:
#define PATH 0
#define BLOCKED 1

// Variables for robot position and heading: 0 = N, 1 = E, 2 = S, 3 = W
int robotHeading = 0;
int robotRow = 0;
int robotColumn = 0;

// Variables for target cell:
const int targetRow = 1;
const int targetColumn = 3;
// (Need to create X for final column position)

// Maze cell creation:
typedef struct
{
	int northWall;
	int southWall;
	int westWall;
	int eastWall;
	int numTimesVisited;
} Cell;

// Maze grid declaration:
Cell Maze[totalRows][totalColumns];

// Drawing Maze:
// Build outer border first and set to blocked:
void outerMazeWallBorder();

// Set inner borders to whichever cells they need:
void constructInnerMaze();

// Draw grid on screen:
void drawGridOnScreen();

// Draw the robot and get it to do the Maze:
void robotDisplay();

// Movement tech:

void turnRight();

void turnLeft();

void goFwd();

// Check the wall for walls:
int checkWall();

// Array of visited cells:
const int arraySize = 72;
int arrayCellInputIndex = 0;
int visitedCells[arraySize];

/*--------------------------------------------------
// Main task begin
--------------------------------------------------*/
task main()
{
	outerMazeWallBorder();
	constructInnerMaze();
	drawGridOnScreen();

	//Go around the Maze using a right-sided checking algorithm
	while ((robotRow != targetRow) || (robotColumn != targetColumn))
	{
		// turn right
		turnRight();

		while(checkWall()==1)
		{
			turnLeft();
			sleep(200);
		}
		Maze[robotRow][robotColumn].numTimesVisited++;
		goFwd();

		//If the cell is found, display winning info:
		if ((robotRow == targetRow) && (robotColumn == targetColumn))
		{
			while(true){
				displayCenteredBigTextLine(3,"Maze SOLVED!");
				sleep(10000);
				eraseDisplay();
				sleep(500);
				break;
			}
		}

		for (int i = 0; i < arraySize - 1; i++)
		{

		}


	}
}

/*--------------------------------------------------
// Main task end
--------------------------------------------------*/

// Function construction
void outerMazeWallBorder()
{
	for(int row = 0; row < totalRows; row++)
	{
		Maze[row][0].westWall = BLOCKED;
		Maze[row][totalColumns - 1].eastWall = BLOCKED;
	}

	for(int column = 0; column < totalColumns; column++)
	{
		Maze[0][column].southWall = BLOCKED;
		Maze[totalRows-1][column].northWall = BLOCKED;
	}
}

void drawGridOnScreen()
{
	// Array value for each cell
	int xStartValue = 0;
	int yStartValue = 0;
	int xEndValue = 0;
	int yEndValue = 0;
	const int columnWidthSegment = (screenWidth/totalColumns);
	const int rowHeightSegment = (screenHeight/totalRows);

	// Go through and draw the walls
	for (int row = 0; row < totalRows; row++)
	{

		for (int column = 0; column < totalColumns; column++)
		{

			if (Maze[row][column].northWall == BLOCKED){
				xStartValue = column * columnWidthSegment;
				xEndValue = (column + 1) * columnWidthSegment;
				yStartValue = (row + 1) * rowHeightSegment;
				yEndValue = (row + 1) * rowHeightSegment;
				drawLine(xStartValue, yStartValue, xEndValue, yEndValue);
			}

			if (Maze[row][column].eastWall == BLOCKED){
				xStartValue = (column + 1) * columnWidthSegment;
				xEndValue = (column + 1) * columnWidthSegment;
				yStartValue = row * rowHeightSegment;
				yEndValue = (row + 1) * rowHeightSegment;
				drawLine(xStartValue, yStartValue, xEndValue, yEndValue);
			}

			if (Maze[row][column].southWall == BLOCKED){
				xStartValue = column * columnWidthSegment;
				xEndValue = (column + 1) * columnWidthSegment;
				yStartValue = row * rowHeightSegment;
				yEndValue = row * rowHeightSegment;
				drawLine(xStartValue, yStartValue, xEndValue, yEndValue);
			}

			if (Maze[row][column].westWall == BLOCKED){
				xStartValue = column * columnWidthSegment;
				xEndValue = column * columnWidthSegment;
				yStartValue = row * rowHeightSegment;
				yEndValue = (row + 1) * rowHeightSegment;
				drawLine(xStartValue, yStartValue, xEndValue, yEndValue);

			}
		}
	}
}

void constructInnerMaze()
{
	/*
	Rows/columns need to be in pairs, for blocked wall definitions on both sides (ie. not go through a blocked wall, on one side.

	value pairs:
	[3,0]|[3,1]|[3,2]|[3,3]|[3,4]|[3,5]
	-----------------------------------
	[2,0]|[2,1]|[2,2]|[2,3]|[2,4]|[2,5]
	-----------------------------------
	[1,0]|[1,1]|[1,2]|[1,3]|[1,4]|[1,5]
	-----------------------------------
	[0,0]|[0,1]|[0,2]|[0,3]|[0,4]|[0,5]
	*/

	// Vertical walls:
	Maze[0][0].eastWall = Maze[0][1].westWall = BLOCKED;
	Maze[3][0].eastWall = Maze[3][1].westWall = BLOCKED;
	Maze[0][3].eastWall = Maze[0][4].westWall = BLOCKED;
	Maze[1][1].eastWall = Maze[1][2].westWall = BLOCKED;
	Maze[2][3].eastWall = Maze[2][4].westWall = BLOCKED;
	Maze[2][4].eastWall = Maze[2][5].westWall = BLOCKED;
	Maze[3][3].eastWall = Maze[3][4].westWall = BLOCKED;
	Maze[3][4].eastWall = Maze[3][5].westWall = BLOCKED;

	// Horizontal walls:
	Maze[1][0].northWall = Maze[2][0].southWall = BLOCKED;
	Maze[1][1].northWall = Maze[2][1].southWall = BLOCKED;
	Maze[2][1].northWall = Maze[3][1].southWall = BLOCKED;
	Maze[2][2].northWall = Maze[3][2].southWall = BLOCKED;
	Maze[1][3].northWall = Maze[2][3].southWall = BLOCKED;
	Maze[0][4].northWall = Maze[1][4].southWall = BLOCKED;
	Maze[0][2].northWall = Maze[1][2].southWall = BLOCKED;
}

void robotDisplay()
{
	eraseDisplay();
	drawGridOnScreen();

	switch(robotHeading){
	case 0:
		displayStringAt(((robotColumn*2)+1)*screenWidth/12,((robotRow*2)+1)* screenHeight/8, "^");
		break;
	case 1:
		displayStringAt(((robotColumn*2)+1)*screenWidth/12,((robotRow*2)+1)* screenHeight/8, ">");
		break;
	case 2:
		displayStringAt(((robotColumn*2)+1)*screenWidth/12,((robotRow*2)+1)* screenHeight/8, "v");
		break;
	case 3:
		displayStringAt(((robotColumn*2)+1)*screenWidth/12,((robotRow*2)+1)* screenHeight/8, "<");
		break;
	default:
		break;
	}

	sleep(200);
}

void turnRight(){
	switch(robotHeading){
	case 0: robotHeading = 1; break;
	case 1: robotHeading = 2; break;
	case 2: robotHeading = 3; break;
	case 3: robotHeading = 0; break;
	default: break;
	}
	robotDisplay();
}

void turnLeft(){
	switch(robotHeading){
	case 0: robotHeading = 3; break;
	case 1: robotHeading = 0; break;
	case 2: robotHeading = 1; break;
	case 3: robotHeading = 2; break;
	default: break;
	}
	robotDisplay();
}

void goFwd(){
	switch(robotHeading){
	case 0: robotRow++; break;
	case 1: robotColumn++; break;
	case 2: robotRow--; break;
	case 3: robotColumn--; break;
	default: break;
	}
	robotDisplay();
}

int checkWall(){
	switch(robotHeading){
	case 0:
		return Maze[robotRow][robotColumn].northWall;
		break;
	case 1:
		return Maze[robotRow][robotColumn].eastWall;
		break;
	case 2:
		return Maze[robotRow][robotColumn].southWall;
		break;
	case 3:
		return Maze[robotRow][robotColumn].westWall;
		break;
	default:
		break;
	}
	return -1;
}

// One solution idea:
// Have a cell portion that asks if it was visited (ie. int visited), and when (which array value)
// If visited, and left, increase by 1
// If at two (ie, visited twice), delete all points from that previous point and consolodate into new array
// If the first visit,and second visit, have opposite enter and exit points, delete node.
