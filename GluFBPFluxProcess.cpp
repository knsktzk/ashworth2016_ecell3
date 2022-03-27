#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GluFBPFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GluFBPFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_FBP );
    PROPERTYSLOT_SET_GET( Real, V_FBP );
    PROPERTYSLOT_SET_GET( Real, K_INS_FBP );
    PROPERTYSLOT_SET_GET( Real, K_L_FBP );
    PROPERTYSLOT_SET_GET( Real, kmFBP );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    PROPERTYSLOT_SET_GET( Real, coeff_multi );
    

  }

  GluFBPFluxProcess()
    :

    coeff_multi( 0.5 ),
    Z_FBP( 0.273 ),
    V_FBP( 68.0 ),
    K_INS_FBP( 10.0 ),
    K_L_FBP( 10.0 ),
    kmFBP( 250.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_FBP );
  SIMPLE_SET_GET_METHOD( Real, V_FBP );
  SIMPLE_SET_GET_METHOD( Real, K_INS_FBP );
  SIMPLE_SET_GET_METHOD( Real, K_L_FBP );
  SIMPLE_SET_GET_METHOD( Real, kmFBP );
  SIMPLE_SET_GET_METHOD( Real, res );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
  SIMPLE_SET_GET_METHOD( Real, ipp );
  SIMPLE_SET_GET_METHOD( Real, pglu );
  SIMPLE_SET_GET_METHOD( Real, coeff_multi );

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
  // FBPPhos(Lci,Insci,ppglui,ppinsi)=(ppglui*lci+kgluref)/(ppinsi*res*insci+kgluref)
  double FBPPhos(double Lci, double insci, double ppglui, double ppinsi) {

    double val = (ppglui*Lci+K_L_FBP)/(ppinsi*res*insci+K_INS_FBP);
    return val;
  }

  // FBP(g3pi,ppvali,FBPphosi)=(1+Z_FBP*ppvali)*V_FBP*FBPphosi*hill(g3pi,kmFBP,1)
  double FBP(double g3pi, double ppvali, double FBPphosi) {

    double val = (1.0 +Z_FBP*ppvali)*V_FBP*FBPphosi*hill(g3pi,kmFBP,1.0);
    return val;
  }

  virtual void initialize()
  {
    Process::initialize();
    g6p  = getVariableReference( "g6p" ).getVariable();
    g3p  = getVariableReference( "g3p" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    
    oxy  = getVariableReference( "oxy" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR   = getVariableReference(  "IR"  ).getVariable();
    GR   = getVariableReference( "GR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real g6pi( g6p->getMolarConc() );
    Real g3pi( g3p->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( g6p->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real FBPphosi = FBPPhos(Lci,insci,ppglui,ppinsi);
    Real equation = FBP(g3pi,ppvali,FBPphosi);
    Real expression = coeff_multi * sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g6p;
  Variable* g3p;
  Variable* phos;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_FBP;
  Real V_FBP;
  Real K_INS_FBP;
  Real K_L_FBP;
  Real kmFBP;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  Real coeff_multi;


};

LIBECS_DM_INIT( GluFBPFluxProcess, Process );
