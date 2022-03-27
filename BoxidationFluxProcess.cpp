#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( BoxidationFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( BoxidationFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_BOXI );
    PROPERTYSLOT_SET_GET( Real, V_BOXI );
    PROPERTYSLOT_SET_GET( Real, K_INS_BOXI );
    PROPERTYSLOT_SET_GET( Real, K_L_BOXI );
    PROPERTYSLOT_SET_GET( Real, betaac );
    PROPERTYSLOT_SET_GET( Real, kmacin );
    PROPERTYSLOT_SET_GET( Real, kmboxi );
    PROPERTYSLOT_SET_GET( Real, kmatp );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    

  }

  BoxidationFluxProcess()
    :

    Z_BOXI( 0.23 ),
    V_BOXI( 3.3 ),
    K_INS_BOXI( 2.5 ),
    K_L_BOXI( 7.0 ),
    betaac( 1.0 ),
    kmacin( 47.8 ),
    kmboxi( 5.0 ),
    kmatp( 87.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_BOXI );
  SIMPLE_SET_GET_METHOD( Real, V_BOXI );
  SIMPLE_SET_GET_METHOD( Real, K_INS_BOXI );
  SIMPLE_SET_GET_METHOD( Real, K_L_BOXI );
  SIMPLE_SET_GET_METHOD( Real, betaac );
  SIMPLE_SET_GET_METHOD( Real, kmacin );
  SIMPLE_SET_GET_METHOD( Real, kmboxi );
  SIMPLE_SET_GET_METHOD( Real, kmatp );
  SIMPLE_SET_GET_METHOD( Real, res );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
  SIMPLE_SET_GET_METHOD( Real, ipp );
  SIMPLE_SET_GET_METHOD( Real, pglu );
  // SIMPLE_SET_GET_METHOD( Real, coeff_multi );

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
  // boxi(fai,atpi,lci,insci,acoai,ppglui,ppinsi,ppvali)=(1+Z_BOXI*ppvali)*V_BOXI*FAi*((1-ppinsi*res*insci/K_INS_BOXI+ppglui*Lci/K_L_BOXI)*(1-betaac*(acoai/(acoai+kmacin)))/(kmboxi+fai))*(atpi/(atpi+kmatp))
  double boxi(double fai, double atpi, double Lci, double insci, double acoai, double ppglui, double ppinsi, double ppvali) {

    double val = (1.0+Z_BOXI*ppvali)*V_BOXI*fai*((1.0-ppinsi*res*insci/K_INS_BOXI+ppglui*Lci/K_L_BOXI)*(1.0-betaac*(acoai/(acoai+kmacin)))/(kmboxi+fai))*(atpi/(atpi+kmatp));
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    acoa  = getVariableReference( "acoa" ).getVariable();
    amp  = getVariableReference( "amp" ).getVariable();
    fa  = getVariableReference( "fa" ).getVariable();

    oxy  = getVariableReference( "oxy" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR   = getVariableReference(  "IR"  ).getVariable();
    GR   = getVariableReference( "GR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real phosi( phos->getMolarConc() );
    Real acoai( acoa->getMolarConc() );
    Real ampi( amp->getMolarConc() );
    Real fai( fa->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( fa->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real equation = boxi(fai,atpi,Lci,insci,acoai,ppglui,ppinsi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* phos;
  Variable* acoa;
  Variable* amp;
  Variable* fa;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_BOXI;
  Real V_BOXI;
  Real K_INS_BOXI;
  Real K_L_BOXI;
  Real betaac;
  Real kmacin;
  Real kmboxi;
  Real kmatp;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  // Real coeff_multi;


};

LIBECS_DM_INIT( BoxidationFluxProcess, Process );
