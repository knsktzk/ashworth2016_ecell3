#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( LipoiFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( LipoiFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, iadi );
    PROPERTYSLOT_SET_GET( Real, Ireflipow );
    PROPERTYSLOT_SET_GET( Real, tli );
    PROPERTYSLOT_SET_GET( Real, slow );

  }

  LipoiFluxProcess()
    :
 
    iadi( 1.0 ),
    Ireflipow( 3.0 ),
    tli( 1000.0 ),
    slow( 15.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real,iadi  );
  SIMPLE_SET_GET_METHOD( Real,Ireflipow  );
  SIMPLE_SET_GET_METHOD( Real,tli  );
  SIMPLE_SET_GET_METHOD( Real,slow  );
  
  virtual void initialize()
  {
    Process::initialize();
    lipoi  = getVariableReference( "lipoi" ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();

  }

  virtual void fire()
  {
    // Real lipoinsulin( lipoi->getValue() );
    Real lipoinsulin( lipoi->getMolarConc() );
    Real insulin( ins->getMolarConc() );
    Real size( ins->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;
    Real expression;

    // if((iadi*Ins/Ireflipow-lipoi)>0)then((iadi*Ins/Ireflipow-lipoi)/tli)else((iadi*Ins/Ireflipow-lipoi)/(slow*tli))
    if ( (iadi * insulin/Ireflipow - lipoinsulin )>0.0 )
    {
      expression = (iadi*insulin/Ireflipow-lipoinsulin)/tli;

    } else {
      expression = ((iadi*insulin/Ireflipow-lipoinsulin)/(slow*tli));

    }

    // setFlux( expression );
    setFlux( sizeN_A * expression );

  }


 protected:

  // VariableReferenceList
  Variable* lipoi;
  Variable* ins;

  // 定数
  Real iadi;
  Real Ireflipow;
  Real tli;
  Real slow;



};

LIBECS_DM_INIT( LipoiFluxProcess, Process );
