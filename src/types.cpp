/*
	Project: ROBOCODE

	File:	 types.cpp
	
	Description:
	Implementation of basic classes

*/

__ROBOCODE_BEGIN

//////////////////////////////////////////////////
//
// Class CPoint
//
//////////////////////////////////////////////////

/////////////////////
// Default constuctor
CPoint::CPoint()
:_name(0)
,_x(0)
,_y(0)
,_angle(RIGHT)
,_velocity(0)
,_isMove(false)
,_whenEndOfBattleField(STOP)
,_isDead(true)
{};

/////////////////////////////////////////////
// Constuctor for point with name, like Tank
// Params:
// name - Point's name, like "T-34" or "Tiger" :-)
// x - coord on X axis
// y - coord on Y axis
// a - Point's angle (facing direction)
// v - Point's velocity
// action - What point do when arrived end of BattleField
// isMove - Startup moving, yes or not
CPoint::CPoint(PointName_t name, Pixel_t x, Pixel_t y, Angle_t a, Velocity_t v, PointAction_t action, bool isMove)
:_name(name)
,_x(x)
,_y(y)
,_angle(a)
,_newAngle(a)
,_velocity(v)
,_isMove(isMove)
,_whenEndOfBattleField(action)
,_isDead(false)
{}

/////////////////////////////////////////////////
// Constructor for point without name, like Shot
// Params:
// x - coord on X axis
// y - coord on Y axis
// a - Point's angle (facing direction)
// v - Point's velocity
// action - What point do when arrived end of BattleField
// isMove - Startup moving, yes or not
CPoint::CPoint(Pixel_t x, Pixel_t y, Angle_t a, Velocity_t v, PointAction_t action, bool isMove)
:_x(x)
,_y(y)
,_angle(a)
,_newAngle(a)
,_velocity(v)
,_isMove(isMove)
,_whenEndOfBattleField(action)
,_isDead(false)
{}

///////////////////////
// Disctructor
CPoint::~CPoint() {};

//////////////////////////////
// Moving point on BattleField
void CPoint::Go()
{
	if (_isMove)
	{
		switch(_angle)
		{
			case RIGHT:
			{
				_x += _velocity;
				break;
			}
			case UP:
			{
				_y += _velocity;
				break;
			}
			case LEFT:
			{
				_x -= _velocity;
				break;
			}
			case DOWN:
			{
				_y -= _velocity;
				break;
			}
		}
	}
}

///////////////////////////////////
// Recice point command "Stop"
void CPoint::Stop()
{
	_isMove = false;
	//when stoped, new facing direction
	_angle = _newAngle;
}

//////////////////////////////////
// Recive point command "Move"
void CPoint::Move()
{
	_isMove = true;
}

//////////////////////////////////
// Recive point command "Turn"
void CPoint::SetNewAngle(Angle_t a)
{
	switch ((a + _angle + 360) % 360)
	{
		case 180:
			a = LEFT;
			break;
		
		case 0:
			a = RIGHT;
			break;
		
		case 270:
			a = DOWN;
			break;
		
		case 90:
			a = UP;
			break;
	}
	_newAngle = a;
	//if stoped, new facing direction
	if (!_isMove) _angle = _newAngle;
}

/////////////////////////////
// Recive point command "Die"
void CPoint::Die()
{
	_isDead = true;
	_isMove = false;
}

///////////////////////////////////////////////////////////////////////////////
//
// CBattleFiled class
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// Default constuctor
CBattleField::CBattleField()
:_minX(0)
,_minY(0)
,_maxX(0)
,_maxY(0)
,_tanksCount(0)
,_commandsCount(0)
,_time(0)
,_hasDeadPoints(false)
{};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init constructior
// Params:
// minX - Min X axis value
// minY - Min Y axis value
// maxX - Max X axis value
// maxY - Max Y axis value
// n - Number of tanks on BattleField;
// m - Number of commands
CBattleField::CBattleField(Pixel_t minX, Pixel_t minY, Pixel_t maxX, Pixel_t maxY, Number_t n, Number_t m)
:_minX(minX)
,_minY(minY)
,_maxX(maxX)
,_maxY(maxY)
,_tanksCount(n)
,_commandsCount(m)
// Init "global" BattleField time
,_time(0)
// Init no dead point on BattleField
,_hasDeadPoints(false)
{

};

