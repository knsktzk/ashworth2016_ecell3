#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GluPEPCKFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GluPEPCKFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_PEPCK );
    PROPERTYSLOT_SET_GET( Real, V_PEPCK );
    PROPERTYSLOT_SET_GET( Real, K_INS_PEPCK );
    PROPERTYSLOT_SET_GET( Real, K_L_PEPCK );
    PROPERTYSLOT_SET_GET( Real, kmlacPCK );
    PROPERTYSLOT_SET_GET( Real, kmatpPCK );
    PROPERTYSLOT_SET_GET( Real, kmgtpPCK );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    // PROPERTYSLOT_SET_GET( Real, coeff_multi );


  }

  GluPEPCKFluxProcess()
    :

    //coeff_multi( ),
    Z_PEPCK( 0.412 ),
    V_PEPCK( 35.0 ),
    K_INS_PEPCK( 8.5 ),
    K_L_PEPCK( 8.5 ),
    kmlacPCK( 500.0 ),
    kmatpPCK( 10.0 ),
    kmgtpPCK( 64.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_PEPCK );
  SIMPLE_SET_GET_METHOD( Real, V_PEPCK );
  SIMPLE_SET_GET_METHOD( Real, K_INS_PEPCK );
  SIMPLE_SET_GET_METHOD( Real, K_L_PEPCK );
  SIMPLE_SET_GET_METHOD( Real, kmlacPCK );
  SIMPLE_SET_GET_METHOD( Real, kmatpPCK );
  SIMPLE_SET_GET_METHOD( Real, kmgtpPCK );
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

  /* Process functions */
  // PEPCKphos(Lci,InscI,ppglui,ppinsi)=(ppglui*lci+kglurefb)/(ppinsi*res*insci+kglurefb)
  double PEPCKphos(double Lci, double insci, double ppglui, double ppinsi) {

    double val = (ppglui*Lci+K_L_PEPCK)/(ppinsi*res*insci+K_INS_PEPCK);
    return val;
  }

  // gluPEPCK(lacci,atpi,gtpi,ppvali,Pepckphosi)=(1+ppvali*Z_PEPCK)*V_PEPCK*PEPCKphosi*hill(lacci,kmlacPCK,1)*(atpi/(atpi+kmatpPCK))*(gtpi/(gtpi+kmgtpPCK))
  double gluPEPCK(double pyri, double atpi, double gtpi, double ppvali, double PEPCKphosi) {

    double val = (1.0+ppvali*Z_PEPCK)*V_PEPCK*PEPCKphosi*hill(pyri,kmlacPCK,1.0)*(atpi/(atpi+kmatpPCK))*(gtpi/(gtpi+kmgtpPCK));
    return val;

  }

  virtual void initialize()
  {
    Process::initialize();
    pyr  = getVariableReference( "pyr" ).getVariable();
    g3p  = getVariableReference( "g3p" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    gdp  = getVariableReference( "gdp" ).getVariable();
    gtp  = getVariableReference( "gtp" ).getVariable();
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
    Real g3pi( g3p->getMolarConc() );
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real gdpi( gdp->getMolarConc() );
    Real gtpi( gtp->getMolarConc() );
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
    Real PEPCKphosi = PEPCKphos(Lci,insci,ppglui,ppinsi);
    Real equation = gluPEPCK(pyri,atpi,gtpi,ppvali,PEPCKphosi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* pyr;
  Variable* g3p;
  Variable* atp;
  Variable* adp;
  Variable* gdp;
  Variable* gtp;
  Variable* phos;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_PEPCK;
  Real V_PEPCK;
  Real K_INS_PEPCK;
  Real K_L_PEPCK;
  Real kmlacPCK;
  Real kmatpPCK;
  Real kmgtpPCK;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  // Real coeff_multi;


};

LIBECS_DM_INIT( GluPEPCKFluxProcess, Process );
