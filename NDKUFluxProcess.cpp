#include "libecs.hpp"
#include "ContinuousProcess.hpp"
#include <math.h>

USE_LIBECS;

LIBECS_DM_CLASS( NDKUFluxProcess, ContinuousProcess )
{

 public:

  LIBECS_DM_OBJECT( NDKUFluxProcess, Process )
  {
    INHERIT_PROPERTIES( Process );

    PROPERTYSLOT_SET_GET( Real, VNKU );
    PROPERTYSLOT_SET_GET( Real, kmatpnk );
    PROPERTYSLOT_SET_GET( Real, kmadpnk );
    PROPERTYSLOT_SET_GET( Real, kmutpnk );
    PROPERTYSLOT_SET_GET( Real, kmudpnk );  

  }

  NDKUFluxProcess()
    :

    VNKU( 30.0 ),
    kmatpnk( 290.0 ),
    kmadpnk( 24.0 ),
    kmutpnk( 21500.0 ),
    kmudpnk( 175.0 )

  {
    // do nothing
  }

  SIMPLE_SET_GET_METHOD( Real, VNKU );
  SIMPLE_SET_GET_METHOD( Real, kmatpnk );
  SIMPLE_SET_GET_METHOD( Real, kmadpnk );
  SIMPLE_SET_GET_METHOD( Real, kmutpnk );
  SIMPLE_SET_GET_METHOD( Real, kmudpnk );


  /* ZONATIONS */
 
  /* FUNCTIONS */

  /* Process functions */
  // NKu(UDPi,ATPi,UTPi,ADPi)=VNKU* ( (ATPi*UDPi/((kmatpnk+ATPi)*(kmudpnk+udpi))) - (ADPi*UTPi/((kmadpnk+adpi)*(kmutpnk+utpi))))
  double NKu(double UDPi, double ATPi, double UTPi, double ADPi) {

    double val = VNKU* ( (ATPi*UDPi/((kmatpnk+ATPi)*(kmudpnk+UDPi))) - (ADPi*UTPi/((kmadpnk+ADPi)*(kmutpnk+UTPi))));
    return val;

  }

  virtual void initialize()
  {
    Process::initialize();
    atp  = getVariableReference( "atp" ).getVariable();
    adp  = getVariableReference( "adp" ).getVariable();
    utp  = getVariableReference( "utp" ).getVariable();
    udp  = getVariableReference( "udp" ).getVariable();


  }

  virtual void fire()
  {
    // Variables
    Real atpi( atp->getMolarConc() );
    Real adpi( adp->getMolarConc() );
    Real utpi( utp->getMolarConc() );
    Real udpi( udp->getMolarConc() );

    // etc
    Real size( atp->getSuperSystem()->getSize() );
    Real sizeN_A = size * N_A;

    // Process
    Real equation = NKu(udpi,atpi,utpi,adpi);
    Real expression = sizeN_A * equation;
    setFlux( expression );

  }


 protected:

  // VariableReferenceList
  Variable* atp;
  Variable* adp;
  Variable* utp;
  Variable* udp;


  // 定数
  Real VNKU;
  Real kmatpnk;
  Real kmadpnk;
  Real kmutpnk;
  Real kmudpnk;

};

LIBECS_DM_INIT( NDKUFluxProcess, Process );
