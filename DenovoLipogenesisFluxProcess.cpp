#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( DenovoLipogenesisFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( DenovoLipogenesisFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vdenog );
    PROPERTYSLOT_SET_GET( Real, iadi );
    PROPERTYSLOT_SET_GET( Real, irefdenog );
    PROPERTYSLOT_SET_GET( Real, lrefdenog );
    PROPERTYSLOT_SET_GET( Real, kmdenog );

    

  }

  DenovoLipogenesisFluxProcess()
    :
 
    vdenog( 0.11 ),
    iadi( 1.0 ),
    irefdenog( 7.0 ),
    lrefdenog( 2.0 ),
    kmdenog( 4500.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vdenog );
  SIMPLE_SET_GET_METHOD( Real, iadi );
  SIMPLE_SET_GET_METHOD( Real, irefdenog );
  SIMPLE_SET_GET_METHOD( Real, lrefdenog );
  SIMPLE_SET_GET_METHOD( Real, kmdenog );


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
    gcb  = getVariableReference( "gcb" ).getVariable();
    ffa  = getVariableReference( "ffa" ).getVariable();
    ins  = getVariableReference( "ins" ).getVariable();
    L    = getVariableReference(  "L"  ).getVariable();

  }

  virtual void fire()
  {
    Real glucose( gcb->getMolarConc() );
    Real freefattyacids( ffa->getMolarConc() );
    Real insulin( ins->getMolarConc() );
    Real glucagon( L->getMolarConc() );
    Real size( gcb->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // vdenog*Gb*trun((1+iadi*ins/irefdenog-l/lrefdenog))/(Gb+kmdenog)
    Real equation = vdenog * glucose * trun(( 1 + iadi * insulin / irefdenog - glucagon/lrefdenog))/(glucose+kmdenog);

    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* gcb;
  Variable* ffa;
  Variable* ins;
  Variable* L;

  // 定数
  Real vdenog;
  Real iadi;
  Real irefdenog;
  Real lrefdenog;
  Real kmdenog;



};

LIBECS_DM_INIT( DenovoLipogenesisFluxProcess, Process );
