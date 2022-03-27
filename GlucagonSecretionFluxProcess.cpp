#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( GlucagonSecretionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( GlucagonSecretionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, rel );
    PROPERTYSLOT_SET_GET( Real, gr );
    PROPERTYSLOT_SET_GET( Real, gri );
    PROPERTYSLOT_SET_GET( Real, tauL );
    PROPERTYSLOT_SET_GET( Real, tLg );
    PROPERTYSLOT_SET_GET( Real, nL );
    PROPERTYSLOT_SET_GET( Real, ML );
    PROPERTYSLOT_SET_GET( Real, tauI );
    PROPERTYSLOT_SET_GET( Real, tIg );
    PROPERTYSLOT_SET_GET( Real, nI );
    PROPERTYSLOT_SET_GET( Real, MI );
    PROPERTYSLOT_SET_GET( Real, mins );
    PROPERTYSLOT_SET_GET( Real, BloodScale );
    

  }

  GlucagonSecretionFluxProcess()
    :
 
    rel( 5.0 ),
    gr( 3.0 ),
    gri( 2.0 ),
    tauL( 0.036 ),
    tLg( 1.3 ),
    nL( 2.0 ),
    ML( 3.0 ),
    tauI( 0.075 ),
    tIg( 0.9 ),
    nI( 2.0 ),
    MI( 4.0 ),
    mins( 60.0 ),
    BloodScale( 0.00049999995 )
    
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, rel );
  SIMPLE_SET_GET_METHOD( Real, gr );
  SIMPLE_SET_GET_METHOD( Real, gri );
  SIMPLE_SET_GET_METHOD( Real, tauL );
  SIMPLE_SET_GET_METHOD( Real, tLg );
  SIMPLE_SET_GET_METHOD( Real, nL );
  SIMPLE_SET_GET_METHOD( Real, ML );
  SIMPLE_SET_GET_METHOD( Real, tauI );
  SIMPLE_SET_GET_METHOD( Real, tIg );
  SIMPLE_SET_GET_METHOD( Real, nI );
  SIMPLE_SET_GET_METHOD( Real, MI );
  SIMPLE_SET_GET_METHOD( Real, mins );
  SIMPLE_SET_GET_METHOD( Real, BloodScale );

  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))
  double hill(double x, double Xt, double n) {

    double pow(double x, double y);
    double s = pow(x, n)/(pow(Xt, n) + pow(x, n));
    //float powf(float x, float y);
    //float s = powf(x, n)/(powf(Xt, n) + powf(x, n));
    return s;
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

    double s = H( log( H( x )) );
    return s;
  }


  virtual void initialize()
  {
    Process::initialize();
    gcb  = getVariableReference( "gcb" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();

  }

  virtual void fire()
  {
    Real glucose( gcb->getMolarConc() );
    Real glucagon( L->getMolarConc() );
    Real size( gcb->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    // (1/(tauL*rel*mins))*(hill(Aln(gr/(BloodScale*gB)),tLg,nL))
    Real equation = (1/(tauL*rel*mins))*(hill(Aln(gr/(BloodScale*glucose)),tLg,nL));

    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* L;

  // 定数
  Real rel;
  Real gr;
  Real gri;
  Real tauL;
  Real tLg;
  Real nL;
  Real ML;
  Real tauI;
  Real tIg;
  Real nI;
  Real MI;
  Real mins;
  Real BloodScale;

};

LIBECS_DM_INIT( GlucagonSecretionFluxProcess, Process );
