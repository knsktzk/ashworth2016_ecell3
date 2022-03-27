#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( AdenoKinaseFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( AdenoKinaseFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vAK );
    PROPERTYSLOT_SET_GET( Real, kmAtpAk );
    PROPERTYSLOT_SET_GET( Real, kmAdpAk );
    PROPERTYSLOT_SET_GET( Real, kmAmpAk );
    
  }

  AdenoKinaseFluxProcess()
    :

    vAK( 100.0 ),
    kmAtpAk( 90.0 ),
    kmAdpAk( 110.0 ),
    kmAmpAk( 80.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vAK );
  SIMPLE_SET_GET_METHOD( Real, kmAtpAk );
  SIMPLE_SET_GET_METHOD( Real, kmAdpAk );
  SIMPLE_SET_GET_METHOD( Real, kmAmpAk );


  /* ZONATIONS */

  /* FUNCTIONS */

  /* Process functions */
  // Adenosine kinase
  double AK(double atpi, double adpi, double ampi) {

    double val = ((vAK/(kmAtpAk*kmAmpAk))*((atpi*ampi)-pow((adpi),2)))   /(   ((1.0+(atpi/kmAtpAk))*(1.0+ampi/kmAmpAk)) + 2.0*adpi/kmAdpAk +pow(adpi,2)/pow((kmAdpAk),2));
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    amp  = getVariableReference( "amp" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real ampi( amp->getMolarConc() );

    // etc
    Real size( atp->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = AK(atpi,adpi,ampi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* amp;


  // Parameters
  Real vAK;
  Real kmAtpAk;
  Real kmAdpAk;
  Real kmAmpAk;

};

LIBECS_DM_INIT( AdenoKinaseFluxProcess, Process );
