#if !defined ( CRUNCH_SCRIPT_INSTRUCTION_H )
#define CRUNCH_SCRIPT_INSTRUCTION_H

#include "../core/core_binary_buffer.h"

namespace crunch { namespace language {

#define _script_instruction_params ScriptCompiler *compiler, crunch::core::BinaryBuffer *dest, const crunch::core::WideString& sourceFilePath, unsigned int lineNumber

class ScriptCompiler;

class ScriptInstruction
{
  public:
    enum class Opcode
    {
      OP_NONE = -1,

      #define ROW( _enum_name ) OP_##_enum_name ,
      #include "crunch_script_opcodes.table"
      #undef ROW

      OP_TOTAL
    };

  public:
    static bool write_POP_STORE_LOCAL( _script_instruction_params, uint32_t dataOffset, uint64_t dataSize, const crunch::core::String& comment = "" );

    static bool write_POP( _script_instruction_params, uint32_t scopeStackSize, const crunch::core::String& comment = "" );

    static bool write_PUSH_FETCH_LOCAL( _script_instruction_params, uint32_t dataOffset, uint64_t dataSize, const crunch::core::String& comment = "" );

    // this will return false if PUSH_FETCH_LOCAL wasn't the last written instruction
    static bool update_PUSH_FETCH_LOCAL_member( _script_instruction_params, uint32_t memberOffset, uint64_t memberSize );

    static bool write_PUSH_FETCH_GLIB_CLS( _script_instruction_params, const crunch::core::String *className, const crunch::core::String *instanceName, const crunch::core::String& comment = "" );
    static bool write_FETCH_TEMP_MEMBER( _script_instruction_params, uint32_t classSize, uint32_t memberSize, uint32_t memberOffset, const crunch::core::String& comment = "" );

    static bool write_PUSH_STRING( _script_instruction_params, const crunch::core::String *s, const crunch::core::String& comment = "" );
    static bool write_PUSH_I64( _script_instruction_params, int64_t i, const crunch::core::String& comment = "" );
    static bool write_PUSH_U64( _script_instruction_params, uint64_t u, const crunch::core::String& comment = "" );
    static bool write_PUSH_F64( _script_instruction_params, double f, const crunch::core::String& comment = "" );
    static bool write_PUSH_NULL_PTR( _script_instruction_params );

    static bool write_RETURN( _script_instruction_params, const crunch::core::String& comment = "" );
    static bool write_RETURN2( _script_instruction_params, uint32_t scopeStackSize, uint64_t returnTypeStackSize, const crunch::core::String& comment = "" );

    static bool write_MINUS_I64( _script_instruction_params );
    static bool write_MINUS_F64( _script_instruction_params );

    static bool write_BOOL_NOT_I64( _script_instruction_params );

    static bool write_BIT_COMP_I64( _script_instruction_params );
    static bool write_BIT_COMP_U64( _script_instruction_params );

    static bool write_EE_STRING( _script_instruction_params );
    static bool write_EE_I64( _script_instruction_params );
    static bool write_EE_U64( _script_instruction_params );
    static bool write_EE_F64( _script_instruction_params );

    static bool write_NE_STRING( _script_instruction_params );
    static bool write_NE_I64( _script_instruction_params );
    static bool write_NE_U64( _script_instruction_params );
    static bool write_NE_F64( _script_instruction_params );

    static bool write_GT_STRING( _script_instruction_params );
    static bool write_GT_I64( _script_instruction_params );
    static bool write_GT_U64( _script_instruction_params );
    static bool write_GT_F64( _script_instruction_params );

    static bool write_GE_STRING( _script_instruction_params );
    static bool write_GE_I64( _script_instruction_params );
    static bool write_GE_U64( _script_instruction_params );
    static bool write_GE_F64( _script_instruction_params );

    static bool write_LT_STRING( _script_instruction_params );
    static bool write_LT_I64( _script_instruction_params );
    static bool write_LT_U64( _script_instruction_params );
    static bool write_LT_F64( _script_instruction_params );

