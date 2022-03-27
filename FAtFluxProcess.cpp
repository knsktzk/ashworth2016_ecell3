#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( FAtFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( FAtFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, Z_FAT_DIF );
    PROPERTYSLOT_SET_GET( Real, V_FAT_DIF );
    PROPERTYSLOT_SET_GET( Real, kmFAt );
    PROPERTYSLOT_SET_GET( Real, Z_FAT_INS );
    PROPERTYSLOT_SET_GET( Real, V_FAT_INS );
    PROPERTYSLOT_SET_GET( Real, K_INS_FAT );
    PROPERTYSLOT_SET_GET( Real, kmfaact );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );    

  }

  FAtFluxProcess()
    :

    mtf( 1.0 ),
    Z_FAT_DIF( 0.0 ),
    V_FAT_DIF( 1.45 ),
    kmFAt( 200.0 ),
    Z_FAT_INS( 0.2 ),
    V_FAT_INS( 0.08 ),
    K_INS_FAT( 0.08 ),
    kmfaact( 2.0 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, Z_FAT_DIF );
  SIMPLE_SET_GET_METHOD( Real, V_FAT_DIF );
  SIMPLE_SET_GET_METHOD( Real, kmFAt );
  SIMPLE_SET_GET_METHOD( Real, Z_FAT_INS );
  SIMPLE_SET_GET_METHOD( Real, V_FAT_INS );
  SIMPLE_SET_GET_METHOD( Real, K_INS_FAT );
  SIMPLE_SET_GET_METHOD( Real, kmfaact );
  SIMPLE_SET_GET_METHOD( Real, res );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
  SIMPLE_SET_GET_METHOD( Real, ipp );

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
  // FAt(FFAi,FAi,insci,ppinsi,ppvali)=mtf*(1+Z_FAT_DIF*ppvali)*V_FAT_DIF*(FFAi-FAi)/(kmFAt+FFAi+FAi)+mtf*(1+Z_FAT_INS*ppvali)*V_FAT_INS*FFAi*(1+ppinsi*res*insci/K_INS_FAT)/(FFAi+kmfaact)
  double FAt(double ffai, double fai, double insci, double ppinsi, double ppvali) {
    double val = mtf*(1+Z_FAT_DIF*ppvali)*V_FAT_DIF*(ffai-fai)/(kmFAt+ffai+fai)+mtf*(1+Z_FAT_INS*ppvali)*V_FAT_INS*ffai*(1+ppinsi*res*insci/K_INS_FAT)/(ffai+kmfaact);
    return val;
  }


  virtual void initialize()
  {
    Process::initialize();
    ffa  = getVariableReference( "ffa" ).getVariable();
    fa  = getVariableReference( "fa" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    IR  = getVariableReference( "IR" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real ffai( ffa->getMolarConc() );
    Real fai( fa->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );
    Real insci( ins->getMolarConc() );

    // etc
    Real size( fa->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
    Real ppinsi( IR->getMolarConc() );

    // Process
    Real equation = FAt(ffai,fai,insci,ppinsi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* ffa;
  Variable* fa;
  Variable* oxy;
  Variable* ins;
  Variable* IR;

  // 定数
  Real mtf;
  Real Z_FAT_DIF;
  Real V_FAT_DIF;
  Real kmFAt;
  Real Z_FAT_INS;
  Real V_FAT_INS;
  Real K_INS_FAT;
  Real kmfaact;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;

};

LIBECS_DM_INIT( FAtFluxProcess, Process );
