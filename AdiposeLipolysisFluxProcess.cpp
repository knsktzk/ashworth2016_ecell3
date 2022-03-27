#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( AdiposeLipolysisFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( AdiposeLipolysisFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, vlipolyw );
    PROPERTYSLOT_SET_GET( Real, kmlipow );

 
  }

  AdiposeLipolysisFluxProcess()
    :
 
    vlipolyw( 2.3 ),
    kmlipow( 2000.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, vlipolyw );
  SIMPLE_SET_GET_METHOD( Real, kmlipow );

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
    tgb    = getVariableReference( "tgb"   ).getVariable();
    ffa    = getVariableReference( "ffa"   ).getVariable();
    glyb   = getVariableReference( "glyb"  ).getVariable();
    lipoi  = getVariableReference( "lipoi" ).getVariable();
    lipol  = getVariableReference( "lipol" ).getVariable();


  }

  virtual void fire()
  {
    Real triglycerides( tgb->getMolarConc() );
    Real freefattyacids( ffa->getMolarConc() );
    Real glyceriol( glyb->getMolarConc() );
    Real lipoinsulin( lipoi->getMolarConc() );
    Real lipoglucagon( lipol->getMolarConc() );

    Real size( tgb->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // vlipolyw*TGb*trun((1-lipoi+lipol))/(TGb+kmlipow)
    Real equation = vlipolyw * triglycerides * trun((1 - lipoinsulin + lipoglucagon ))/( triglycerides + kmlipow );

    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* tgb;
  Variable* ffa;
  Variable* glyb;
  Variable* lipoi;
  Variable* lipol;

  // Parameters
  Real vlipolyw;
  Real kmlipow;




};

LIBECS_DM_INIT( AdiposeLipolysisFluxProcess, Process );
