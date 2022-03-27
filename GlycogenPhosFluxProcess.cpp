#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlycogenPhosFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlycogenPhosFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_GPHOS );
    PROPERTYSLOT_SET_GET( Real, K_L_GPHOS );
    PROPERTYSLOT_SET_GET( Real, K_INS_GPHOS );
    PROPERTYSLOT_SET_GET( Real, V_GPHOS );
    PROPERTYSLOT_SET_GET( Real, glys );
    PROPERTYSLOT_SET_GET( Real, kmphosgp );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );    

  }

  GlycogenPhosFluxProcess()
    :

    Z_GPHOS( 0.0 ),
    K_L_GPHOS( 0.36 ),
    K_INS_GPHOS( 0.1 ),
    V_GPHOS( 5.0 ),
    glys( 100000.0 ),
    kmphosgp( 4000.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_GPHOS );
  SIMPLE_SET_GET_METHOD( Real, K_L_GPHOS );
  SIMPLE_SET_GET_METHOD( Real, K_INS_GPHOS );
  SIMPLE_SET_GET_METHOD( Real, V_GPHOS );
  SIMPLE_SET_GET_METHOD( Real, glys );
  SIMPLE_SET_GET_METHOD( Real, kmphosgp );
  SIMPLE_SET_GET_METHOD( Real, res );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
  SIMPLE_SET_GET_METHOD( Real, ipp );
  SIMPLE_SET_GET_METHOD( Real, pglu );

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

  // H(x)=if(x>0)then(x)else(0)
  double H( double x ) {

    if ( x > 0.0 ) {

      return x;

    } else {

      return 0.0;
    }
  }

  // Aln(x)=H(ln(H(x)))
  double Aln( double x ) {

    double val = H( log( H( x ) ) );
    return val;
  }

  // trun(values)=if(values>0.05)then(values)else(0.05)
  double trun( double values ) {

    if (values>0.05)
    {
      return values;
    } else {

      return 0.05;
    }
  }

  /* Process functions */
  // GPPhos(Lci,Insci,ppglui,ppinsi)=(ppglui*lci+K_L_GPHOS)/(ppinsi*res*insci+K_INS_GPHOS)
  double GPPhos(double Lci, double insci, double ppglui, double ppinsi) {
    double val = (ppglui*Lci+K_L_GPHOS)/(ppinsi*res*insci+K_INS_GPHOS);
    return val;
  }

  // GP(Gi,phosi,ppvali,GPphosi)=(1+Z_GPHOS*ppvali)*GPphosi*V_GPHOS*hill(Gi,glys,1)*(phosi/(phosi+kmphosgp))
  double GP(double gi, double phosi, double ppvali, double GPPhosi) {
    double val = (1.0+Z_GPHOS*ppvali)*GPPhosi*V_GPHOS*hill(gi,glys,1.0)*(phosi/(phosi+kmphosgp));
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    g  = getVariableReference( "g" ).getVariable();
    g6p  = getVariableReference( "g6p" ).getVariable();
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
    Real gi( g->getMolarConc() );
    Real g6pi( g6p->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( g->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real GPPhosi = GPPhos(Lci,insci,ppglui,ppinsi);
    Real equation = GP(gi,phosi,ppvali,GPPhosi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g;
  Variable* g6p;
  Variable* phos;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_GPHOS;
  Real K_L_GPHOS;
  Real K_INS_GPHOS;
  Real V_GPHOS;
  Real glys;
  Real kmphosgp;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;

};

LIBECS_DM_INIT( GlycogenPhosFluxProcess, Process );
