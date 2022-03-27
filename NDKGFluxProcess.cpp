#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( NDKGFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( NDKGFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, VNKG );
    PROPERTYSLOT_SET_GET( Real, kmatpnk );
    PROPERTYSLOT_SET_GET( Real, kmadpnk );
    PROPERTYSLOT_SET_GET( Real, kmgtpnk );
    PROPERTYSLOT_SET_GET( Real, kmgdpnk );
    
  }

  NDKGFluxProcess()
    :

    VNKG( 3000.0 ),
    kmatpnk( 290.0 ),
    kmadpnk( 24.0 ),
    kmgtpnk( 120.0 ),
    kmgdpnk( 33.5 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real,VNKG  );
  SIMPLE_SET_GET_METHOD( Real,kmatpnk  );
  SIMPLE_SET_GET_METHOD( Real,kmadpnk  );
  SIMPLE_SET_GET_METHOD( Real,kmgtpnk  );
  SIMPLE_SET_GET_METHOD( Real,kmgdpnk  );

  /* ZONATIONS */

  /* FUNCTIONS */

  /* Process functions */
  // NKG(GDPi,ATPi,GTPi,ADPi)=VNKG* ( (ATPi*GDPi/((kmatpnk+ATPi)*(kmgdpnk+gdpi))) - (ADPi*GTPi/((kmadpnk+adpi)*(kmgtpnk+gtpi))))
  double NKG(double gdpi, double atpi, double gtpi, double adpi) {

    double val = VNKG* ( (atpi*gdpi/((kmatpnk+atpi)*(kmgdpnk+gdpi))) - (adpi*gtpi/((kmadpnk+adpi)*(kmgtpnk+gtpi))));
    return val;

  }


  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    gtp  = getVariableReference( "gtp" ).getVariable();
    gdp  = getVariableReference( "gdp" ).getVariable();
  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real gtpi( gtp->getMolarConc() );
    Real gdpi( gdp->getMolarConc() );

    // etc
    Real size( atp->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = NKG(gdpi,atpi,gtpi,adpi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* gtp;
  Variable* gdp;

  // 定数
  Real VNKG;
  Real kmatpnk;
  Real kmadpnk;
  Real kmgtpnk;
  Real kmgdpnk;
  
};

LIBECS_DM_INIT( NDKGFluxProcess, Process );
