#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlucoseConsumptionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlucoseConsumptionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Kmguse );
    PROPERTYSLOT_SET_GET( Real, gluconst );

  }

  GlucoseConsumptionFluxProcess()
    :
 
    Kmguse( 1000.0 ),
    gluconst( 5.15 )
    
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Kmguse );
  SIMPLE_SET_GET_METHOD( Real, gluconst );


  virtual void initialize()
  {
    Process::initialize();
    gcb  = getVariableReference( "gcb" ).getVariable();

  }

  virtual void fire()
  {
    Real Glucose( gcb->getMolarConc() );
    Real size( gcb->getSuperSystem()->getSize() );

    Real expression = size * N_A * ( gluconst * Glucose / ( Kmguse + Glucose ) );
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;

  // 定数
  Real Kmguse;
  Real gluconst;


};

LIBECS_DM_INIT( GlucoseConsumptionFluxProcess, Process );
