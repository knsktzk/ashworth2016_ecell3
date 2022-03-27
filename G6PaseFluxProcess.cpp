#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( G6PaseFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( G6PaseFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_G6PASE );
    PROPERTYSLOT_SET_GET( Real, V_G6PASE );
    PROPERTYSLOT_SET_GET( Real, kmg6p );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
  }

  G6PaseFluxProcess()
    :

    //coeff_multi( ),
    Z_G6PASE( 0.31 ),
    V_G6PASE( 370.0 ),
    kmg6p( 2410.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 )

  {
    // do nothing
  }


  SIMPLE_SET_GET_METHOD( Real, Z_G6PASE );
  SIMPLE_SET_GET_METHOD( Real, V_G6PASE );
  SIMPLE_SET_GET_METHOD( Real, kmg6p );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );


  /* ZONATIONS */
  // ppcalc(ox)=if(ox>46.5)then(TANH((ox-46.5)/12))else(opt1*2*TANH((ox-46.5)/24) +opt2*((ox-46.5)/10.5)  -opt3*(((46.5-ox)^1.2)/12) )
  double ppcalc(double ox){
    
    if ( ox > 46.5 ) {

      double val = tanh(( ox - 46.5 ) /12.0);
      return val;

    } else {
      
      double val = opt1 * 2.0 * tanh((ox-46.5)/24.0) + opt2 * ((ox-46.5)/10.5) - opt3 * (pow((46.5-ox), 1.2) /12.0);
      return val;

    }
  }

  /* Process functions */
  // G6Pase(g6pi,ppvali)=(1+Z_G6PASE*ppvali)*V_G6PASE*g6pi/(g6pi+kmg6p)
  double G6Pase(double g6pi, double ppvali) {
    double val = (1.0+Z_G6PASE*ppvali)*V_G6PASE*g6pi/(g6pi+kmg6p);
    return val;
  }

  virtual void initialize()
  {
    Process::initialize();
    g6p  = getVariableReference( "g6p" ).getVariable();
    gc  = getVariableReference( "gc" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real g6pi( g6p->getMolarConc() );
    Real gci( gc->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );

    // zonations
    Real ppvali = ppcalc( oxyi );

    // etc
    Real size( gc->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = G6Pase(g6pi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g6p;
  Variable* gc;
  Variable* phos;
  Variable* oxy;

  // 定数
  Real Z_G6PASE;
  Real V_G6PASE;
  Real kmg6p;
  Real opt1;
  Real opt2;
  Real opt3;

};

LIBECS_DM_INIT( G6PaseFluxProcess, Process );
