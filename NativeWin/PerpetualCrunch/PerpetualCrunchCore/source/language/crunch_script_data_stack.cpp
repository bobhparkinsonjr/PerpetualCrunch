
#include "../crunch_global.h"

#include "crunch_script_data_stack.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace crunch { namespace language {

ScriptDataStack::ScriptDataStack( unsigned int initialSize, unsigned int expansionSize )
{
  if ( initialSize == 0 )
    initialSize = 1024 * 1024 * 4;
  
  mStack.resize( initialSize );

  mSP = &( mStack[0] );

  mExpansionSize = expansionSize;

  assert( getOffset() == 0 && "expected initial stack offset to be zero" );
}

ScriptDataStack::~ScriptDataStack()
{
  // empty
}

} }
