#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( AcoaSynFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( AcoaSynFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_ASYN );
    PROPERTYSLOT_SET_GET( Real, V_ASYN );
    PROPERTYSLOT_SET_GET( Real, kmacoainh );
    PROPERTYSLOT_SET_GET( Real, K_INS_ASYN );
    PROPERTYSLOT_SET_GET( Real, K_L_ASYN );
    PROPERTYSLOT_SET_GET( Real, kmasyn );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    PROPERTYSLOT_SET_GET( Real, coeff_multi );


  }

  AcoaSynFluxProcess()
    :

    Z_ASYN( 0.0 ),
    V_ASYN( 14.0 ),
    kmacoainh( 35.0 ),
    K_INS_ASYN( 5.0 ),
    K_L_ASYN( 3.0 ),
    kmasyn( 540.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 ),
    coeff_multi( 0.5 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_ASYN );
  SIMPLE_SET_GET_METHOD( Real, V_ASYN );
  SIMPLE_SET_GET_METHOD( Real, kmacoainh );
  SIMPLE_SET_GET_METHOD( Real, K_INS_ASYN );
  SIMPLE_SET_GET_METHOD( Real, K_L_ASYN );
  SIMPLE_SET_GET_METHOD( Real, kmasyn );
  SIMPLE_SET_GET_METHOD( Real, res );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
  SIMPLE_SET_GET_METHOD( Real, ipp );
  SIMPLE_SET_GET_METHOD( Real, pglu );
  SIMPLE_SET_GET_METHOD( Real, coeff_multi );

  /* ZONATIONS */
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

  /* Process functions */
  // acoasyn(lacci,Lci,Insci,acoai,ppglui,ppinsi,ppvali)=(1+Z_ASYN*ppvali)*V_ASYN*lacci*(1-acoAi/(acoai+kmacoainh))*(1+ppinsi*res*insci/K_INS_ASYN-ppglui*Lci/K_L_ASYN)/(kmasyn+lacci)
  double acoasyn(double pyri, double Lci, double insci, double acoai, double ppglui, double ppinsi, double ppvali){

    double val = (1.0+Z_ASYN*ppvali)*V_ASYN*pyri*(1.0-acoai/(acoai+kmacoainh))*(1.0+ppinsi*res*insci/K_INS_ASYN-ppglui*Lci/K_L_ASYN)/(kmasyn+pyri);
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    pyr  = getVariableReference( "pyr" ).getVariable();
    atp   = getVariableReference( "atp"  ).getVariable();
    adp   = getVariableReference( "adp"  ).getVariable();
    acoa  = getVariableReference( "acoa" ).getVariable();
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
    Real pyri( pyr->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real acoai( acoa->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( pyr->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real equation = acoasyn(pyri,Lci,insci,acoai,ppglui,ppinsi,ppvali);
    Real expression = coeff_multi * sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* pyr;
  Variable* atp;
  Variable* adp;
  Variable* acoa;
  Variable* phos;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // Parameters
  Real Z_ASYN;
  Real V_ASYN;
  Real kmacoainh;
  Real K_INS_ASYN;
  Real K_L_ASYN;
  Real kmasyn;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  Real coeff_multi;


};

LIBECS_DM_INIT( AcoaSynFluxProcess, Process );
