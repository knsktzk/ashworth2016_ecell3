#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( InsulinSecretionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( InsulinSecretionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, rel );
    PROPERTYSLOT_SET_GET( Real, gri );
    PROPERTYSLOT_SET_GET( Real, tauI );
    PROPERTYSLOT_SET_GET( Real, tIg );
    PROPERTYSLOT_SET_GET( Real, nI );
    PROPERTYSLOT_SET_GET( Real, mins );
    PROPERTYSLOT_SET_GET( Real, BloodScale );

    

  }

  InsulinSecretionFluxProcess()
    :
 
    rel( 5.0 ),
    gri( 2.0 ),
    tauI( 0.075 ),
    tIg( 0.9 ),
    nI( 2.0 ),
    mins( 60.0 ),
    BloodScale( 0.00049999995 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, rel );
  SIMPLE_SET_GET_METHOD( Real, gri );
  SIMPLE_SET_GET_METHOD( Real, tauI );
  SIMPLE_SET_GET_METHOD( Real, tIg );
  SIMPLE_SET_GET_METHOD( Real, nI );
  SIMPLE_SET_GET_METHOD( Real, mins );
  SIMPLE_SET_GET_METHOD( Real, BloodScale );



  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))
  double hill(double x, double Xt, double n) {

    double pow(double x, double y);
    double s = pow(x, n)/(pow(Xt, n) + pow(x, n));
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

    double s = H( log( H( x ) ) );
    return s;
  }


  virtual void initialize()
  {
    Process::initialize();
    gcb  = getVariableReference( "gcb" ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();

  }

  virtual void fire()
  {
    Real glucose( gcb->getMolarConc() );
    Real insulin( ins->getMolarConc() );
    Real size( gcb->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    // (1/(tauI*rel*mins))*(hill(Aln(BloodScale*gB/gri),tIg,nI))
    Real equation = (1/(tauI*rel*mins))*(hill(Aln(BloodScale*glucose/gri),tIg,nI));

    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* ins;

  // 定数
  Real rel;
  Real gri;
  Real tauI;
  Real tIg;
  Real nI;
  Real mins;
  Real BloodScale;


};

LIBECS_DM_INIT( InsulinSecretionFluxProcess, Process );
