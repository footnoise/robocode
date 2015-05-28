/*
	Project: ROBOCODE

	File:	 types.h
	
	Description:
	Defines basic data type, classes, varibles and error messages

*/

__ROBOCODE_BEGIN

class CPoint;
class CBattleField;
class CError;

/////////////////////////////
//Basic types
typedef CPoint* Point_t;
typedef std::vector<CPoint> PointsVector_t;

typedef unsigned int Number_t;

//Pixel type, using for tanks and shot coordinates
typedef unsigned int Pixel_t;
//Velocity type, using for moving CPoint, etc tanks and shots
typedef unsigned int Velocity_t;

/////////////////////////////////////////////////////////////////
// Point's angle (facing direction)
typedef enum {RIGHT = 0, UP = 90, LEFT = 180, DOWN = 270} Angle_t;

typedef std::string PointName_t;
typedef std::string CommandName_t;
typedef std::string Error_t;
typedef char* FileName_t;

/////////////////////////////////////////////////////////////
// Point's action, used when point arrived end of BattleField
typedef enum {STOP, EXPLODE} PointAction_t;

//////////////////////////////
// Time
typedef unsigned short Time_t;


/////////////////////////////////////
// Global varibales
const Velocity_t g_TankVelocity = 10; // Tank's velocity
const Velocity_t g_ShotVelocity = 20; // Shot's velocity

////////////////////////////////////
// BattleField min(x,y) max(x,y) coordinates
const Pixel_t g_minXBattleField = 0;  
const Pixel_t g_minYBattleField = 0;
const Pixel_t g_maxXBattleField = 120;
const Pixel_t g_maxYBattleField = 120;

//////////////////////////////////////////////
// additional varibles
const Number_t g_maxTanksCount = 10;
const Number_t g_maxCommandsCount = 1000;
const Number_t g_maxTankNameLength = 10;

////////////////////////////////////
// Output string where no winner on BattleField
const PointName_t g_noWinnerName = "NO WINNER!";

//////////////////////////////////////
// Default input file for "test case"
const FileName_t g_defaultInputFile = "sample.in";

////////////////////////////////////
// Commands
const CommandName_t g_commandTurn  = "TURN";
const CommandName_t g_commandMove  = "MOVE";
const CommandName_t g_commandStop  = "STOP";
const CommandName_t g_commandShoot = "SHOOT";

////////////////////////////////////
// Error messages
const Error_t ERR_BAD_ANGLE				= "Bad angle in input";
const Error_t ERR_WRONG_TANKS_COUNT		= "Wrong tanks count";
const Error_t ERR_WRONG_COMMAND_COUNT	= "Wrong commands count";
const Error_t ERR_WRONG_COMMAND			= "Wrong command name";
const Error_t ERR_TANK_EXIST			= "Tank exist";
const Error_t ERR_LONG_TANK_NAME		= "Tank name to long";
const Error_t ERR_UNHANDLED_ERROR		= "Sorry, but unhandled error";
const Error_t ERR_INPUT_FILE_NOT_EXIST  = "Sorry, but input file not exist";

////////////////////////////////////////////////////////////////
// Overrides basic operators
std::istream& operator >> (std::istream&, CBattleField&);
std::istream& operator >> (std::istream&, CPoint&);
std::istream& operator >> (std::istream&, Angle_t&);

/////////////////////////////////////////////////////
// Checking point position in BattleField
bool operator == (const CPoint&, const CBattleField&);
///////////////////////////////////////////////
// Checking points for Head-a-head strikes
// For example: 
// 1. Tank head-a-head srtikes other tank
// 2. Shot strikes to other shot or tank
bool operator == (const CPoint&, const CPoint&);

//////////////////////////////////////////
//Basic point class fot BattleField
//////////////////////////////////////////
class CPoint
{
	/////////////////////////////
	// Coordinates on BattleField
	Pixel_t _x;
	Pixel_t _y;

	/////////////////////////////
	// Points's velocity
	Velocity_t _velocity;
	
	///////////////////////
	// Point's angle (facing direction)
	Angle_t _angle;
	Angle_t _newAngle;
	
	//////////////////////////////////////////////
	// This is flags answers for point's behaviour
	// Point maybe move or not, maybe live or dead
	bool _isMove;
	bool _isDead;

	///////////////////////////////////////////////////////////
	// Point's name, in future maybe using for Soldier's name :)
	PointName_t _name;
	
	/*
	  This is action using when point arrived end of BattleField
	  For example, if point is a 'Shot', then end of BattleField it explosed.
	  When point is a 'Tank', just stop it
	*/
	PointAction_t _whenEndOfBattleField;

public:
	//////////////////////
	// Default constuctor
	CPoint();

	//////////////////////////////////////////////////////////////////
	// Constructor for points with name, like Tank
	CPoint(PointName_t, Pixel_t, Pixel_t, Angle_t, Velocity_t, PointAction_t, bool);
	
	//////////////////////////////////////////////////////////////////
	// Constructor for points without name, like Shot
	CPoint(Pixel_t, Pixel_t, Angle_t, Velocity_t, PointAction_t, bool);
	~CPoint();
	
	const Pixel_t GetX() const { return _x;};
	const Pixel_t GetY() const { return _y;};
	const Angle_t GetAngle() const { return _angle;};
	const Velocity_t GetVelocity() const { return _velocity;};
	const PointAction_t GetAction() const { return _whenEndOfBattleField; }
	const PointName_t GetName() const { return _name;};

	const bool IsDead() const { return _isDead; };
	const bool IsMoved() const {return _isMove; };

	void SetNewAngle(Angle_t);
	
	void Move();
	void Go();
	void Stop();
	void Die();
};

class CBattleField
{
	/////////////////////////////
	// BattleField "global" time
	Time_t _time;
	
	/////////////////////////////
	// BattleFiled coordinates
	Pixel_t _minX;
	Pixel_t _minY;
	Pixel_t _maxX;
	Pixel_t _maxY;

	Number_t _tanksCount; //tanks startup count
	Number_t _commandsCount; // commans startup count 
	
	/////////////////////////
	// Points on BattleField
	PointsVector_t _points;
	
	/////////////////////////
	// Flag for dead points on BattleField
	// For example:
	// 1. Shot arrived end of BattleField
	// 2. Shot strikes tank 
	// 3. etc.
	bool _hasDeadPoints;

public:
	
	/////////////////////
	// Default constuctor
	CBattleField();
	
	///////////////////////////////////////////////////////////////////
	// Init constructor
	CBattleField(Pixel_t, Pixel_t, Pixel_t, Pixel_t, Number_t, Number_t);
	
	bool IsGameOver();
	bool InField(Point_t);
	
	void Battle(std::istream&);
	void MovePoints();
	void CheckForExplosion();
	void MakeTank(PointName_t, Pixel_t, Pixel_t, Angle_t);
	void MakeShot(Pixel_t, Pixel_t, Angle_t);

	/////////////////////////////////////////////////////////////////////////////
	// Static method used in std::remove_if() in CBattleField::RemoveDeadPoints()
	static bool IsRemovePoint(CPoint&);
	void RemoveDeadPoints();

	Point_t GetTankByName(PointName_t);
	PointName_t GetWinner();
};

//////////////////////////////////////////
// Basic error class
class CError
{
	Error_t _message;
public:
	CError(Error_t);
	~CError();
	const Error_t GetMessage() const { return _message; }; 
};

__ROBOCODE_END