////////////////////////////////////////////////////
// Get point(Tank) by name
// Params:
// name - Point's name
//
// Return:
// Pointer ot Point or 0 
Point_t CBattleField::GetTankByName(PointName_t name)
{
	for (PointsVector_t::iterator i = _points.begin(); i != _points.end(); i++)
	{
		if ((*i).GetName() == name)
		{
			return &(*i);
		}
	}
	
	return 0;
}
/////////////////////////////////////////////////////
// Check point in BattleField or not
//
// Return:
// bool
bool CBattleField::InField(Point_t _point)
{
	
	switch (_point->GetAngle())
	{
		case UP:
			return _point->GetY() < _maxY;
			break;
		
		case DOWN:
			return _point->GetY() > _minY;
			break;
		
		case RIGHT:
		
			return _point->GetX() < _maxX;
			break;

		case LEFT:
			return _point->GetX() > _minX;
			break;
	}
	
	return false;
}
//////////////////////////////////////////////////////////////////////////////////
// Static method, using for std::remove_if() from CBattleFiled::RemoveDeadPoints()
// 
// Return:
// bool
bool CBattleField::IsRemovePoint(CPoint& p)
{
	return p.IsDead();
}

///////////////////////////////////
// Make new Tank on BattleField
//
// Params:
// name - tank name
// x - coord on X axis
// y - coord on Y axis
// angle - facing direction
void CBattleField::MakeTank(PointName_t name, Pixel_t x, Pixel_t y, Angle_t angle)
{
	//Checking for tank exist
	if (GetTankByName(name)) throw CError(ERR_TANK_EXIST);

	//Add Tank on BattleField!
	_points.push_back(CPoint(name, x, y, angle, g_TankVelocity, STOP, false));
}

/////////////////////////////////////
// Make new Shot on BattleField
//
// Params:
// x - coord on X axis
// y - coord on Y axis
// angle - facing direction
void CBattleField::MakeShot(Pixel_t x, Pixel_t y, Angle_t angle)
{
	//Add Shot on BattleField!
	_points.push_back(CPoint(x, y, angle, g_ShotVelocity, EXPLODE, true));
}

//////////////////////////////////////
// Checking battle is over or not
//
// Return:
// bool
bool CBattleField::IsGameOver()
{
	bool result = true;

	for ( PointsVector_t::iterator i = _points.begin(), end = _points.end(); i != end; ++i )
	{
		if ((*i).IsMoved())
		{
			result = false;
		}
	}
	
	return (result && _commandsCount == 0);
}

////////////////////////////////////////////////////////////////
// Main battle method, emulation of tanks battle, finding who winner
void CBattleField::Battle(std::istream& _is)
{
	Time_t time;;
	_is >> time;
		
	while (!IsGameOver())
	{
		if (!time && _commandsCount != 0)
		{
			_is >> time;	
		}
		
		////////////////////////////////////////////
		// Loop getting command at this time momment
		// Like: 
		// 25 A SHOOT
		// 25 A MOVE
		// 25 A STOP
		while (time == _time && _commandsCount != 0)
		{
			_commandsCount--;
			time = 0;
			PointName_t name;
			CommandName_t cmd;
			Angle_t angle;
			
			//Read tank name
			_is >> name;
			
			_is >> cmd;

			if (cmd == g_commandTurn)
			{
				//if command "turn" need read angle, for correct input
				_is >> angle;
			}

			Point_t tank = GetTankByName(name);

			// If tank exist recive command, else if not exist ignore command 
			if (tank != 0)
			{
				if (cmd == g_commandTurn)
				{
					//Tank recive command "TURN"
					tank->SetNewAngle(angle);
				}
				else if (cmd == g_commandStop)
				{
					//Tank recive command "STOP"
					tank->Stop();
				}
				else if (cmd == g_commandShoot)
				{
					//Tank recive command "SHOOT"
					//BOOOOOOOOOOM! :-)
					MakeShot(tank->GetX(), tank->GetY(), tank->GetAngle());
				}
				else if (cmd == g_commandMove)
				{
					//Tank recive command "MOVE"
					tank->Move();
				}
				else
				{
					//Wrong command name
					throw CError(ERR_WRONG_COMMAND);
				}
			}

			if (_commandsCount)
			{
				_is >> time;
			}
		}
		
		CheckForExplosion();
		MovePoints();
		RemoveDeadPoints();
		
		//Increment "global" time of battle on BattleField
		_time++;
	}
}

//////////////////////////////////////////////////////
// Check point for explosion, like head-a-head strikes
void CBattleField::CheckForExplosion()
{
	for(PointsVector_t::iterator i = _points.begin(), end = _points.end(); i != end; ++i)
		{
			for (PointsVector_t::iterator j = _points.begin(), endd = _points.end(); j != endd; ++j)
			{
				if (*i == *j)
				{
					_hasDeadPoints = true;
					(*i).Die();
					(*j).Die();
				}
			}
		}
}

////////////////////////////////////////////////////////
// Move live points on BattleField
void CBattleField::MovePoints()
{
	//Move points, etc. Tanks ans Shots
	for (PointsVector_t::iterator i = _points.begin(), end = _points.end(); i != end; ++i)
	{
		// if point moving and live
		if ((*i).IsMoved() && !(*i).IsDead())
		{
			if (InField(&(*i)))
			{
				(*i).Go();
			}
			else
			{
				//Point arrived end of BattleFiled 
				switch ((*i).GetAction())
				{
					case STOP:
					{
						//stop
						(*i).Stop();
						break;
					}
					case EXPLODE:
					{
						//explode
						(*i).Die();
						_hasDeadPoints = true;
						break;
					}
				}
			}
		}
	}
}

