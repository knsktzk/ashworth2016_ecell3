#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlucagonConsumptionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlucagonConsumptionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, tauL );
    PROPERTYSLOT_SET_GET( Real, ML );
    PROPERTYSLOT_SET_GET( Real, mins );
    

  }

  GlucagonConsumptionFluxProcess()
    :

    tauL( 0.036 ),
    ML( 3.0 ),
    mins( 60.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, tauL );
  SIMPLE_SET_GET_METHOD( Real, ML );
  SIMPLE_SET_GET_METHOD( Real, mins );
  
  /* ZONATIONS */
  
  /* Process functions */

  virtual void initialize()
  {
    Process::initialize();
    L    = getVariableReference(  "L"  ).getVariable();

  }

  virtual void fire()
  {

    Real Lci( L->getMolarConc() );

    // etc
    Real size( L->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = (1/(tauL*mins))*(Lci/ML);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList

  Variable* L;

  // 定数
  Real tauL;
  Real ML;
  Real mins;

};

LIBECS_DM_INIT( GlucagonConsumptionFluxProcess, Process );
