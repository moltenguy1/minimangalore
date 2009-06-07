#ifndef N_MATHEXT_H
#define N_MATHEXT_H
//----------------------------------------------------------------------------
/**
    @class MathExt
    @ingroup 
    @brief some generic math functions
    
    (c) 2006    je.a.le@wanadoo.fr
*/
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
namespace Math
{
class MathExt
{
    public:
        /// constructor
		MathExt() {};
        /// destructor
		virtual ~MathExt() {};

		static float clampVal( float min, float max, float val );
		static float rotateVal( float min, float max, float val );
};

//----------------------------------------------------------------------------
/**
*/
inline float MathExt::clampVal( float min, float max, float val )
{
	return( ( (val <= min) ? min : ( (val >= max) ? max : val ) ) );
}

//----------------------------------------------------------------------------
/**
*/
inline float MathExt::rotateVal( float min, float max, float val ) {
	float tmp = max - min;

	if( val > min ) {
		while( val > max ) {
			val -= tmp;
		}
	} else {
		while( val < min ) {
			val += tmp;
		}
	}
	return( val );
}

};
//----------------------------------------------------------------------------
#endif // N_MATHEXT_H

