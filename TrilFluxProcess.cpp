#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( TrilFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( TrilFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, Lreftriw );
    PROPERTYSLOT_SET_GET( Real, fast );
    PROPERTYSLOT_SET_GET( Real, ttl );
 

    

  }

  TrilFluxProcess()
    :
 
    Lreftriw( 0.3 ),
    fast( 0.07 ),
    ttl( 10000.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, Lreftriw );
  SIMPLE_SET_GET_METHOD( Real, fast );
  SIMPLE_SET_GET_METHOD( Real, ttl );


  virtual void initialize()
  {
    Process::initialize();
    L  = getVariableReference( "L" ).getVariable();
    tril  = getVariableReference( "tril" ).getVariable();

  }

  virtual void fire()
  {
    Real glucagon( L->getMolarConc() );
    // Real trisynglucagon( tril->getValue() );
    Real trisynglucagon( tril->getMolarConc() );
    Real size( L->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    Real expression;

    // if((L/Lreftriw-tril)>0)then((L/Lreftriw-tril)/ttl)else((L/Lreftriw-tril)/(fast*ttl))
    if ( (glucagon/Lreftriw-trisynglucagon)>0.0 )
    {
      expression = ((glucagon/Lreftriw-trisynglucagon)/ttl);

    } else {

      expression = ((glucagon/Lreftriw-trisynglucagon)/(fast*ttl));

    }

    setFlux( sizeN_A * expression );

  }


 protected:

  // VariableReferenceList
  Variable* L;
  Variable* tril;

  // 定数
  Real Lreftriw;
  Real fast;
  Real ttl;




};

LIBECS_DM_INIT( TrilFluxProcess, Process );
