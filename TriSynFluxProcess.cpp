#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( TriSynFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( TriSynFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_TRISYN );
    PROPERTYSLOT_SET_GET( Real, V_TRISYN );
    PROPERTYSLOT_SET_GET( Real, K_INS_TRISYN );
    PROPERTYSLOT_SET_GET( Real, K_L_TRISYN );
    PROPERTYSLOT_SET_GET( Real, kmtrisyng );
    PROPERTYSLOT_SET_GET( Real, kmtrisynf );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    

  }

  TriSynFluxProcess()
    :
 
    Z_TRISYN( 0.0 ),
    V_TRISYN( 9.0 ),
    K_INS_TRISYN( 4.0 ),
    K_L_TRISYN( 4.0 ),
    kmtrisyng( 460.0 ),
    kmtrisynf( 645.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_TRISYN );
  SIMPLE_SET_GET_METHOD( Real, V_TRISYN );
  SIMPLE_SET_GET_METHOD( Real, K_INS_TRISYN );
  SIMPLE_SET_GET_METHOD( Real, K_L_TRISYN );
  SIMPLE_SET_GET_METHOD( Real, kmtrisyng );
  SIMPLE_SET_GET_METHOD( Real, kmtrisynf );
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
  // trisyn(FAi,G3Pi,Lci,Insci,ppglui,ppinsi,ppvali)=(1+Z_TRISYN*ppvali)*V_TRISYN*(g3pi*FAi*trun((1+ppinsi*res*insci/K_INS_TRISYN-ppglui*Lci/K_L_TRISYN)))/((kmtrisyng+g3pi)*(kmtrisynf+FAi))
  double trisyn(double fai, double g3pi, double Lci, double insci, double ppglui, double ppinsi, double ppvali) {

    double val = (1.0+Z_TRISYN*ppvali)*V_TRISYN*(g3pi*fai*trun((1.0+ppinsi*res*insci/K_INS_TRISYN-ppglui*Lci/K_L_TRISYN)))/((kmtrisyng+g3pi)*(kmtrisynf+fai));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    g3p  = getVariableReference( "g3p" ).getVariable();
    fa  = getVariableReference( "fa" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    amp  = getVariableReference( "amp" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    tg  = getVariableReference( "tg" ).getVariable();

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
    Real fai( fa->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real ampi( amp->getMolarConc() );
    Real phosi( phos->getMolarConc() );
    Real tgi( tg->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real Lci( L->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( tg->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi = IR->getMolarConc();
    Real ppglui = GR->getMolarConc();

    // Process
    Real equation = trisyn(fai,g3pi,Lci,insci,ppglui,ppinsi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g3p;
  Variable* fa;
  Variable* atp;
  Variable* amp;
  Variable* phos;
  Variable* tg;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_TRISYN;
  Real V_TRISYN;
  Real K_INS_TRISYN;
  Real K_L_TRISYN;
  Real kmtrisyng;
  Real kmtrisynf;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;


};

LIBECS_DM_INIT( TriSynFluxProcess, Process );
