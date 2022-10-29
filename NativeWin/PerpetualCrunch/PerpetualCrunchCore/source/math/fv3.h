#if !defined ( FV3_H )
#define FV3_H

namespace crunch { namespace math {

template < typename value_t >
class fv3
{
	public:
		static const fv3 Origin;

	public:
		fv3()
		{
			// empty: DO NOT INIT x/y/z here
		}
		
		core_forceinline fv3( const fv3& other ) :
			x( other.x ),
			y( other.y ),
			z( other.z )
		{
			// empty
		}
		
		core_forceinline fv3( value_t X, value_t Y, value_t Z ) :
			x( X ),
			y( Y ),
			z( Z )
		{
			// empty
		}
		
		fv3& operator = ( const fv3& other )
		{
			if ( this == &other )
				return *this;
				
			x = other.x;
			y = other.y;
			z = other.z;
				
			return *this;
		}
		
		~fv3()
		{
			// empty
		}
		
	public:
		core_forceinline fv3& Set( value_t X, value_t Y, value_t Z )
		{
			x = X;
			y = Y;
			z = Z;
			
			return *this;
		}
		
		[[nodiscard]]
    core_forceinline fv3 Normalize() const
    {
      value_t mag2 = LengthSquared();
			
      if ( mag2 > (value_t)( 0.0 ) )
      {
        value_t imag = (value_t)( 1.0 ) / sqrt( mag2 );
				
        return fv3( x * imag, y * imag, z * imag );
      }
			
      return fv3::Origin;
    }
		
		core_forceinline value_t Length() const
		{
			return sqrt( x * x + y * y + z * z );
		}
		
		core_forceinline value_t LengthSquared() const
		{
			return ( x * x + y * y + z * z );
		}

    core_forceinline void ComponentClampMin( value_t v )
    {
      if ( x < v )
        x = v;
      if ( y < v )
        y = v;
      if ( z < v )
        z = v;
    }

	  core_forceinline bool approxEquals( const fv3< value_t >& other, const value_t epsilon ) const
	  {
		  if ( fabs( x - other.x ) < epsilon )
		  {
			  if ( fabs( y - other.y ) < epsilon )
				  return ( fabs( z - other.z ) < epsilon );
		  }
		
		  return false;
	  }

		[[nodiscard]]
    core_forceinline crunch::core::String toString() const
    {
      return crunch::core::String::fmt( "< %0.6f, %0.6f, %0.6f >", x, y, z );
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
		
		core_forceinline fv3 operator - () const 
		{ 
			return ( fv3( -x, -y, -z ) );
		} 
		
		core_forceinline fv3 operator + () const 
		{ 
			return *this; 
		}
	
		core_forceinline fv3& operator -= ( const fv3& other )
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			
			return *this;
		}

		core_forceinline fv3& operator += ( const fv3& other )
		{
			x += other.x;
			y += other.y;
			z += other.z;
			
			return *this;
		}

		core_forceinline fv3& operator *= ( value_t s )
		{
			x *= s;
			y *= s;
			z *= s;
			
			return *this;
		}

		core_forceinline fv3& operator /= ( value_t s )
		{
			value_t is = (value_t)( 1.0 ) / s;
			
			x *= is;
			y *= is;
			z *= is;
			
			return *this;
		}
			
		core_forceinline fv3 operator - ( const fv3& other ) const
		{
			return fv3( x - other.x, y - other.y, z - other.z );
		}

		core_forceinline fv3 operator + ( const fv3& other ) const
		{
			return fv3( x + other.x, y + other.y, z + other.z );
		}
		
	public:
		value_t x;
		value_t y;
		value_t z;
	
};

template < typename value_t >
core_forceinline value_t DotProd( const fv3< value_t >& a, const fv3< value_t >& b )
{
	return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

template < typename value_t >
core_forceinline fv3< value_t > CrossProd( const fv3< value_t >& a, const fv3< value_t >& b )
{
	return fv3< value_t >( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

template < typename value_t >
core_forceinline fv3< value_t > CrossProdNormalize( const fv3< value_t >& a, const fv3< value_t >& b )
{
	return fv3< value_t >( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x ).Normalize();
}

template < typename value_t >
core_forceinline fv3< value_t > operator * ( value_t f, const fv3< value_t >& a ) 
{
	return ( fv3< value_t >( a.x * f, a.y * f, a.z * f ) );
}

template < typename value_t >
core_forceinline fv3< value_t > operator * ( const fv3< value_t >& a, value_t f ) 
{
	return ( fv3< value_t >( a.x * f, a.y * f, a.z * f ) );
}

template < typename value_t >
core_forceinline fv3< value_t > operator / ( const fv3< value_t >& a, value_t f ) 
{
	return ( fv3< value_t >( a.x / f, a.y / f, a.z / f ) );
}

typedef fv3< float > f32v3;
typedef fv3< double > f64v3;

core_forceinline f64v3 to64( f32v3 p ) { return f64v3( double( p.x ), double( p.y ), double( p.z ) ); }
core_forceinline f32v3 to32( f64v3 p ) { return f32v3( float( p.x ), float( p.y ), float( p.z ) ); }

} }

#endif
