#if !defined ( CRUNCH_SCRIPT_LIBRARY_TOOLS_H )
#define CRUNCH_SCRIPT_LIBRARY_TOOLS_H

///////////////////////////////////////////////////////////////////////////////////////////////////

#define _sms_p( _p ) ScriptTypes::getLibraryScriptTypeDescription( #_p )
#define _sms_r( _r ) ScriptTypes::getLibraryScriptTypeDescription( #_r )

///////////////////////////////////////////////////////////////////////////////////////////////////

#define crunch_libclass_setup( _cname, _fname, _r ) add( ( new _cname##_##_fname () )->setup( this, #_fname, { }, _sms_r( _r ) ) );
#define crunch_libclass_setup1( _cname, _fname, _p0, _r ) add( ( new _cname##_##_fname##_##_p0() )->setup( this, #_fname, { _sms_p( _p0 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup2( _cname, _fname, _p0, _p1, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup3( _cname, _fname, _p0, _p1, _p2, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup4( _cname, _fname, _p0, _p1, _p2, _p3, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup5( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ), _sms_p( _p4 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup6( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ), _sms_p( _p4 ), _sms_p( _p5 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup7( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ), _sms_p( _p4 ), _sms_p( _p5 ), _sms_p( _p6 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup8( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6##_##_p7() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ), _sms_p( _p4 ), _sms_p( _p5 ), _sms_p( _p6 ), _sms_p( _p7 ) }, _sms_r( _r ) ) );
#define crunch_libclass_setup9( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _r ) add( ( new _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6##_##_p7##_##_p8() )->setup( this, #_fname, { _sms_p( _p0 ), _sms_p( _p1 ), _sms_p( _p2 ), _sms_p( _p3 ), _sms_p( _p4 ), _sms_p( _p5 ), _sms_p( _p6 ), _sms_p( _p7 ), _sms_p( _p8 ) }, _sms_r( _r ) ) );

///////////////////////////////////////////////////////////////////////////////////////////////////

#if defined CRUNCH_LIBRARY_ENABLE_DOCUMENTATION
  #define crunch_libfunction_doc_prototype_1() virtual crunch::core::String getDocumentation() const override;
#else
  #define crunch_libfunction_doc_prototype_1()
#endif

#define crunch_libfunction_doc_prototype() crunch_libfunction_doc_prototype_1()

#define crunch_libclass_declare_p0( _cname, _fname, _r ) \
  class _cname##_##_fname : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p1( _cname, _fname, _p0, _r ) \
  class _cname##_##_fname##_##_p0 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p2( _cname, _fname, _p0, _p1, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p3( _cname, _fname, _p0, _p1, _p2, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p4( _cname, _fname, _p0, _p1, _p2, _p3, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p5( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p6( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p7( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p8( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6##_##_p7 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

#define crunch_libclass_declare_p9( _cname, _fname, _p0, _p1, _p2, _p3, _p4, _p5, _p6, _p7, _p8, _r ) \
  class _cname##_##_fname##_##_p0##_##_p1##_##_p2##_##_p3##_##_p4##_##_p5##_##_p6##_##_p7##_##_p8 : public ScriptLibraryFunction { \
    public: void entry( const ScriptExecutable *executable, char *stackParams, char *stackResult ) override; \
    crunch_libfunction_doc_prototype() \
  };

///////////////////////////////////////////////////////////////////////////////////////////////////

#define crunch_libfunction_nocallcode( _function_signature ) \
  { \
    ScriptLibraryFunction *f = (ScriptLibraryFunction*)( findFunction( _function_signature ) ); \
    if ( f != nullptr ) f->set_nocallcode( true ); \
  }

#endif
