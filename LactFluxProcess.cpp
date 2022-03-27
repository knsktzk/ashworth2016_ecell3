#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( LactFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( LactFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, vMaxLac );
    PROPERTYSLOT_SET_GET( Real, KMLac );

  }

  LactFluxProcess()
    :

    mtf( 1.0 ),
    vMaxLac( 200.0 ),
    KMLac( 1200.0 )
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, vMaxLac );
  SIMPLE_SET_GET_METHOD( Real, KMLac );

  /* ZONATIONS */

  /* FUNCTIONS */
  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))


  /* Process functions */
  // lact(Laci,Lacci)=mtf*((vMaxLac * (Laci - LacCi))/( KMLac + Laci + LacCi))
  double lact(double laci, double pyri) {

    double val = mtf*((vMaxLac * (laci - pyri))/( KMLac + laci + pyri));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    lac  = getVariableReference( "lac" ).getVariable();
    pyr  = getVariableReference( "pyr" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real laci( lac->getMolarConc() );
    Real pyri( pyr->getMolarConc() );

    // etc
    Real size( pyr->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = lact(laci, pyri);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* lac;
  Variable* pyr;


  // 定数
  Real mtf;
  Real vMaxLac;
  Real KMLac;

};

LIBECS_DM_INIT( LactFluxProcess, Process );
