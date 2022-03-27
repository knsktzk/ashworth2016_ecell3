#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( InsulinConsumptionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( InsulinConsumptionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, tauI );
    PROPERTYSLOT_SET_GET( Real, MI );
    PROPERTYSLOT_SET_GET( Real, mins );
    

  }

  InsulinConsumptionFluxProcess()
    :

    tauI( 0.075 ),
    MI( 4.0 ),
    mins( 60.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, tauI );
  SIMPLE_SET_GET_METHOD( Real, MI );
  SIMPLE_SET_GET_METHOD( Real, mins );
  
  /* ZONATIONS */
  
  /* Process functions */

  virtual void initialize()
  {
    Process::initialize();
    ins    = getVariableReference(  "ins"  ).getVariable();

  }

  virtual void fire()
  {

   
    Real insci( ins->getMolarConc() );

    // etc
    Real size( ins->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = (1/(tauI*mins))*(insci/MI);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList

  Variable* ins;

  // 定数
  Real tauI;
  Real MI;
  Real mins;

};

LIBECS_DM_INIT( InsulinConsumptionFluxProcess, Process );
