#ifndef EYEGEN_H
#define EYEGEN_H

#include <QObject>

#include "properties.h"

class EyeGen : public QObject
	{
	Q_OBJECT

	public:

		/*********************************************************************\
		|* Menu options
		\*********************************************************************/
		static const int				ENABLED				= 1;
		static const int				DISABLED			= 0;

		/*********************************************************************\
		|* Focal length
		\*********************************************************************/
		static const int				MIN_FOCAL_LENGTH	= 180;
		static const int				MAX_FOCAL_LENGTH	= 450;

		/*********************************************************************\
		|* Cell depth and width
		\*********************************************************************/
		static const int				CELLD				= 1;
		static const int				CELLW				= 20;

		/*********************************************************************\
		|* Columns should be +ve odd
		\*********************************************************************/
		static const int				NUM_COLUMNS			= 7;
		static const int				NUM_CHILDREN		= NUM_COLUMNS * 2;
		static const int				MAX_GENERATION		= 300;

		/*********************************************************************\
		|* Refractive index between air and cell (water)
		\*********************************************************************/
		constexpr static const float	RI					= 1.33f;

	protected:
		int _parent[MAX_GENERATION][NUM_COLUMNS+1];
		int _numCells[NUM_CHILDREN+1][NUM_COLUMNS+1];

		int _key;

		double _efficiency[MAX_GENERATION];
		double _childEfficiency[NUM_CHILDREN+1];
		double _offset[MAX_GENERATION][NUM_COLUMNS+1];
		double _childOffset[NUM_CHILDREN+1];

		GETSET(int, generation, Generation);				// Current gen
		GETSET(int, focalLength, FocalLength);				// Focal length
		GETSET(int, totalGenerations, TotalGenerations);	// total gens
		GETSET(int, bestChild, BestChild);					// Best child

	public:
		explicit EyeGen(QObject *parent = nullptr);

		void initialise(void);

		inline int parent(int idx) { return _parent[_generation][idx]; }
		inline double offset(int idx) { return _offset[_generation][idx]; }
		inline double efficiency(int idx) { return _efficiency[idx]; }

	signals:
		void drawScreen(EyeGen *state);

	public slots:
		int evolveLens(bool &done);

	private:
		void _initialise(void);
		void _childGeometry(int column);
		int _calcEfficiency(double totOffsets);
	};

#endif // EYEGEN_H