//////////////////////////////////////
// Clear BattleField from dead points
void CBattleField::RemoveDeadPoints()
{	
	//Remove dead objects
	if (_hasDeadPoints) _points.erase(std::remove_if(_points.begin(), _points.end(), CBattleField::IsRemovePoint), _points.end());
	_hasDeadPoints = false;
}

//////////////////////////////////////////
// Get winner name, when battle is finish
// 
// Return:
// PointName_t - Name or winner
PointName_t CBattleField::GetWinner()
{
	/*
	  When no winner?
	  0. Battle is finished
	  1. If no tanks on BattleField
	  2. If tanks more than 1
	  3. If 1 tank maked on BattleField at startup 
	*/
	if (_tanksCount == 0 && _commandsCount == 0)
	{
		return "";
	}
	else
	{
		if (_points.empty() || _points.size() > 1 || _tanksCount == 1)
		{
			return g_noWinnerName;
		}
		else
		{
			return _points[0].GetName();
		}
	}
}

//////////////////////////////
//
// CError class
//
///////////////////////////////

///////////////////////////////
// Constructor
CError::CError(Error_t message)
:_message(message)
{};

///////////////////////////////
// Dictructor
CError::~CError() {};


//////////////////////////////////////////////////////////////////////
// Overrides operators
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Output BattleFiled
std::ostream& operator << (std::ostream& _os, CBattleField& _bf)
{
	_os << _bf.GetWinner() << std::endl;
	return _os;
}

//////////////////////////////////////////////////////////
//Read angle for tanks position or for TURN command
std::istream& operator >> (std::istream& _is, Angle_t& _a)
{
	Number_t n;
	_is >> n;
	switch (n)
	{
		case 90:
			_a = UP;
			break;
		case 180:
			_a = LEFT;
			break;
		case 270:
			_a = DOWN;
			break;
		case 0:
			_a = RIGHT;
			break;
		default:
			throw CError(ERR_BAD_ANGLE);
			break;
	}
	
	return _is;
}

////////////////////////////////////////////////////////////////
// Read BattleField
////////////////////////////////////////////////////////////////
std::istream& operator >> (std::istream& _is, CBattleField& _bf )
{
  Number_t n = 0; _is >> n; // startup tanks count
  if (n > g_maxTanksCount) throw CError(ERR_WRONG_TANKS_COUNT);
  
  Number_t m = 0; _is >> m; // startup commans count
  if (m > g_maxCommandsCount) throw CError(ERR_WRONG_COMMAND_COUNT);

  if (n > 0 && m > 0)
  {
	  //Construct battlefield
	  _bf = CBattleField(g_minXBattleField, g_minYBattleField, g_maxXBattleField, g_maxYBattleField, n, m);
	  
	  // Makes tanks
	  while (n != 0)
	  {
		  PointName_t name;
		  Pixel_t x = 0;
		  Pixel_t y = 0;
		  Angle_t angle = RIGHT;

		  _is >> name >> x >> y >> angle;
		  if (name.size() > g_maxTankNameLength) throw CError(ERR_LONG_TANK_NAME);

		  //Construct Tank, if input is correct
		  _bf.MakeTank(name, x, y, angle);
		  --n;
	  }
	  _bf.Battle(_is);
  }
  
  return _is;
}

/////////////////////////////////////////////////////
// Compare two points
bool operator == (const CPoint& _a, const CPoint& _b)
{
	//if head-a-head strike!
	bool result =  	
	(_a.GetX() == _b.GetX()) && 
	(_a.GetY() == _b.GetY()) &&
	(_a.GetAngle() != _b.GetAngle());
	
	//if not, check strikes on point way
	if (!result)
	{
		if (_a.GetVelocity() > _b.GetVelocity())
		{
			if (_b.IsMoved())
			{
				//If point moved, check range use point velocity and face direction
				if ((_a.GetAngle() == _b.GetAngle()) || (std::abs(_a.GetAngle() - _b.GetAngle()) == LEFT))
				{
					if (_b.GetX() > _a.GetX() && _b.GetX() < (_a.GetX()+ _a.GetVelocity()) || (_b.GetY() > _a.GetY() && _b.GetY() < _a.GetY() + _a.GetVelocity()))
					{
						result = true;
					}
				}	
			}
			else
			{
				//if point stoped, check range only
				if (_b.GetX() > _a.GetX() && _b.GetX() < (_a.GetX()+ _a.GetVelocity()) || (_b.GetY() > _a.GetY() && _b.GetY() < _a.GetY() + _a.GetVelocity()))
				{
					result = true;
				}
			}
		}
	}

	return result;
}
__ROBOCODE_END