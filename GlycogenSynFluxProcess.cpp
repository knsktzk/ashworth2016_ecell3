#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlycogenSynFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlycogenSynFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, filli );
    PROPERTYSLOT_SET_GET( Real, Z_GSYN );
    PROPERTYSLOT_SET_GET( Real, K_INS_GSYN );
    PROPERTYSLOT_SET_GET( Real, K_L_GSYN );
    PROPERTYSLOT_SET_GET( Real, V_GSYN );
    PROPERTYSLOT_SET_GET( Real, nf );
    PROPERTYSLOT_SET_GET( Real, glus );
    PROPERTYSLOT_SET_GET( Real, kmutpgs );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );

  }

  GlycogenSynFluxProcess()
    :

    filli( 1.0 ),
    Z_GSYN( 0.5 ),
    K_INS_GSYN( 0.05 ),
    K_L_GSYN( 0.5 ),
    V_GSYN( 55.0 ),
    nf( 4.0 ),
    glus( 50.0 ),
    kmutpgs( 48.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, filli );
  SIMPLE_SET_GET_METHOD( Real, Z_GSYN );
  SIMPLE_SET_GET_METHOD( Real, K_INS_GSYN );
  SIMPLE_SET_GET_METHOD( Real, K_L_GSYN );
  SIMPLE_SET_GET_METHOD( Real, V_GSYN );
  SIMPLE_SET_GET_METHOD( Real, nf );
  SIMPLE_SET_GET_METHOD( Real, glus );
  SIMPLE_SET_GET_METHOD( Real, kmutpgs );
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

  /* Process functions */
  // GSPhos(Lci,Insci,ppglui,ppinsi)=(ppinsi*res*insci+K_INS_GSYN)/(ppglui*lci+K_L_GSYN)
  double GSPhos(double Lci, double insci, double ppglui, double ppinsi) {
    double val = (ppinsi*res*insci+K_INS_GSYN)/(ppglui*Lci+K_L_GSYN);
    return val;
  }

  // GS(g6pi,utpi,ppvali,GSphosi,filli)=(1+Z_GSYN*ppvali)*GSphosi*V_GSYN*hill(g6pi,glus,nf)*filli*(utpi/(utpi+kmutpgs))
  double GS(double g6pi, double utpi, double ppvali, double GSPhosi) {
    double val = (1+Z_GSYN*ppvali)*GSPhosi*V_GSYN*hill(g6pi,glus,nf)*filli*(utpi/(utpi+kmutpgs));
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    g    = getVariableReference( "g" ).getVariable();
    g6p  = getVariableReference( "g6p" ).getVariable();
    phos = getVariableReference("phos" ).getVariable();
    utp  = getVariableReference( "utp" ).getVariable();
    udp  = getVariableReference( "udp" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();
    L    = getVariableReference( "L"  ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR   = getVariableReference( "IR"  ).getVariable();
    GR   = getVariableReference( "GR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real gi( g->getMolarConc() );
    Real g6pi( g6p->getMolarConc() );
    Real phosi( phos->getMolarConc() );
    Real utpi( utp->getMolarConc() );
    Real udpi( udp->getMolarConc() );

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
    Real GSPhosi = GSPhos(Lci,insci,ppglui,ppinsi);
    Real equation = GS(g6pi,utpi,ppvali,GSPhosi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* g;
  Variable* g6p;
  Variable* phos;
  Variable* utp;
  Variable* udp;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real filli;
  Real Z_GSYN;
  Real K_INS_GSYN;
  Real K_L_GSYN;
  Real V_GSYN;
  Real nf;
  Real glus;
  Real kmutpgs;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;

};

LIBECS_DM_INIT( GlycogenSynFluxProcess, Process );
