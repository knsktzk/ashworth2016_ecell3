#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( AtpSynFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( AtpSynFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, V_ATPS );
    PROPERTYSLOT_SET_GET( Real, kmadpas );
    PROPERTYSLOT_SET_GET( Real, kmacoaas );
    PROPERTYSLOT_SET_GET( Real, kmp );
    PROPERTYSLOT_SET_GET( Real, Z_ATPS );
    PROPERTYSLOT_SET_GET( Real, kmoxy );
    PROPERTYSLOT_SET_GET( Real, noxy );
    PROPERTYSLOT_SET_GET( Real, res );
    PROPERTYSLOT_SET_GET( Real, opt1 );
    PROPERTYSLOT_SET_GET( Real, opt2 );
    PROPERTYSLOT_SET_GET( Real, opt3 );
    PROPERTYSLOT_SET_GET( Real, ipp );
    PROPERTYSLOT_SET_GET( Real, pglu );
    PROPERTYSLOT_SET_GET( Real, coeff_multi );
    

  }

  AtpSynFluxProcess()
    :

    
    coeff_multi( 12.0 ),
    V_ATPS( 525.0 ),
    kmadpas( 410.0 ),
    kmacoaas( 0.4 ),
    kmp( 3830.0 ),
    Z_ATPS( 0.23 ),
    kmoxy( 28.0 ),
    noxy( 1.5 ),
    res( 1.0 ),
    opt1( 1.0 ),
    opt2( 0.0 ),
    opt3( 0.0 ),
    ipp( -0.15 ),
    pglu( 0.15 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, V_ATPS );
  SIMPLE_SET_GET_METHOD( Real, kmadpas );
  SIMPLE_SET_GET_METHOD( Real, kmacoaas );
  SIMPLE_SET_GET_METHOD( Real, kmp );
  SIMPLE_SET_GET_METHOD( Real, Z_ATPS );
  SIMPLE_SET_GET_METHOD( Real, kmoxy );
  SIMPLE_SET_GET_METHOD( Real, noxy );
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
  // hill(x,Xt,n)=(x^n)/((Xt^n)+(x^n))
  double hill(double x, double Xt, double n) {

    double pow(double x, double y);
    double val = pow(x, n)/(pow(Xt, n) + pow(x, n));
    return val;
  }


  /* Process functions */
  // ATPS(adpi,phosi,acoai,oxyi,ppvali)=V_ATPS*(adpi/(kmadpas+adpi))*(acoai/(acoai+kmacoaas))*(phosi/(kmp+phosi))*(1+ppvali*Z_ATPS)*hill(oxyi,kmoxy,noxy)
  double ATPS(double adpi, double phosi, double acoai, double oxyi, double ppvali) {

    double val = V_ATPS*(adpi/(kmadpas+adpi))*(acoai/(acoai+kmacoaas))*(phosi/(kmp+phosi))*(1+ppvali*Z_ATPS)*hill(oxyi,kmoxy,noxy);
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    acoa  = getVariableReference( "acoa" ).getVariable();
    phos  = getVariableReference( "phos" ).getVariable();
    oxy  = getVariableReference( "oxy" ).getVariable();

  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real acoai( acoa->getMolarConc() );
    Real phosi( phos->getMolarConc() );

    // oxygen and hormone
    Real oxyi( oxy->getMolarConc() );

    // etc
    Real size( acoa->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

      // zonations
    Real ppvali = ppcalc( oxyi );

    // Process
    Real equation = ATPS(adpi,phosi,acoai,oxyi,ppvali);
    Real expression =  sizeN_A * equation / coeff_multi;
    //Real expression =  sizeN_A * equation * coeff_multi;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* acoa;
  Variable* phos;
  Variable* oxy;

  // 定数
  Real V_ATPS;
  Real kmadpas;
  Real kmacoaas;
  Real kmp;
  Real Z_ATPS;
  Real kmoxy;
  Real noxy;
  Real res;
  Real opt1;
  Real opt2;
  Real opt3;
  Real ipp;
  Real pglu;
  Real coeff_multi;


};

LIBECS_DM_INIT( AtpSynFluxProcess, Process );
