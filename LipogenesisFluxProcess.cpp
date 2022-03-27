#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( LipogenesisFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( LipogenesisFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_LIPOG );
    PROPERTYSLOT_SET_GET( Real, V_LIPOG );
    PROPERTYSLOT_SET_GET( Real, K_INS_LIPOG );
    PROPERTYSLOT_SET_GET( Real, K_L_LIPOG );
    PROPERTYSLOT_SET_GET( Real, kmfawinh );
    PROPERTYSLOT_SET_GET( Real, kmlipogen );
    PROPERTYSLOT_SET_GET( Real, kmatpacc );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    PROPERTYSLOT_SET_GET( Real, coeff_multi );
    

  }

  LipogenesisFluxProcess()
    :

    coeff_multi( 0.125 ),
    Z_LIPOG( -0.23 ),
    V_LIPOG( 4.0 ),
    K_INS_LIPOG( 3.0 ),
    K_L_LIPOG( 7.0 ),
    kmfawinh( 300.0 ),
    kmlipogen( 58.0 ),
    kmatpacc( 120.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_LIPOG );
  SIMPLE_SET_GET_METHOD( Real, V_LIPOG );
  SIMPLE_SET_GET_METHOD( Real, K_INS_LIPOG );
  SIMPLE_SET_GET_METHOD( Real, K_L_LIPOG );
  SIMPLE_SET_GET_METHOD( Real, kmfawinh );
  SIMPLE_SET_GET_METHOD( Real, kmlipogen );
  SIMPLE_SET_GET_METHOD( Real, kmatpacc );
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


  /* Process functions */
  // lipog(acoai,atpi,Lci,Insci,fai,ppglui,ppinsi,ppvali)=(1+Z_LIPOG*ppvali)*V_LIPOG*acoAi*(1+ppinsi*res*insci/K_INS_LIPOG-ppglui*Lci/K_L_LIPOG)*(1-fai/(fai+kmfawinh))/(kmlipogen+acoAi)*(atpi/(atpi+kmatpacc))
  double lipog(double acoai, double atpi, double Lci, double insci, double fai, double ppglui, double ppinsi, double ppvali) {

    double val = (1.0+Z_LIPOG*ppvali)*V_LIPOG*acoai*(1.0+ppinsi*res*insci/K_INS_LIPOG-ppglui*Lci/K_L_LIPOG)*(1.0-fai/(fai+kmfawinh))/(kmlipogen+acoai)*(atpi/(atpi+kmatpacc));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    acoa  = getVariableReference( "acoa" ).getVariable();
    fa  = getVariableReference( "fa" ).getVariable();
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
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real acoai( acoa->getMolarConc() );
    Real fai( fa->getMolarConc() );
    Real phosi( phos->getMolarConc() );

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
    Real equation = lipog(acoai,atpi,Lci,insci,fai,ppglui,ppinsi,ppvali);
    Real expression = coeff_multi * sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* acoa;
  Variable* fa;
  Variable* phos;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_LIPOG;
  Real V_LIPOG;
  Real K_INS_LIPOG;
  Real K_L_LIPOG;
  Real kmfawinh;
  Real kmlipogen;
  Real kmatpacc;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  Real coeff_multi;


};

LIBECS_DM_INIT( LipogenesisFluxProcess, Process );
