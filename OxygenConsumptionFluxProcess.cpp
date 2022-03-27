#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( OxygenConsumptionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( OxygenConsumptionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, kOxUse );

  }

  OxygenConsumptionFluxProcess()
    :

    kOxUse( 0.141 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, kOxUse );
  
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
    Real equation = kOxUse * oxyi;
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* oxy;

  // 定数
  Real kOxUse;

};

LIBECS_DM_INIT( OxygenConsumptionFluxProcess, Process );