    static bool write_LE_STRING( _script_instruction_params );
    static bool write_LE_I64( _script_instruction_params );
    static bool write_LE_U64( _script_instruction_params );
    static bool write_LE_F64( _script_instruction_params );

    static bool write_PLUS_I64( _script_instruction_params );
    static bool write_PLUS_U64( _script_instruction_params );
    static bool write_PLUS_F64( _script_instruction_params );

    static bool write_SUBTRACT_I64( _script_instruction_params );
    static bool write_SUBTRACT_U64( _script_instruction_params );
    static bool write_SUBTRACT_F64( _script_instruction_params );

    static bool write_MULTIPLY_I64( _script_instruction_params );
    static bool write_MULTIPLY_U64( _script_instruction_params );
    static bool write_MULTIPLY_F64( _script_instruction_params );

    static bool write_DIVIDE_I64( _script_instruction_params );
    static bool write_DIVIDE_U64( _script_instruction_params );
    static bool write_DIVIDE_F64( _script_instruction_params );

    static bool write_BOOL_AND_I64( _script_instruction_params );
    static bool write_BOOL_AND_U64( _script_instruction_params );
    static bool write_BOOL_AND_F64( _script_instruction_params );

    static bool write_BOOL_OR_I64( _script_instruction_params );
    static bool write_BOOL_OR_U64( _script_instruction_params );
    static bool write_BOOL_OR_F64( _script_instruction_params );

    static bool write_BIT_AND_I64( _script_instruction_params );
    static bool write_BIT_AND_U64( _script_instruction_params );

    static bool write_BIT_OR_I64( _script_instruction_params );
    static bool write_BIT_OR_U64( _script_instruction_params );

    static bool write_STRING_TO_I64( _script_instruction_params );
    static bool write_U64_TO_I64( _script_instruction_params );
    static bool write_F64_TO_I64( _script_instruction_params );

    static bool write_STRING_TO_U64( _script_instruction_params );
    static bool write_I64_TO_U64( _script_instruction_params );
    static bool write_F64_TO_U64( _script_instruction_params );

    static bool write_STRING_TO_F64( _script_instruction_params );
    static bool write_I64_TO_F64( _script_instruction_params );
    static bool write_U64_TO_F64( _script_instruction_params );

    static bool write_LIB_FUNC_CALL( _script_instruction_params, const crunch::core::String *className, const crunch::core::String *functionSignature, const crunch::core::String& comment = "" );
    static bool write_LIB_GFUNC_CALL( _script_instruction_params, const crunch::core::String *functionSignature, const crunch::core::String& comment = "" );
    static bool write_SO_FUNC_CALL( _script_instruction_params );
    static bool write_SO_GFUNC_CALL( _script_instruction_params, uint32_t offset, uint64_t paramsStackSize, const crunch::core::String& comment = "" );

    static bool write_BRANCH( _script_instruction_params, const crunch::core::String& comment );
    static bool update_BRANCH( ScriptCompiler *compiler, char *pc, uint32_t falseOffset );

    static bool write_BRANCHF( _script_instruction_params, const crunch::core::String& comment );
    static bool update_BRANCHF( ScriptCompiler *compiler, char *pc, uint32_t endIfOffset );

    static bool write_JUMP( _script_instruction_params, const crunch::core::String& comment );
    static bool update_JUMP( ScriptCompiler *compiler, char *pc, uint32_t offset );

    static bool write_SWITCH_JUMP_TABLE( _script_instruction_params, int64_t arrayBaseValue, int32_t jumpTableSize, const crunch::core::String& comment );
    static bool update_SWITCH_JUMP_TABLE( ScriptCompiler *compiler, char *pc, uint64_t defaultCaseOffset, uint64_t *jumpTable );

  public:
    static crunch::core::String getOpcodeName( Opcode op );

  private:
    static bool writeInstruction( crunch::core::BinaryBuffer *dest, Opcode op, uint32_t arg = 0 );

};

} }

#endif
