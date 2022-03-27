#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( TriiFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( TriiFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, iadi );
    PROPERTYSLOT_SET_GET( Real, Ireftriw );
    PROPERTYSLOT_SET_GET( Real, slow );
    PROPERTYSLOT_SET_GET( Real, tti );

  }

  TriiFluxProcess()
    :
 
    iadi( 1.0 ),
    Ireftriw( 3.0 ),
    slow( 15.0 ),
    tti( 1000.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, iadi );
  SIMPLE_SET_GET_METHOD( Real, Ireftriw );
  SIMPLE_SET_GET_METHOD( Real, slow );
  SIMPLE_SET_GET_METHOD( Real, tti );


  virtual void initialize()
  {
    Process::initialize();
    trii  = getVariableReference( "trii" ).getVariable();
    ins   = getVariableReference( "ins" ).getVariable();

  }

  virtual void fire()
  {
    // Real trisyninsulin( trii->getValue() );
    Real trisyninsulin( trii->getMolarConc() );
    Real insulin( ins->getMolarConc() );
    Real size( ins->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    Real expression;

    // if((iadi*Ins/Ireftriw-trii)>0)then((iadi*Ins/Ireftriw-trii)/tti)else((iadi*Ins/Ireftriw-trii)/(slow*tti))
    if ( (iadi*insulin/Ireftriw-trisyninsulin) > 0.0 )
    {
      expression = ((iadi*insulin/Ireftriw-trisyninsulin)/tti);

    } else {
      
      expression = ((iadi*insulin/Ireftriw-trisyninsulin)/(slow*tti));
    }
    
    // setFlux( expression );
    setFlux( sizeN_A * expression );

  }


 protected:

  // VariableReferenceList
  Variable* trii;
  Variable* ins;

  // 定数
  Real iadi;
  Real Ireftriw;
  Real slow;
  Real tti;

};

LIBECS_DM_INIT( TriiFluxProcess, Process );
