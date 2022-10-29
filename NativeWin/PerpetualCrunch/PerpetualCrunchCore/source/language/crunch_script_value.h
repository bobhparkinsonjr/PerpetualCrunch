#if !defined ( CRUNCH_SCRIPT_VALUE_H )
#define CRUNCH_SCRIPT_VALUE_H

#include "crunch_script_types.h"

namespace crunch { namespace language {

class ScriptCompiler;
class ScriptExecutable;

class ScriptValue
{
  private:
    union ValueType
    {
      crunch_i64 i;
      crunch_u64 u;
      crunch_f64 f;
      const crunch_string *s;
      crunch_char c;
      crunch_f64 v[3];
      void *p;
    };

  public:
    ScriptValue();
    explicit ScriptValue( ScriptCompiler *compiler, ScriptTypes::ScriptType t );
    explicit ScriptValue( crunch_i64 i );
    explicit ScriptValue( crunch_u64 u );
    explicit ScriptValue( crunch_f64 f );
    explicit ScriptValue( ScriptCompiler *compiler, const crunch_string *s );
    explicit ScriptValue( char c );
    explicit ScriptValue( crunch_f64 vx, crunch_f64 vy, crunch_f64 vz );
    ~ScriptValue();

  public:
    ScriptValue( const ScriptValue& other );
    ScriptValue& operator = ( const ScriptValue& other );

  public:
    inline ScriptTypes::ScriptType getType() const { return mType; }

    crunch::core::String getSourceDescription() const;

    core_forceinline crunch_i64 getI64() const { return mValue.i; }
    core_forceinline crunch_u64 getU64() const { return mValue.u; }
    core_forceinline crunch_f64 getF64() const { return mValue.f; }
    core_forceinline const crunch_string* getString() const { return mValue.s; }
    core_forceinline crunch_char getChar() const { return mValue.c; }

    core_forceinline bool isConstantFoldType() const
    {
      return ( mType == ScriptTypes::ScriptType::TYPE_I64 || mType == ScriptTypes::ScriptType::TYPE_U64 || mType == ScriptTypes::ScriptType::TYPE_F64 );
    }

  public:
    ScriptValue& negate();
    ScriptValue& boolNot();
    ScriptValue& bitwiseComplement();

    ScriptValue& compareEE( const ScriptValue& rhs );
    ScriptValue& compareNE( const ScriptValue& rhs );
    ScriptValue& compareGT( const ScriptValue& rhs );
    ScriptValue& compareGE( const ScriptValue& rhs );
    ScriptValue& compareLT( const ScriptValue& rhs );
    ScriptValue& compareLE( const ScriptValue& rhs );

    ScriptValue& add( const ScriptValue& rhs );
    ScriptValue& subtract( const ScriptValue& rhs );
    ScriptValue& multiply( const ScriptValue& rhs );
    ScriptValue& divide( const ScriptValue& rhs );

    ScriptValue& boolAnd( const ScriptValue& rhs );
    ScriptValue& boolOr( const ScriptValue& rhs );

    ScriptValue& bitwiseAnd( const ScriptValue& rhs );
    ScriptValue& bitwiseOr( const ScriptValue& rhs );

  public:
    bool generateCode( ScriptCompiler *compiler, crunch::core::BinaryBuffer *dest, const crunch::core::WideString& sourceFilePath, unsigned int lineNumber ) const;

  public:
    bool read( const ScriptExecutable *se, crunch::core::BinaryBuffer *source );
    bool write( const ScriptExecutable *se, crunch::core::BinaryBuffer *dest ) const;

  private:
    ValueType mValue;
    ScriptTypes::ScriptType mType = ScriptTypes::ScriptType::TYPE_NONE;

};

} }

#endif
