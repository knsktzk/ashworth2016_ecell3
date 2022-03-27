#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( FattyAcidsConsumptionFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( FattyAcidsConsumptionFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, iusage );
    PROPERTYSLOT_SET_GET( Real, kmlffause );
    PROPERTYSLOT_SET_GET( Real, kmiffause );
    PROPERTYSLOT_SET_GET( Real, vffause );
    PROPERTYSLOT_SET_GET( Real, kmffause );

  }

  FattyAcidsConsumptionFluxProcess()
    :
 
    iusage( 1.0 ),
    kmlffause( 1.0 ),
    kmiffause( 0.6 ),
    vffause( 1.4 ),
    kmffause( 100.0 )
    
  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, iusage );
  SIMPLE_SET_GET_METHOD( Real, kmlffause );
  SIMPLE_SET_GET_METHOD( Real, kmiffause );
  SIMPLE_SET_GET_METHOD( Real, vffause );
  SIMPLE_SET_GET_METHOD( Real, kmffause );


  virtual void initialize()
  {
    Process::initialize();
    ffa  = getVariableReference( "ffa" ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();

  }

  virtual void fire()
  {
    Real freefattyacids( ffa->getMolarConc() );
    Real insulin( ins->getMolarConc() );
    Real glucagon( L->getMolarConc() );
    Real size( ffa->getSuperSystem()->getSize() );

    Real expression = size * N_A * ( vffause * freefattyacids * ( iusage * insulin + kmiffause )/( ( freefattyacids + kmffause )*( glucagon + kmlffause )));
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* ffa;
  Variable* ins;
  Variable* L;

  // 定数
  Real iusage;
  Real kmlffause;
  Real kmiffause;
  Real vffause;
  Real kmffause;


};

LIBECS_DM_INIT( FattyAcidsConsumptionFluxProcess, Process );
