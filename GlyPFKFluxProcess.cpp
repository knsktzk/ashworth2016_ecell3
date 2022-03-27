#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlyPFKFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlyPFKFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, K_INS_PFK );
    PROPERTYSLOT_SET_GET( Real, K_L_PFK );
    PROPERTYSLOT_SET_GET( Real, V_PFK );
    PROPERTYSLOT_SET_GET( Real, kmPFK );
    PROPERTYSLOT_SET_GET( Real, kmatpPFK );
    PROPERTYSLOT_SET_GET( Real, kiatpPFK );
    PROPERTYSLOT_SET_GET( Real, kiadpPFK );
    PROPERTYSLOT_SET_GET( Real, grapeff );
    PROPERTYSLOT_SET_GET( Real, kig3p );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    // PROPERTYSLOT_SET_GET( Real, coeff_multi );
    

  }

  GlyPFKFluxProcess()
    :

    //coeff_multi( ),
    K_INS_PFK( 10.0 ),
    K_L_PFK( 10.0 ),
    V_PFK( 160.0 ),
    kmPFK( 5.0 ),
    kmatpPFK( 42.5 ),
    kiatpPFK( 2100.0 ),
    kiadpPFK( 83.6 ),
    grapeff( 0.75 ),
    kig3p( 20.7 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real,K_INS_PFK );
  SIMPLE_SET_GET_METHOD( Real,K_L_PFK );
  SIMPLE_SET_GET_METHOD( Real, V_PFK );
  SIMPLE_SET_GET_METHOD( Real, kmPFK );
  SIMPLE_SET_GET_METHOD( Real, kmatpPFK );
  SIMPLE_SET_GET_METHOD( Real, kiatpPFK );
  SIMPLE_SET_GET_METHOD( Real, kiadpPFK );
  SIMPLE_SET_GET_METHOD( Real, grapeff );
  SIMPLE_SET_GET_METHOD( Real, kig3p );
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
  // PFKPhos(Lci,Insci,ppglui,ppinsi)=(ppinsi*res*insci+kglyref)/(ppglui*lci+kglyref)
  double PFKPhos(double Lci, double insci, double ppglui, double ppinsi) {

    double val = (ppinsi*res*insci+K_INS_PFK)/(ppglui*Lci+K_L_PFK);
    return val;

  }

  // PFK(g6pi,atpi,adpi,g3pi,PFKphosi)=V_PFK*PFKphosi*hill(g6pi,kmPFK,1)*((atpi/(atpi+kmatpPFK)))*(1-(atpi/(atpi+kiatpPFK)))*(adpi/(adpi+kiadpPFK))*(1-grapeff*(g3pi/(g3pi+kig3p)))
  double PFK(double g6pi, double atpi, double adpi, double g3pi, double PFKphosi) {

    double val = V_PFK*PFKphosi*hill(g6pi,kmPFK,1)*((atpi/(atpi+kmatpPFK)))*(1-(atpi/(atpi+kiatpPFK)))*(adpi/(adpi+kiadpPFK))*(1-grapeff*(g3pi/(g3pi+kig3p)));
    return val;

  }

  virtual void initialize()
  {
    Process::initialize();
    g6p  = getVariableReference( "g6p" ).getVariable();
    g3p  = getVariableReference( "g3p" ).getVariable();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
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
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );

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
    Real PFKPhosi = PFKPhos(Lci,insci,ppglui,ppinsi);
    Real equation = PFK(g6pi,atpi,adpi,g3pi,PFKPhosi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g6p;
  Variable* g3p;
  Variable* atp;
  Variable* adp;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real K_INS_PFK;
  Real K_L_PFK;
  Real V_PFK;
  Real kmPFK;
  Real kmatpPFK;
  Real kiatpPFK;
  Real kiadpPFK;
  Real grapeff;
  Real kig3p;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  // Real coeff_multi;


};

LIBECS_DM_INIT( GlyPFKFluxProcess, Process );
