#if !defined ( FV4_H )
#define FV4_H

namespace crunch { namespace math {

template < typename value_t >
class fv4
{
	public:
		static const fv4 Origin;

	public:
		fv4()
		{
			// empty: DO NOT INIT x/y/z here
		}
		
		core_forceinline fv4( const fv4& other ) :
			x( other.x ),
			y( other.y ),
			z( other.z ),
      w( other.w )
		{
			// empty
		}
		
		core_forceinline fv4( value_t X, value_t Y, value_t Z, value_t W ) :
			x( X ),
			y( Y ),
			z( Z ),
      w( W )
		{
			// empty
		}
		
		fv4& operator = ( const fv4& other )
		{
			if ( this == &other )
				return *this;
				
			x = other.x;
			y = other.y;
			z = other.z;
      w = other.w;
				
			return *this;
		}
		
		~fv4()
		{
			// empty
		}
		
	public:
		core_forceinline fv4& Set( value_t X, value_t Y, value_t Z, value_t W )
		{
			x = X;
			y = Y;
			z = Z;
      w = W;
			
			return *this;
		}
		
		[[nodiscard]]
    core_forceinline fv4 Normalize() const
    {
      value_t mag2 = LengthSquared();
			
      if ( mag2 > (value_t)( 0.0 ) )
      {
        value_t imag = (value_t)( 1.0 ) / sqrt( mag2 );
				
        return fv4( x * imag, y * imag, z * imag, w * imag );
      }
			
      return fv4::Origin;
    }
		
		core_forceinline value_t Length() const
		{
			return sqrt( x * x + y * y + z * z + w * w );
		}
		
		core_forceinline value_t LengthSquared() const
		{
			return ( x * x + y * y + z * z + w * w );
		}

    core_forceinline void ComponentClampMin( value_t v )
    {
      if ( x < v )
        x = v;
      if ( y < v )
        y = v;
      if ( z < v )
        z = v;
      if ( w < v )
        w = v;
    }

	  core_forceinline bool approxEquals( const fv4< value_t >& other, const value_t epsilon ) const
	  {
		  if ( fabs( x - other.x ) < epsilon )
		  {
			  if ( fabs( y - other.y ) < epsilon )
        {
				  if ( fabs( z - other.z ) < epsilon )
            return ( fabs( w - other.w ) < epsilon );
        }
		  }
		
		  return false;
	  }

		[[nodiscard]]
    core_forceinline crunch::core::String toString() const
    {
      return crunch::core::String::fmt( "< %0.6f, %0.6f, %0.6f, %0.6f >", x, y, z, w );
    }

	public:
		core_forceinline value_t& operator [] ( int i ) 
		{ 
			return (&x)[i];
		}     
		
		core_forceinline const value_t& operator [] ( int i ) const 
		{ 
			return (&x)[i]; 
		}  
		
		core_forceinline fv4 operator - () const 
		{ 
			return ( fv4( -x, -y, -z, -w ) );
		} 
		
		core_forceinline fv4 operator + () const 
		{ 
			return *this; 
		}
	
		core_forceinline fv4& operator -= ( const fv4& other )
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
      w -= other.w;
			
			return *this;
		}

		core_forceinline fv4& operator += ( const fv4& other )
		{
			x += other.x;
			y += other.y;
			z += other.z;
      w += other.w;
			
			return *this;
		}

		core_forceinline fv4& operator *= ( value_t s )
		{
			x *= s;
			y *= s;
			z *= s;
      w *= s;
			
			return *this;
		}

		core_forceinline fv4& operator /= ( value_t s )
		{
			value_t is = (value_t)( 1.0 ) / s;
			
			x *= is;
			y *= is;
			z *= is;
      w *= is;
			
			return *this;
		}
			
		core_forceinline fv4 operator - ( const fv4& other ) const
		{
			return fv4( x - other.x, y - other.y, z - other.z, w - other.w );
		}

		core_forceinline fv4 operator + ( const fv4& other ) const
		{
			return fv4( x + other.x, y + other.y, z + other.z, w + other.w );
		}
		
	public:
		value_t x;
		value_t y;
		value_t z;
    value_t w;
	
};

template < typename value_t >
core_forceinline value_t DotProd( const fv4< value_t >& a, const fv4< value_t >& b )
{
	return ( a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w );
}

template < typename value_t >
core_forceinline fv4< value_t > operator * ( value_t f, const fv4< value_t >& a ) 
{
	return ( fv4< value_t >( a.x * f, a.y * f, a.z * f, a.w * f ) );
}

template < typename value_t >
core_forceinline fv4< value_t > operator * ( const fv4< value_t >& a, value_t f ) 
{
	return ( fv4< value_t >( a.x * f, a.y * f, a.z * f, a.w * f ) );
}

template < typename value_t >
core_forceinline fv4< value_t > operator / ( const fv4< value_t >& a, value_t f ) 
{
	return ( fv4< value_t >( a.x / f, a.y / f, a.z / f, a.w / f ) );
}

typedef fv4< float > f32v4;
typedef fv4< double > f64v4;

core_forceinline f64v4 to64( f32v4 p ) { return f64v4( double( p.x ), double( p.y ), double( p.z ), double( p.w ) ); }
core_forceinline f32v4 to32( f64v4 p ) { return f32v4( float( p.x ), float( p.y ), float( p.z ), float( p.w ) ); }

} }

#endif
