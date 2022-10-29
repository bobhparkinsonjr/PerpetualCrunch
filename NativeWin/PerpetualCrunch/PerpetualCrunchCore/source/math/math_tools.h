#if !defined ( MATH_TOOLS_H )
#define MATH_TOOLS_H

#define MathTools_PI_f32 3.141592653589793f
#define MathTools_PI_f64 3.141592653589793

#define MathTools_TO_DEG_f32( _rad ) ( (_rad) * 180.0f / MathTools_PI_f32 )
#define MathTools_TO_RAD_f32( _deg ) ( (_deg) * MathTools_PI_f32 / 180.0f )

#define MathTools_TO_DEG_f64( _rad ) ( (_rad) * 180.0 / MathTools_PI_f64 )
#define MathTools_TO_RAD_f64( _deg ) ( (_deg) * MathTools_PI_f64 / 180.0 )

namespace crunch { namespace math {

namespace MathTools
{
	inline float min_f32( float a, float b )
	{
		return ( ( a < b ) ? a : b );
	}

	inline float max_f32( float a, float b )
	{
		return ( ( a > b ) ? a : b );
	}

  // v clamped in the inclusive range [min,max]
	inline float clamp_f32( float v, float min, float max )
	{
		if ( v <= min )
			return min;
			
		if ( v >= max )
			return max;
			
		return v;
	}
	
	inline double clamp_f64( double v, double min, double max )
	{
		if ( v <= min )
			return min;
			
		if ( v >= max )
			return max;
			
		return v;
	}

	inline int clamp_i32( int v, int min, int max )
	{
		if ( v < min )
			return min;
			
		if ( v > max )
			return max;
			
		return v;
	}
}

} }

#endif
