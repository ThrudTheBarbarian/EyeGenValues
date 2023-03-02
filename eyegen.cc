#include <cmath>
#include "eyegen.h"

/*****************************************************************************\
|* Constructor
\*****************************************************************************/
EyeGen::EyeGen(QObject *parent)
	   :QObject{parent}
	   ,_key(0)
	   ,_generation(0)
	   ,_focalLength(300)
	   ,_totalGenerations(0)
	   ,_bestChild(0)
	{
	_initialise();
	}

/*****************************************************************************\
|* Initialise everything
\*****************************************************************************/
void EyeGen::initialise(void)
	{
	_initialise();
	emit drawScreen(this);
	}

/*****************************************************************************\
|* Do a run through
\*****************************************************************************/
int EyeGen::evolveLens(bool &done)
	{
	double totOffsets = 0.0;

	for (int i=0; i<=NUM_COLUMNS-1; i++)
		{
		_offset[1][i] = i * CELLW;
		totOffsets += _offset[1][i];
		}

	if (_generation < MAX_GENERATION)
		{
		emit drawScreen(this);

		for (int column=1; column <= NUM_COLUMNS; column++)
			_childGeometry(column);

		_bestChild = _calcEfficiency(totOffsets);

		for (int i=1; i <=NUM_COLUMNS; i++)
			_parent[_generation+1][i] = _numCells[_bestChild][i];

		if (_efficiency[_generation] == _efficiency[_generation-2])
			{
			emit drawScreen(this);
			done = true;
			_generation --;
			}
		else
			_generation ++;
		}
	else
		done = false;


	return _generation;
	}

#pragma mark - Private methods

/*****************************************************************************\
|* Initialise the model
\*****************************************************************************/
void EyeGen::_initialise(void)
	{
	_generation = 1;
	_totalGenerations = 0;
	_bestChild = 0;

	for (int i=0; i<MAX_GENERATION; i++)
		{
		for (int j=0; j<=NUM_COLUMNS; j++)
			{
			_parent[i][j] = 0;
			_numCells[i][j] = 0;
			_offset[i][j] = 0.0;
			}
		_efficiency[i] = 0.0;
		}

	for (int i=0; i<=NUM_CHILDREN; i++)
		{
		_childEfficiency[i] = 0.0;
		_childOffset[i] = 0.0;
		}

	for (int i=0; i<=NUM_COLUMNS; i++)
		{
		_parent[1][i] = _focalLength;
		_offset[1][i] = i* CELLW;
		}

	}

/*****************************************************************************\
|* Figure out the child geometry for a column
\*****************************************************************************/
void EyeGen::_childGeometry(int column)
	{
	for (int i=0; i<=NUM_COLUMNS; i++)
		{
		_numCells[column*2-1][i]	= _parent[_generation][i];
		_numCells[column*2][i]		= _parent[_generation][i];
		}
	_numCells[column*2-1][column]	= _parent[_generation][column] -1;
	_numCells[column*2  ][column]	= _parent[_generation][column] +1;
	}

/*****************************************************************************\
|* Figure out the child geometry for a column
\*****************************************************************************/
int EyeGen::_calcEfficiency(double totOffsets)
	{
	double maxEfficiency		= 0.0;

	for (int child=1; child <= NUM_CHILDREN; child++)
		{
		_childEfficiency[child] = 0.0;

		for (int ray=1; ray <= NUM_COLUMNS-1; ray ++)
			{
			double dh			= (_numCells[child][ray] - _numCells[child][ray+1])
								* CELLD;
			double theta1		= atan(dh / CELLW);
			double theta2		= asin(sin(theta1)/RI);
			double theta3		= theta1 - theta2;
			double length		= _focalLength + _numCells[child][ray+1]*CELLD
								+ 0.5*dh;
			double dv			= length * tan(theta3);
			double R			= ray * CELLW - dv;
			_childOffset[ray]	= R;
			if (R<0)
				R = -R;
			double trueOffset	= R * cos(theta3);
			_childEfficiency[child] += trueOffset;
			}

		_childEfficiency[child] = 100 - 100 * (_childEfficiency[child] / totOffsets);
		if (_childEfficiency[child] > maxEfficiency)
			{
			maxEfficiency = _childEfficiency[child];
			_bestChild    = child;
			for (int i=1; i<NUM_COLUMNS-1; i++)
				_offset[_generation+1][i] = _childOffset[i];
			}
		}

	_efficiency[_generation] = _childEfficiency[_bestChild];
	return _bestChild;
	}
