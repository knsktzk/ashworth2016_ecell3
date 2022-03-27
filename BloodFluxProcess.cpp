#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( BloodFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( BloodFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, bf );
    PROPERTYSLOT_SET_GET( Real, spRef );

  }

  BloodFluxProcess()
    :

    bf( 1.2 ),
    spRef( 1.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, bf );
  SIMPLE_SET_GET_METHOD( Real, spRef );

  virtual void initialize()
  {
    Process::initialize();
    here  = getVariableReference( "here" ).getVariable();
    before  = getVariableReference( "before" ).getVariable();

  }

  virtual void fire()
  {
    // Real size( here->getSuperSystem()->getSize() );
    Real size( here->getSuperSystem()->getSize() );
    Real beforei( before->getMolarConc() );
    Real herei( here->getMolarConc() );
    Real sizeN_A = size * N_A;

    Real expression = sizeN_A * bf * ( beforei - herei )/spRef;
    setFlux( expression );
  }

 protected:

  // VariableReferenceList
  Variable* here;
  Variable* before;

  // 定数
  Real bf;
  Real spRef;


};

LIBECS_DM_INIT( BloodFluxProcess, Process );
