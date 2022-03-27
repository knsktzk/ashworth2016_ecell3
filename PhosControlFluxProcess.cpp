#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( PhosControlFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( PhosControlFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vcorr );
    PROPERTYSLOT_SET_GET( Real, phosref );

  }

  PhosControlFluxProcess()
    :

    vcorr( 0.1 ),
    phosref( 4150.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vcorr );
  SIMPLE_SET_GET_METHOD( Real, phosref );
 

  /* ZONATIONS */

  /* FUNCTIONS */

  /* Process functions */
  // PhosCon(phosi)=vcorr*(phosi-phosref)
  double PhosCon(double phosi) {

    double val = vcorr*(phosi-phosref);
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    phos  = getVariableReference( "phos" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real phosi( phos->getMolarConc() );

    // etc
    Real size( phos->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = PhosCon(phosi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* phos;


  // 定数
  Real vcorr;
  Real phosref;



};

LIBECS_DM_INIT( PhosControlFluxProcess, Process );
