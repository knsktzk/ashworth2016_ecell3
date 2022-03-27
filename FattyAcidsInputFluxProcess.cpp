// GlyPFK(1.1.1): G6P to GADP
#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( FattyAcidsInputFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( FattyAcidsInputFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, driveF );
    PROPERTYSLOT_SET_GET( Real, timeref );
    PROPERTYSLOT_SET_GET( Real, pw );

  }

  FattyAcidsInputFluxProcess()
    :
    mtf( 1.0 ),
    driveF( 3.5 ),
    timeref( 28800.0 ),
    pw( 6.0 )
    
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, driveF );
  SIMPLE_SET_GET_METHOD( Real, timeref );
  SIMPLE_SET_GET_METHOD( Real, pw );

  virtual void initialize()
  {
    Process::initialize();
    ffa  = getVariableReference( "ffa" ).getVariable();
    time  = getVariableReference( "time" ).getVariable();

  }

  virtual void fire()
  {
    double pow(double x, double y);
    Real FattyAcids( ffa->getMolarConc() );
    Real t( time->getValue() );
    Real size(ffa->getSuperSystem()->getSize() );

    Real expression = size * N_A * ( driveF * pow((sin( 2 * M_PI * t /(timeref) ) ), pw) );
    setFlux( expression );

  }




 protected:

  // VariableReferenceList
  Variable* ffa;
  Variable* time;

  // 定数
  Real mtf;
  Real driveF;
  Real timeref;
  Real pw;

};

LIBECS_DM_INIT( FattyAcidsInputFluxProcess, Process );
