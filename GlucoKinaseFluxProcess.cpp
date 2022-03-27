#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlucoKinaseFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlucoKinaseFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_GK );
    PROPERTYSLOT_SET_GET( Real, V_GK );
    PROPERTYSLOT_SET_GET( Real, ngfree );
    PROPERTYSLOT_SET_GET( Real, kmgfree );
    PROPERTYSLOT_SET_GET( Real, nglu );
    PROPERTYSLOT_SET_GET( Real, kmglu );
    PROPERTYSLOT_SET_GET( Real, kmatpgk );
    PROPERTYSLOT_SET_GET( Real, kig6pGK );
    PROPERTYSLOT_SET_GET( Real, npseudo );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );

  }

  GlucoKinaseFluxProcess()
    :
  
    Z_GK( -0.429 ),
    V_GK( 132.16 ),
    ngfree( 2.0 ),
    kmgfree( 15000.0 ),
    nglu( 1.4 ),
    kmglu( 7500.0 ),
    kmatpgk( 240.0 ),
    kig6pGK( 240.0 ),
    npseudo( 4.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_GK );
  SIMPLE_SET_GET_METHOD( Real, V_GK );
  SIMPLE_SET_GET_METHOD( Real, ngfree );
  SIMPLE_SET_GET_METHOD( Real, kmgfree );
  SIMPLE_SET_GET_METHOD( Real, nglu );
  SIMPLE_SET_GET_METHOD( Real, kmglu );
  SIMPLE_SET_GET_METHOD( Real, kmatpgk );
  SIMPLE_SET_GET_METHOD( Real, kig6pGK );
  SIMPLE_SET_GET_METHOD( Real, npseudo );
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

  /* FUNCTIONS */
  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))
  double hill(double x, double Xt, double n) {

    double pow(double x, double y);
    double val = pow(x, n)/(pow(Xt, n) + pow(x, n));
    return val;
  }


  /* Process functions */
  // GK(gci,atpi,g6pi,ppvali) =(1+Z_GK*ppvali)*((V_GK*(gCi^ngfree))/((gCi^ngfree)+(kmgfree^ngfree))) * ((gCi^nglu)/((gCi^nglu)+((kmglu)^nglu))) *(atpi/(atpi+kmatpgk))*(1-hill(g6pi,kig6pGK,npseudo))
  double GK(double gci, double atpi,double g6pi, double ppvali) {

    double val = (1.0+Z_GK*ppvali)*((V_GK*pow(gci,ngfree))/(pow(gci,ngfree)+pow(kmgfree,ngfree))) * (pow(gci,nglu)/(pow(gci,nglu)+pow((kmglu),nglu))) *(atpi/(atpi+kmatpgk))*(1-hill(g6pi,kig6pGK,npseudo));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    gc   = getVariableReference( "gc" ).getVariable();
    g6p  = getVariableReference( "g6p" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();

    oxy  = getVariableReference( "oxy" ).getVariable();


  }

  virtual void fire()
  {
    // Variables
    Real gci( gc->getMolarConc() );
    Real g6pi( g6p->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );

    // etc
    Real size( gc->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // zonations
    Real ppvali = ppcalc( oxyi );

    // Process
    Real equation = GK(gci,atpi,g6pi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gc;
  Variable* g6p;
  Variable* atp;
  Variable* adp;
  Variable* oxy;

  // 定数
  Real Z_GK;
  Real V_GK;
  Real ngfree;
  Real kmgfree;
  Real nglu;
  Real kmglu;
  Real kmatpgk;
  Real kig6pGK;
  Real npseudo;
  Real opt1;
  Real opt2;
  Real opt3;



};

LIBECS_DM_INIT( GlucoKinaseFluxProcess, Process );
