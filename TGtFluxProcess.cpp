#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( TGtFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( TGtFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, mtf );
    PROPERTYSLOT_SET_GET( Real, Z_TGT_VLDL);
    PROPERTYSLOT_SET_GET( Real, V_TGT_VLDL );
    PROPERTYSLOT_SET_GET( Real, kmTGt );
    PROPERTYSLOT_SET_GET( Real, Z_TGT_DIF );
    PROPERTYSLOT_SET_GET( Real, V_TGT_DIF );
    PROPERTYSLOT_SET_GET( Real, tgref );
    PROPERTYSLOT_SET_GET( Real, kmtgtp );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    

  }

  TGtFluxProcess()
    :

    mtf( 1.0 ),
    Z_TGT_VLDL( 0.0 ),
    V_TGT_VLDL( 0.3 ),
    kmTGt( 33810.0 ),
    Z_TGT_DIF( 0.0 ),
    V_TGT_DIF( 0.6 ),
    tgref( 33.81 ),
    kmtgtp( 1000.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, mtf );
  SIMPLE_SET_GET_METHOD( Real, Z_TGT_VLDL  );
  SIMPLE_SET_GET_METHOD( Real, V_TGT_VLDL );
  SIMPLE_SET_GET_METHOD( Real, kmTGt );
  SIMPLE_SET_GET_METHOD( Real, Z_TGT_DIF );
  SIMPLE_SET_GET_METHOD( Real, V_TGT_DIF );
  SIMPLE_SET_GET_METHOD( Real, tgref );
  SIMPLE_SET_GET_METHOD( Real, kmtgtp );
  SIMPLE_SET_GET_METHOD( Real, opt1 );
  SIMPLE_SET_GET_METHOD( Real, opt2 );
  SIMPLE_SET_GET_METHOD( Real, opt3 );
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
  // TGtp(TGbI,TGi,ppvali)=(1+Z_TGT_DIF*ppvali)*mtf*V_TGT_DIF*(TGbi-TGi/tgref)/(kmTGtp+TGi/tgref+TGBi)
  double TGtp(double TGbi, double TGi, double ppvali) {

    double val = (1+Z_TGT_DIF*ppvali)*mtf*V_TGT_DIF*(TGbi-TGi/tgref)/(kmtgtp+TGi/tgref+TGbi);
    return val;

  }

  // TGta(TGi,ppvali)=(0-1)*(mtf)*(1+Z_TGT_VLDL*ppvali)*V_TGT_VLDL*TGi/(kmTGt+TGi) release as VLDL
  double TGta(double TGi, double ppvali) {

    double val = (0-1)*(mtf)*(1+Z_TGT_VLDL*ppvali)*V_TGT_VLDL*TGi/(kmTGt+TGi);
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    tgb  = getVariableReference( "tgb" ).getVariable();
    tg  = getVariableReference( "tg" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real tgbi( tgb->getMolarConc() );
    Real tgi( tg->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );

    // etc
    Real size( tg->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );
  
    // Process
    Real equation = TGta(tgi,ppvali) + TGtp(tgbi,tgi,ppvali);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* tgb;
  Variable* tg;
  Variable* oxy;

  // 定数
  Real mtf;
  Real Z_TGT_VLDL;
  Real V_TGT_VLDL;
  Real kmTGt;
  Real Z_TGT_DIF;
  Real V_TGT_DIF;
  Real tgref;
  Real kmtgtp;
  Real opt1;
  Real opt2;
  Real opt3;


};

LIBECS_DM_INIT( TGtFluxProcess, Process );
