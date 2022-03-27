#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( LipolysisFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( LipolysisFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Z_LIPOLY );
    PROPERTYSLOT_SET_GET( Real, V_LIPOLY );
    PROPERTYSLOT_SET_GET( Real, K_INS_LIPOLY );
    PROPERTYSLOT_SET_GET( Real, K_L_LIPOLY );
    PROPERTYSLOT_SET_GET( Real, kmlipo );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );

    

  }

  LipolysisFluxProcess()
    :
 
    Z_LIPOLY( 0.0 ),
    V_LIPOLY( 0.085 ),
    K_INS_LIPOLY( 4.0 ),
    K_L_LIPOLY( 5.0 ),
    kmlipo( 50715.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Z_LIPOLY );
  SIMPLE_SET_GET_METHOD( Real, V_LIPOLY );
  SIMPLE_SET_GET_METHOD( Real, K_INS_LIPOLY );
  SIMPLE_SET_GET_METHOD( Real, K_L_LIPOLY );
  SIMPLE_SET_GET_METHOD( Real, kmlipo );
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
  // lipoly(TGi,lci,insci,ppglui,ppinsi,ppvali)=(1+Z_LIPOLY*ppvali)*V_LIPOLY*TGi*trun((1-ppinsi*res*insci/K_INS_LIPOLY+ppglui*Lci/K_L_LIPOLY))/(TGi+kmlipo)
  double lipoly(double tgi, double Lci, double insci, double ppglui, double ppinsi, double ppvali){
    double val = (1.0+Z_LIPOLY*ppvali)*V_LIPOLY*tgi*trun((1.0-ppinsi*res*insci/K_INS_LIPOLY+ppglui*Lci/K_L_LIPOLY))/(tgi+kmlipo);
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    fa  = getVariableReference( "fa" ).getVariable();
    tg  = getVariableReference( "tg" ).getVariable();
    gly  = getVariableReference( "gly" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR   = getVariableReference(  "IR"  ).getVariable();
    GR   = getVariableReference( "GR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real fai( fa->getMolarConc() );
    Real tgi( tg->getMolarConc() );
    Real glyi( gly->getMolarConc() );

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
    Real equation = lipoly(tgi,Lci,insci,ppglui,ppinsi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* fa;
  Variable* tg;
  Variable* gly;
  Variable* oxy;
  Variable* L;
  Variable* ins;
  Variable* IR;
  Variable* GR;

  // 定数
  Real Z_LIPOLY;
  Real V_LIPOLY;
  Real K_INS_LIPOLY;
  Real K_L_LIPOLY;
  Real kmlipo;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;


};

LIBECS_DM_INIT( LipolysisFluxProcess, Process );
