// GlyPFK(1.1.1): G6P to GADP
#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlucoseInputFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlucoseInputFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, DriveG );
    PROPERTYSLOT_SET_GET( Real, timeref );
    PROPERTYSLOT_SET_GET( Real, pw );

  }

  GlucoseInputFluxProcess()
    :
    mtf( 1.0 ),
    DriveG( 19.275 ),
    timeref( 28800.0 ),
    pw( 6.0 )
    
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, DriveG );
  SIMPLE_SET_GET_METHOD( Real, timeref );
  SIMPLE_SET_GET_METHOD( Real, pw );

  virtual void initialize()
  {
    Process::initialize();
    gcb  = getVariableReference( "gcb" ).getVariable();
    time  = getVariableReference( "time" ).getVariable();

  }

  virtual void fire()
  {
    double pow(double x, double y);
    Real Glucose( gcb->getMolarConc() );
    Real t( time->getValue() );
    Real size(gcb->getSuperSystem()->getSize() );

    Real expression = size * N_A * ( DriveG * pow((sin(2 * M_PI * t /(timeref))), pw));
    setFlux( expression );

  }




 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* time;

  // 定数
  Real mtf;
  Real DriveG;
  Real timeref;
  Real pw;

};

LIBECS_DM_INIT( GlucoseInputFluxProcess, Process );
