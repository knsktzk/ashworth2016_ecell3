#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlutFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlutFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, kpg );
    PROPERTYSLOT_SET_GET( Real, kmglut );
    PROPERTYSLOT_SET_GET( Real, kcg );
    PROPERTYSLOT_SET_GET( Real, kmgpass );

  }

  GlutFluxProcess()
    :

    mtf( 1.0 ),
    kpg( 118.0 ),
    kmglut( 17000.0 ),
    kcg( 224.0 ),
    kmgpass( 17000.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, kpg );
  SIMPLE_SET_GET_METHOD( Real, kmglut );
  SIMPLE_SET_GET_METHOD( Real, kcg );
  SIMPLE_SET_GET_METHOD( Real, kmgpass );
  
  /* Process functions */
  // glut(gcbi,gci)=mtf*kpg*gCBi/(kmglut+gcbi) + mtf*kcg*(gCBi - gCi)/(kmgpass+gcbi+gci)
  double glut(double gcbi, double gci) {

    double val = mtf*kpg*gcbi/(kmglut+gcbi) + mtf*kcg*(gcbi - gci)/(kmgpass+gcbi+gci);
    return val;
  }

  virtual void initialize()
  {
    Process::initialize();
    gcb  = getVariableReference( "gcb" ).getVariable();
    gc  = getVariableReference( "gc" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real gcbi( gcb->getMolarConc() );
    Real gci( gc->getMolarConc() );

    // etc
    Real size( gc->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = glut(gcbi,gci);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* gc;

  // 定数
  Real mtf;
  Real kpg;
  Real kmglut;
  Real kcg;
  Real kmgpass;
  
};

LIBECS_DM_INIT( GlutFluxProcess, Process );
