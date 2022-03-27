# import modules
import numpy as np
import pandas as pd

# Load EM file
loadModel( 'BASE_MODEL.em' )

# Create stubs
systems = ["compartment", "hepatocyte"]
SystemPath_list = ["/{}_{}".format(system, n) for system in systems for n in range(1,9)]
Target_Properties = { 'Variable': ['MolarConc',]}
Stub_dict = {}
Logger_dict = {}

for a_SystemPath in SystemPath_list:
    for E_type in ('Variable', 'Process'):
        for E in getEntityList( E_type, a_SystemPath ):
                FullID = ':'.join( ( E_type, a_SystemPath, E ) )
                Stub_dict[ FullID ] = createEntityStub( FullID )

for a_SystemPath in SystemPath_list:
    num = a_SystemPath.strip("/").strip("compartment_").strip("hepatocyte_")
    for E_type, Properties in Target_Properties.items():
        for E in getEntityList( E_type, a_SystemPath ):
            for p in Properties:
                FullID = ':'.join( ( E_type, a_SystemPath, E ) )
                stub = createEntityStub( FullID )
                FullPN = ':'.join( ( E_type, a_SystemPath, E, p ) )
                Logger = createLoggerStub( FullPN )
                Logger.create()
                # Logger Policy
                aloggingppolicy = (0.1, 0.999, 0, 209715200)
                Logger.setLoggerPolicy( aloggingppolicy )


# Run
run( 14400 )

# Get data and output
df = pd.DataFrame()
for a_SystemPath in SystemPath_list:
    num = a_SystemPath.strip("/").strip("compartment_").strip("hepatocyte_")
    for E_type, Properties in Target_Properties.items():
        for E in getEntityList( E_type, a_SystemPath ):
            for p in Properties:
                FullPN = ':'.join( ( E_type, a_SystemPath, E, p ) )
                Logger = createLoggerStub( FullPN )
                Logger.create()
                # output edc aNewFile
                aDataFile = np.array( Logger.getData() )
                if not( E  in ["SIZE", "GR", "IR"]):
                    df["{E}_{num}".format(E = E, num = num, p = p)] = aDataFile[:,1]

df.to_csv("./result.csv")

print "DONE!!"