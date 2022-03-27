#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( AtpConsumpFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( AtpConsumpFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vcorrb );
    PROPERTYSLOT_SET_GET( Real, kmatpuse );


  }

  AtpConsumpFluxProcess()
    :

    vcorrb( 200.0 ),
    kmatpuse( 2500.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vcorrb );
  SIMPLE_SET_GET_METHOD( Real, kmatpuse );

  /* ZONATIONS */

  /* FUNCTIONS */
  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))
  double hill(double x, double Xt, double n) {

    double pow(double x, double y);
    double val = pow(x, n)/(pow(Xt, n) + pow(x, n));
    return val;
  }

  /* Process functions */
  // ATPuse(atpi)=vcorrb*hill(atpi,kmatpuse,1)
  double ATPuse(double atpi) {

    double val = vcorrb*hill(atpi,kmatpuse,1.0);
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // etc
    Real size( atp->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = ATPuse(atpi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* phos;


  // Parameters
  Real vcorrb;
  Real kmatpuse;



};

LIBECS_DM_INIT( AtpConsumpFluxProcess, Process );
