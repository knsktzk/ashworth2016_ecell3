#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( OxygenInputFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( OxygenInputFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, OxyIn );

  }

  OxygenInputFluxProcess()
    :

    OxyIn( 6.75 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, OxyIn );

  virtual void initialize()
  {
    Process::initialize();
    oxy  = getVariableReference( "oxy" ).getVariable();

  }

  virtual void fire()
  {
    // oxygen
    Real oxyi( oxy->getMolarConc() );

    // etc
    Real size( oxy->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = OxyIn / 5;
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* oxy;


  // 定数
  Real OxyIn;

};

LIBECS_DM_INIT( OxygenInputFluxProcess, Process );
