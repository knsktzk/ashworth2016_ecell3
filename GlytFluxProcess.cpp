#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlytFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlytFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, vglyt );
    PROPERTYSLOT_SET_GET( Real, kmglyt );

  }

  GlytFluxProcess()
    :

    mtf( 1.0 ),
    vglyt( 100.0 ),
    kmglyt( 270.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, vglyt );
  SIMPLE_SET_GET_METHOD( Real, kmglyt );

  /* ZONATIONS */

  /* FUNCTIONS */

  /* Process functions */
  // glyt(glybi,glyi)=mtf*vglyt*(glybi-glyi)/(kmglyt+glybi+glyi)
  Real glyt(Real glybi, Real glyi) {

    Real val = mtf*vglyt*(glybi-glyi)/(kmglyt+glybi+glyi);
    return val;

  }

  virtual void initialize()
  {
    Process::initialize();
    glyb  = getVariableReference( "glyb" ).getVariable();
    gly  = getVariableReference( "gly" ).getVariable();


  }

  virtual void fire()
  {
    // Variables
    Real glybi( glyb->getMolarConc() );
    Real glyi( gly->getMolarConc() );

    // etc
    Real size( gly->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = glyt(glybi,glyi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* glyb;
  Variable* gly;


  //
  Real mtf;
  Real vglyt;
  Real kmglyt;

};

LIBECS_DM_INIT( GlytFluxProcess, Process );
