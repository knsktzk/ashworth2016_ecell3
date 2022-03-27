#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( IntestialTrisynFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( IntestialTrisynFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vtrisynthw );
    PROPERTYSLOT_SET_GET( Real, kmtrisyngw );
    PROPERTYSLOT_SET_GET( Real, kmtrisynfw );
    PROPERTYSLOT_SET_GET( Real, coeffi_multi );

  }

  IntestialTrisynFluxProcess()
    :
 
    vtrisynthw( 9.5 ),
    kmtrisyngw( 10000.0 ),
    kmtrisynfw( 645.0 ),
    coeffi_multi( 0.5 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vtrisynthw );
  SIMPLE_SET_GET_METHOD( Real, kmtrisyngw );
  SIMPLE_SET_GET_METHOD( Real, kmtrisynfw );
  SIMPLE_SET_GET_METHOD( Real, coeffi_multi );

  // trun(values)=if(values>0.05)then(values)else(0.05)
  double trun( double values ) {

    if (values>0.05)
    {
      return values;
    } else {

      return 0.05;
    }
  }

  virtual void initialize()
  {
    Process::initialize();
    gcb   = getVariableReference( "gcb" ).getVariable();
    ffa   = getVariableReference( "ffa" ).getVariable();
    tgb   = getVariableReference( "tgb" ).getVariable();
    trii  = getVariableReference( "trii" ).getVariable();
    tril  = getVariableReference( "tril" ).getVariable();

  }

  virtual void fire()
  {
    Real glucose( gcb->getMolarConc() );
    Real freefattyacids( ffa->getMolarConc() );
    Real triglycerides( tgb->getMolarConc() );
    Real trisyninsulin( trii->getMolarConc() );
    Real trisynglucagon( tril->getMolarConc() );
    Real size( gcb->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // vtrisynthw*(Gb*(FfA)*trun((1+trii-tril)))/((kmtrisyngw+Gb)*(kmtrisynfw+FfA))
    Real equation = vtrisynthw * ( glucose *( freefattyacids )*trun(( 1 + trisyninsulin - trisynglucagon )))/((kmtrisyngw+glucose)*(kmtrisynfw+freefattyacids));

    Real expression = coeffi_multi * sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* ffa;
  Variable* tgb;
  Variable* trii;
  Variable* tril;

  // 定数
  Real vtrisynthw;
  Real kmtrisyngw;
  Real kmtrisynfw;
  Real coeffi_multi;


};

LIBECS_DM_INIT( IntestialTrisynFluxProcess, Process );
