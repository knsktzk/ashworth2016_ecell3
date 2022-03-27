#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlyConvFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlyConvFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, V_GLYK );
    PROPERTYSLOT_SET_GET( Real, kmglyconv );
    PROPERTYSLOT_SET_GET( Real, kmatpglki );

  }

  GlyConvFluxProcess()
    :

    V_GLYK( 5.0 ),
    kmglyconv( 41.0 ),
    kmatpglki( 15.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, V_GLYK );
  SIMPLE_SET_GET_METHOD( Real, kmglyconv );
  SIMPLE_SET_GET_METHOD( Real, kmatpglki );

  /* ZONATIONS */

  /* FUNCTIONS */

  /* Process functions */
  // glyconv(glyi,atpi) = V_GLYK*Glyi/(kmglyconv +Glyi)*(atpi/(atpi+kmatpglki))
  double glyconv(double glyi, double atpi) {

    double val = V_GLYK*glyi/(kmglyconv +glyi)*(atpi/(atpi+kmatpglki));
    return val;
  }

  virtual void initialize()
  {
    Process::initialize();
    g3p  = getVariableReference( "g3p" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    gly  = getVariableReference( "gly" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real g3pi( g3p->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real glyi( gly->getMolarConc() );

    // etc
    Real size( gly->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = glyconv(glyi,atpi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g3p;
  Variable* atp;
  Variable* adp;
  Variable* gly;

  // 定数
  Real V_GLYK;
  Real kmglyconv;
  Real kmatpglki;

};

LIBECS_DM_INIT( GlyConvFluxProcess, Process );
