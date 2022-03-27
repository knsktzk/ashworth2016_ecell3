#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( PyrKinaseFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( PyrKinaseFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_PK );
    PROPERTYSLOT_SET_GET( Real, V_PK );
    PROPERTYSLOT_SET_GET( Real, K_INS_PK );
    PROPERTYSLOT_SET_GET( Real, K_L_PK );
    PROPERTYSLOT_SET_GET( Real, kmg3pPK );
    PROPERTYSLOT_SET_GET( Real, kmadpPK );
    PROPERTYSLOT_SET_GET( Real, eff );
    PROPERTYSLOT_SET_GET( Real, kiacoaPK );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    PROPERTYSLOT_SET_GET( Real, coeff_multi );


  }

  PyrKinaseFluxProcess()
    :

    coeff_multi( 0.25 ),
    Z_PK( -0.355 ),
    V_PK( 87.0 ),
    K_INS_PK( 4.0 ),
    K_L_PK( 4.0 ),
    kmg3pPK( 150.0 ),
    kmadpPK( 240.0 ),
    eff( 0.8 ),
    kiacoaPK( 30.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_PK );
  SIMPLE_SET_GET_METHOD( Real, V_PK );
  SIMPLE_SET_GET_METHOD( Real, K_INS_PK );
  SIMPLE_SET_GET_METHOD( Real, K_L_PK );
  SIMPLE_SET_GET_METHOD( Real, kmg3pPK );
  SIMPLE_SET_GET_METHOD( Real, kmadpPK );
  SIMPLE_SET_GET_METHOD( Real, eff );
  SIMPLE_SET_GET_METHOD( Real, kiacoaPK );
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
  // PKphos(Lci,Insci,ppglui,ppinsi)=(ppinsi*res*insci+kglyrefb)/(ppglui*lci+kglyrefb)
  double PKphos(double Lci, double insci, double ppglui, double ppinsi) {

    double val = (ppinsi*res*insci+K_INS_PK)/(ppglui*Lci+K_L_PK);
    return val;

  }

  // PK(g3pi,adpi,acoai,ppvali,PKphosi)=(1+Z_PK*ppvali)*V_PK*PKphosi*hill(g3pi,kmg3pPK,1)*(adpi/(adpi+kmadpPK))*(1-eff*(acoai/(acoai+kiacoaPK)))
  double PK(double g3pi, double adpi, double acoai, double ppvali, double PKphosi) {

    double val = (1.0+Z_PK*ppvali)*V_PK*PKphosi*hill(g3pi,kmg3pPK,1.0)*(adpi/(adpi+kmadpPK))*(1.0-eff*(acoai/(acoai+kiacoaPK)));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    g3p  = getVariableReference( "g3p" ).getVariable();
    pyr  = getVariableReference( "pyr" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    acoa  = getVariableReference( "acoa" ).getVariable();

    oxy  = getVariableReference( "oxy" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR   = getVariableReference(  "IR"  ).getVariable();
    GR   = getVariableReference( "GR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real g3pi( g3p->getMolarConc() );
    Real pyri( pyr->getMolarConc() );
    Real phosi( phos->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real acoai( acoa->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( acoa->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real PKphosi = PKphos(Lci,insci,ppglui,ppinsi);
    Real equation = PK(g3pi,adpi,acoai,ppvali,PKphosi);
    Real expression = coeff_multi * sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g3p;
  Variable* pyr;
  Variable* phos;
  Variable* atp;
  Variable* adp;
  Variable* acoa;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_PK;
  Real V_PK;
  Real K_INS_PK;
  Real K_L_PK;
  Real kmg3pPK;
  Real kmadpPK;
  Real eff;
  Real kiacoaPK;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  Real coeff_multi;


};

LIBECS_DM_INIT( PyrKinaseFluxProcess, Process );
