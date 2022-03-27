#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( LipolFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( LipolFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, LrefLipow );
    PROPERTYSLOT_SET_GET( Real, tll );
    PROPERTYSLOT_SET_GET( Real, fast );

  }

  LipolFluxProcess()
    :
 
    LrefLipow( 0.3 ),
    tll( 10000.0 ),
    fast( 0.07 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, LrefLipow );
  SIMPLE_SET_GET_METHOD( Real, tll );
  SIMPLE_SET_GET_METHOD( Real, fast );


  virtual void initialize()
  {
    Process::initialize();
    L      = getVariableReference( "L" ).getVariable();
    lipol  = getVariableReference( "lipol" ).getVariable();

  }

  virtual void fire()
  {
    Real glucagon( L->getMolarConc() );
    // Real lipoglucagon( lipol->getValue() );
    Real lipoglucagon( lipol->getMolarConc() );
    Real size( L->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    Real expression;

    // if((L/LrefLipow-lipol)>0) then((L/LrefLipow-lipol)/tll) else((L/LrefLipow-lipol)/(fast*tll))
    if ( (glucagon/LrefLipow-lipoglucagon) > 0.0 )
    {
      expression = (glucagon/LrefLipow-lipoglucagon)/tll;

    } else {

      expression = (glucagon/LrefLipow-lipoglucagon)/(fast*tll);
    }

    // setFlux( expression );
    setFlux( sizeN_A * expression );

  }


 protected:

  // VariableReferenceList
  Variable* L;
  Variable* lipol;

  // 定数
  Real LrefLipow;
  Real tll;
  Real fast;

};

LIBECS_DM_INIT( LipolFluxProcess, Process );
