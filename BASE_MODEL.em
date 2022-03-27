
Stepper FixedODE1Stepper( ODE ){StepInterval 1.0e-1;}

System System( / )
{
    StepperID    ODE;

    Variable Variable( SIZE ) {  Value  10.0;  } # dummy
}

System System( /compartment_0 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    4.0;
    }

    ### Variables ###

    Variable Variable( gcb )
    {
        Name "Glucose in compartment_0(Rest of Body)";
        MolarConc        5000.0;
        Fixed                 0;
    }

    Variable Variable( glyb )
    {
        Name "Glycerol in compartment_0(Rest of Body)";
        MolarConc          40.0;
        Fixed                 0;
    }

    Variable Variable( lac )
    {
        Name "Lactate in compartment_0(Rest of Body)";
        MolarConc       1000.0;
        Fixed                0;
    }

    Variable Variable( ffa )
    {
        Name "Free Fatty Acids in compartment_0(Rest of Body)";
        MolarConc         500.0;
        Fixed                 0;
    }

    Variable Variable( tgb )
    {
        Name "Triglycerides in compartment_0(Rest of Body)";
        MolarConc        1050.0;
        Fixed                 0;
    }

    Variable Variable( oxy )
    {
        MolarConc          76.4225311279296;
        Fixed                 0;
    }

    Variable Variable( ins )
    {
        Name "Insulin in compartment_0(Rest of Body)";
        MolarConc           0.0;
        Fixed                 0;
    }

    Variable Variable( L )
    {
        Name "Glucagon in compartment_0(Rest of Body)";
        MolarConc           0.0;
        Fixed                 0;
    }

    Variable Variable( lipoi )
    {
        Name "Insulin effect on lipolysis in compartment_0(Rest of Body)";
        Value               0.0;
        Fixed                 0;
    }

    Variable Variable( lipol )
    {
        Name "Glucagon effect on lipolysis in compartment_0(Rest of Body)";
        Value               0.0;
        Fixed                 0;
    }

    Variable Variable( trii )
    {
        Name "Insulin effect on triglyceride synthesis in compartment_0(Rest of Body)";
        Value               0.0;
        Fixed                 0;
    }

    Variable Variable( tril )
    {
        Name "Glucagon effect on triglyceride synthesis in compartment_0(Rest of Body)";
        Value               0.0;
        Fixed                 0;
    }


    ### Time Control for Input of Glucose and FFAs ###

    Variable Variable( t )
    {
        Value  0.0;
    }

     Process ExpressionFluxProcess( clock )
    {
        Name "Time value for input Process";
        Expression "1.0";
        VariableReferenceList [t :.:t 1];
    }

    ### Blood flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:gcb                1 ]
        [ before  :/compartment_8:gcb  -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:glyb                1 ]
        [ before  :/compartment_8:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:lac                 1 ]
        [ before  :/compartment_8:lac   -1 ];
    }

    Process MassActionFluxProcess( fabf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:ffa                 1 ]
        [ before  :/compartment_8:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:tgb                 1 ]
        [ before  :/compartment_8:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:ins                 1 ]
        [ before  :/compartment_8:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:L                   1 ]
        [ before  :/compartment_8:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 0.03; # 1.2 / 40.0
        VariableReferenceList
        [ here    :.:oxy                 1 ]
        [ before  :/compartment_8:oxy   -1 ];
    }

    ### Meals ###

    Process GlucoseInputFluxProcess( gcbinput )
     {
         VariableReferenceList
         [ gcb  :.:gcb     1 ]
         [ time :.:t       0 ];
     }
     
    Process FattyAcidsInputFluxProcess( ffainput )
     {
         VariableReferenceList
         [ ffa  :.:ffa     1 ]
         [ time :.:t       0 ];
     }
        
    ### Oxygen input ###

    Process OxygenInputFluxProcess( oxyinput )
    {
        VariableReferenceList
        [ oxy  :.:oxy   1 ];
    }
  ###
    ### Hormone secretion ( Insulin and Glucagon )

    Process GlucagonSecretionFluxProcess( Linput )
    {
        Name "Glucagon_endocrine";
        VariableReferenceList
        [ gcb  :.:gcb     0 ]
        [ L    :.:L       1 ];
    }

    Process InsulinSecretionFluxProcess( insinput )
    {
        Name "Insulin_endocrine";
        VariableReferenceList
        [ gcb  :.:gcb     0 ]
        [ ins  :.:ins     1 ];
    }

    ### Consumption of Glucose and FFAs in RoB ###

    Process GlucoseConsumptionFluxProcess( gcbuse )
     {
        VariableReferenceList
        [ gcb  :.:gcb    -1 ];
     }

    Process FattyAcidsConsumptionFluxProcess( ffause )
     {
        VariableReferenceList
        [ ffa  :.:ffa    -1 ]
        [ ins  :.:ins     0 ]
        [ L    :.:L       0 ];
     }

    ### Adipose metabolism ###

     Process DenovoLipogenesisFluxProcess( denovog )
     {
        Name "Adipose de novo lipogenesis/De novo synthesis";
        VariableReferenceList
        [ gcb  :.:gcb    -1 ]
        [ ffa  :.:ffa     4 ]
        [ ins  :.:ins     0 ]
        [ L    :.:L       0 ];
     }

     Process AdiposeLipolysisFluxProcess( lipolyw )
     {
        Name "Adipose lipolysis";
        VariableReferenceList
        [ tgb   :.:tgb   -1 ]
        [ ffa   :.:ffa    3 ]
        [ glyb  :.:glyb   1 ]
        [ lipoi :.:lipoi  0 ]
        [ lipol :.:lipol  0 ];
     }

     Process LipoiFluxProcess( lipoi )
     {
        Name "Insulin effect for adipose lipolysis";
        VariableReferenceList
        [ ins   :.:ins    0 ]
        [ lipoi :.:lipoi  1 ];
     }

     Process LipolFluxProcess( lipol )
     {
        Name "Glucagon effect for adipose lipolysis";
        VariableReferenceList
        [ L     :.:L      0 ]
        [ lipol :.:lipol  1 ];
     }

     Process IntestialTrisynFluxProcess( trisynw )
     {
        Name "Triglycerides synthesis in intestine";
        VariableReferenceList
        [  gcb  :.:gcb   -1 ]
        [  ffa  :.:ffa   -6 ]
        [  tgb  :.:tgb    2 ]
        [ trii  :.:trii   0 ]
        [ tril  :.:tril   0 ];
     }

     Process TriiFluxProcess( trii )
     {
        Name "Insulin effect of Triglyceride synthesis in intestine";
        VariableReferenceList
        [ ins   :.:ins    0 ]
        [ trii  :.:trii   1 ];
     }

     Process TrilFluxProcess( tril )
     {
        Name "Glucagon effect of adipose lipolysis";
        VariableReferenceList
        [ L     :.:L      0 ]
        [ tril  :.:tril   1 ];
     }

}

System System( /compartment_1 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      68.3870544433593;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_0:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_0:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_0:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_0:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_0:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_0:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_0:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_0:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_1)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 16243.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_1";
        MolarConc 0.8576158092556811;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_1";
        MolarConc 1.1423841907443189;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_1 to hepatocyte_1";
        VariableReferenceList
        [ gcb   :/compartment_1:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_1 to hepatocyte_1";
        VariableReferenceList
        [ glyb  :/compartment_1:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_1 to hepatocyte_1";
        VariableReferenceList
        [ lac   :/compartment_1:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_1 to hepatocyte_1";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_1:oxy     0 ]
        [ ffa   :/compartment_1:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_1:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_1 to hepatocyte_1( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_1:oxy     0 ]
        [ tgb   :/compartment_1:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_1:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_1:L      0 ]
        [ ins  :/compartment_1:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_1";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_1";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_1";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_1";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_1";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_2 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      61.1964683532714;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_1:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_1:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_1:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_1:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_1:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_1:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_1:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_1:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_2)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 16261.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_2";
        MolarConc 0.8738444810769694;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_2";
        MolarConc 1.1261555189230306;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_2 to hepatocyte_2";
        VariableReferenceList
        [ gcb   :/compartment_2:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_2 to hepatocyte_2";
        VariableReferenceList
        [ glyb  :/compartment_2:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_2 to hepatocyte_2";
        VariableReferenceList
        [ lac   :/compartment_2:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_2 to hepatocyte_2";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_2:oxy     0 ]
        [ ffa   :/compartment_2:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_2:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_2 to hepatocyte_2( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_2:oxy     0 ]
        [ tgb   :/compartment_2:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_2:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_2:L      0 ]
        [ ins  :/compartment_2:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_2";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_2";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_2";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_2";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_2";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_3 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      54.7619400024414;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_2:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_2:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_2:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_2:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_2:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_2:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_2:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_2:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_3)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 16537.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_3";
        MolarConc 0.910447856187933;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_3";
        MolarConc 1.089552143812067;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_3 to hepatocyte_3";
        VariableReferenceList
        [ gcb   :/compartment_3:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_3 to hepatocyte_3";
        VariableReferenceList
        [ glyb  :/compartment_3:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_3 to hepatocyte_3";
        VariableReferenceList
        [ lac   :/compartment_3:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_3 to hepatocyte_3";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_3:oxy     0 ]
        [ ffa   :/compartment_3:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_3:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_3 to hepatocyte_3( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_3:oxy     0 ]
        [ tgb   :/compartment_3:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_3:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_3:L      0 ]
        [ ins  :/compartment_3:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_3";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_3";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_3";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_3";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_3";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_4 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      49.0039710998535;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_3:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_3:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_3:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_3:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_3:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_3:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_3:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_3:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_4)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 17251.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_4";
        MolarConc 0.9691468567102426;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_4";
        MolarConc 1.0308531432897574;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_4 to hepatocyte_4";
        VariableReferenceList
        [ gcb   :/compartment_4:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_4 to hepatocyte_4";
        VariableReferenceList
        [ glyb  :/compartment_4:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_4 to hepatocyte_4";
        VariableReferenceList
        [ lac   :/compartment_4:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_4 to hepatocyte_4";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_4:oxy     0 ]
        [ ffa   :/compartment_4:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_4:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_4 to hepatocyte_4( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_4:oxy     0 ]
        [ tgb   :/compartment_4:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_4:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_4:L      0 ]
        [ ins  :/compartment_4:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_4";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_4";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_4";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_4";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_4";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_5 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      43.8514289855957;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_4:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_4:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_4:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_4:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_4:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_4:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_4:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_4:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_5)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 18303.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_5";
        MolarConc 1.032973388618647;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_5";
        MolarConc 0.967026611381353;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_5 to hepatocyte_5";
        VariableReferenceList
        [ gcb   :/compartment_5:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_5 to hepatocyte_5";
        VariableReferenceList
        [ glyb  :/compartment_5:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_5 to hepatocyte_5";
        VariableReferenceList
        [ lac   :/compartment_5:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_5 to hepatocyte_5";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_5:oxy     0 ]
        [ ffa   :/compartment_5:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_5:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_5 to hepatocyte_5( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_5:oxy     0 ]
        [ tgb   :/compartment_5:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_5:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_5:L      0 ]
        [ ins  :/compartment_5:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_5";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_5";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_5";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_5";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_5";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_6 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      39.2406539916992;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_5:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_5:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_5:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_5:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_5:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_5:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_5:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_5:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_6)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 19505.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_6";
        MolarConc 1.0880721652568597;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_6";
        MolarConc 0.9119278347431404;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_6 to hepatocyte_6";
        VariableReferenceList
        [ gcb   :/compartment_6:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_6 to hepatocyte_6";
        VariableReferenceList
        [ glyb  :/compartment_6:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_6 to hepatocyte_6";
        VariableReferenceList
        [ lac   :/compartment_6:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_6 to hepatocyte_6";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_6:oxy     0 ]
        [ ffa   :/compartment_6:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_6:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_6 to hepatocyte_6( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_6:oxy     0 ]
        [ tgb   :/compartment_6:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_6:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_6:L      0 ]
        [ ins  :/compartment_6:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_6";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_6";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_6";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_6";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_6";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_7 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      35.1146774291992;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_6:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_6:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_6:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_6:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_6:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_6:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_6:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_6:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_7)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 20716.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_7";
        MolarConc 1.1325214794274197;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_7";
        MolarConc 0.8674785205725802;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_7 to hepatocyte_7";
        VariableReferenceList
        [ gcb   :/compartment_7:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_7 to hepatocyte_7";
        VariableReferenceList
        [ glyb  :/compartment_7:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_7 to hepatocyte_7";
        VariableReferenceList
        [ lac   :/compartment_7:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_7 to hepatocyte_7";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_7:oxy     0 ]
        [ ffa   :/compartment_7:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_7:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_7 to hepatocyte_7( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_7:oxy     0 ]
        [ tgb   :/compartment_7:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_7:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_7:L      0 ]
        [ ins  :/compartment_7:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_7";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_7";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_7";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_7";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_7";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}

System System( /compartment_8 )
{
    StepperID    ODE;

    Variable Variable( SIZE )
    {
        Value    0.1;
    }

    Variable Variable( gcb )
    {
        MolarConc    5000.0;
        Fixed             0;
    }

    Variable Variable( glyb )
    {
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( lac )
    {
        MolarConc    1000.0;
        Fixed             0;
    }

    Variable Variable( ffa )
    {
        MolarConc     500.0;
        Fixed             0;
    }

    Variable Variable( tgb )
    {
        MolarConc    1050.0;
        Fixed             0;
    }

    Variable Variable( oxy )
    {
        MolarConc      31.4225311279296;
        Fixed             0;
    }

    Variable Variable( L )
    {
        MolarConc       0.0;
        Fixed             0;
    }

    Variable Variable( ins )
    {
        MolarConc       0.0;
        Fixed             0;
    }


    ### Blood Flow ###

    Process MassActionFluxProcess( gcbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:gcb                        1 ]
        [ before  :/compartment_7:gcb   -1 ];
    }

    Process MassActionFluxProcess( glybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:glyb                       1 ]
        [ before  :/compartment_7:glyb  -1 ];
    }

    Process MassActionFluxProcess( lacbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:lac                        1 ]
        [ before  :/compartment_7:lac   -1 ];
    }

    Process MassActionFluxProcess( ffabf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ffa                        1 ]
        [ before  :/compartment_7:ffa   -1 ];
    }

    Process MassActionFluxProcess( tgbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:tgb                        1 ]
        [ before  :/compartment_7:tgb   -1 ];
    }

    Process MassActionFluxProcess( insbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:ins                        1 ]
        [ before  :/compartment_7:ins   -1 ];
    }

    Process MassActionFluxProcess( Lbf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:L                          1 ]
        [ before  :/compartment_7:L     -1 ];
    }

    Process MassActionFluxProcess( oxybf )
    {
        k 1.2;
        VariableReferenceList
        [ here    :.:oxy                        1 ]
        [ before  :/compartment_7:oxy   -1 ];
    }

    ### Consumption of Insulin and Glucagon in sinusoid ###

    Process InsulinConsumptionFluxProcess( insuse )
    {
        Name "Insulin consumption";
        VariableReferenceList
        [ ins :.:ins     -1 ];
    }

    Process GlucagonConsumptionFluxProcess( Luse )
    {
        Name "Glucagon consumption";
        VariableReferenceList
        [ L   :.:L       -1 ];
    }

    Process OxygenConsumptionFluxProcess( oxyuse )
    {
        Name "Oxygen consumption";
        VariableReferenceList
        [ oxy :.:oxy     -1 ];
    }

}

System System( /hepatocyte_8)
{
    StepperID    ODE;

    ###VARIABLEs

    Variable Variable( SIZE )
    {
        Name "Volume of hepatocyte";
        Value            0.4;
    }

    Variable Variable( g )
    {
        Name "Glycogen";
        MolarConc  200000.0;
        Fixed             0;
    }

    Variable Variable( gc )
    {
        Name "Glucose";
        MolarConc    7500.0;
        Fixed             0;
    }

    Variable Variable( g3p )
    {
        Name "G3P/GADP-Glycerol-3-Phoshate/Glyceraldehyde-3-Phosphate";
        MolarConc     480.0;
        Fixed             0;
    }

    Variable Variable( g6p )
    {
        Name "Glucose-6-Phosphate";
        MolarConc     102.0;
        Fixed             0;
    }

    Variable Variable( gly )
    {
        Name "Glycerol";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( pyr )
    {
        Name "Pyruvate/Lactate";
        MolarConc    1200.0;
        Fixed             0;
    }

    Variable Variable( fa )
    {
        Name "Fatty Acid";
        MolarConc      50.0;
        Fixed             0;
    }

    Variable Variable( tg )
    {
        Name "Triglyceride";
        MolarConc 21835.0;
        Fixed             0;
    }

    Variable Variable( acoa )
    {
        Name "Acetyl-CoA";
        MolarConc      40.0;
        Fixed             0;
    }

    Variable Variable( atp )
    {
        Name "ATP-Adenosine Triphosphate";
        MolarConc    2838.0;
        Fixed             0;
    }

    Variable Variable( adp )
    {
        Name "ADP-Adenosine Diphosphate";
        MolarConc     837.0;
        Fixed             0;
    }

    Variable Variable( amp )
    {
        Name "AMP-Adenosine Monophosphate";
        MolarConc     247.0;
        Fixed             0;
    }

    Variable Variable( gtp )
    {
        Name "GTP-Guanosine Triphosphate";
        MolarConc     284.0;
        Fixed             0;
    }

    Variable Variable( gdp )
    {
        Name "GDP-Guanosine Diphosphate";
        MolarConc      91.0;
        Fixed             0;
    }

    Variable Variable( utp )
    {
        Name "UTP-Uridine Triphosphate";
        MolarConc     278.0;
        Fixed             0;
    }

    Variable Variable( udp )
    {
        Name "UDP-Uridine Diphosphate";
        MolarConc     115.0;
        Fixed             0;
    }

    Variable Variable( phos )
    {
        Name "Phosphate";
        MolarConc    3784.0;
        Fixed             0;
    }

    Variable Variable( IR )
    {
        Name "Insulin_receptor_8";
        MolarConc 1.1670492278257292;
        Fixed                  0;
    }

    Variable Variable( GR )
    {
        Name "Glucagon_receptor_8";
        MolarConc 0.8329507721742708;
        Fixed                  0;
    }

    ### Membrane transport  ###

    Process GlutFluxProcess( gct )
    {
        Name "Glucose Transport from compartment_8 to hepatocyte_8";
        VariableReferenceList
        [ gcb   :/compartment_8:gcb    -1 ]
        [ gc    :.:gc                        1 ];
    }

    Process GlytFluxProcess( glyt )
    {
        Name "Glycerol Transport from compartment_8 to hepatocyte_8";
        VariableReferenceList
        [ glyb  :/compartment_8:glyb   -1 ]
        [ gly   :.:gly                       1 ];
    }

    Process LactFluxProcess( lact )
    {
        Name "Lactate Transport from compartment_8 to hepatocyte_8";
        VariableReferenceList
        [ lac   :/compartment_8:lac    -1 ]
        [ pyr  :.:pyr                      1 ];
    }

    Process FAtFluxProcess( fat )
    {
        Name "FFA Transport from compartment_8 to hepatocyte_8";
        V_FAT_INS @(0.08); # defalut: 0.08
        V_FAT_DIF @(1.45); # defalut: 1.45

        VariableReferenceList
        [ oxy   :/compartment_8:oxy     0 ]
        [ ffa   :/compartment_8:ffa    -1 ]
        [ fa    :.:fa                        1 ]
        [ ins   :/compartment_8:ins     0 ]
        [ IR   :.:IR                         0 ];
    }

    Process TGtFluxProcess( tgt )
    {
        Name "Triglycerides Transport from compartment_8 to hepatocyte_8( tgta + tgtp )";
        V_TGT_DIF @(0.6); # defalut: 0.6
        V_TGT_VLDL @(0.3); # defalut: 0.3

        VariableReferenceList
        [ oxy   :/compartment_8:oxy     0 ]
        [ tgb   :/compartment_8:tgb    -1 ]
        [ tg    :.:tg                        1 ];
    }

    ### Hepatic metabolism  ###

    Process GlyPFKFluxProcess( pfk )
    {
        Name "Glycolysis stage 1 by PDK(1.1.1)";
        V_PFK @(160.0); # defalut: 160.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ g3p  :.:g3p                      2 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AcoaSynFluxProcess( asyn )
    {
        Name "Pyruvate Oxidation / Acoa Synthesis(1.1.2)";
        V_ASYN @(14.0); # defalut: 14.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ pyr :.:pyr                    -2 ]
        [ atp  :.:atp                      5 ]
        [ adp  :.:adp                     -5 ]
        [ acoa :.:acoa                     2 ]
        [ phos :.:phos                    -5 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process TriSynFluxProcess( trisyn )
    {
        Name "Triglyceride Synthesis(1.1.3)";
        V_TRISYN @(9.0); # defalut: 9.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g3p  :.:g3p                     -1 ]
        [ fa   :.:fa                      -3 ]
        [ atp  :.:atp                     -3 ]
        [ amp  :.:amp                      3 ]
        [ phos :.:phos                     7 ]
        [ tg   :.:tg                       1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process LipolysisFluxProcess( lipoly )
    {
        Name "Lipolysis(1.1.4)";
        V_LIPOLY @(0.085); # defalut: 0.085

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ fa   :.:fa                       3 ]
        [ tg   :.:tg                      -1 ]
        [ gly  :.:gly                      1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlyConvFluxProcess( glyk )
    {
        Name "Conversion of Glycerol to G3P(1.1.5)";
        V_GLYK @(5.0); # defalut: 5.0

        VariableReferenceList
        [ g3p  :.:g3p     1 ]
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ gly  :.:gly    -1 ];
    }

    Process G6PaseFluxProcess( g6pase )
    {
        Name "Glucose-6-phosphatase(G6Pase)(1.2.1)";
        V_G6PASE @(370.0); # defalut: 370.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g6p  :.:g6p                     -1 ]
        [ gc   :.:gc                       1 ]
        [ phos :.:phos                     1 ];
    }

    Process GlycogenSynFluxProcess( gsyn )
    {
        Name "GlycogenSynthase(1.2.2)";
        V_GSYN @(55.0); # defalut: 55.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g    :.:g                        1 ]
        [ g6p  :.:g6p                     -1 ]
        [ phos :.:phos                     2 ]
        [ utp  :.:utp                     -1 ]
        [ udp  :.:udp                      1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GlycogenPhosFluxProcess( gphos )
    {
        Name "Glycogen Phosphorylase(1.2.3)";
        V_GPHOS @(5.0); # defalut: 5.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g    :.:g                       -1 ]
        [ g6p  :.:g6p                      1 ]
        [ phos :.:phos                    -1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluPEPCKFluxProcess( pepck )
    {
        Name "Gluconeogenesis stage 1 by PEPCK(Pyruvate/Lactate to G3P)(1.2.4)";
        V_PEPCK @(35.0); # defalut: 35.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ pyr :.:pyr                    -1 ]
        [ g3p  :.:g3p                      1 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      2 ]
        [ gdp  :.:gdp                      1 ]
        [ gtp  :.:gtp                     -1 ]
        [ phos :.:phos                     2 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process GluFBPFluxProcess( fbp )
    {
        Name "Gluconeogenesis 2 by FBPase(G3P to G6P)(1.2.5)";
        V_FBP @(68.0); # defalut: 68.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g6p  :.:g6p                      1 ]
        [ g3p  :.:g3p                     -2 ]
        [ phos :.:phos                     1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process BoxidationFluxProcess( boxi )
    {
        Name "Acetyl-CoA from FAs(B-oxidation)(1.2.6)";
        V_BOXI @(3.3); # defalut: 3.3

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ atp  :.:atp                     -2 ]
        [ adp  :.:adp                      1 ]
        [ phos :.:phos                     3 ]
        [ acoa :.:acoa                     8 ]
        [ amp  :.:amp                      1 ]
        [ fa   :.:fa                      -1 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpSynFluxProcess( atps )
    {
        Name "ATP Synthesis via the citrate cycle and electron transport chain(1.2.7)";
        V_ATPS @(525.0); # defalut: 525.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ atp  :.:atp                     12 ]
        [ adp  :.:adp                    -12 ]
        [ acoa :.:acoa                    -1 ]
        [ phos :.:phos                   -12 ];
    }

    Process GlucoKinaseFluxProcess( gk )
    {
        Name "Glucokinase(1.3.1)";
        V_GK @(132.16); # defalut: 132.16

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ gc   :.:gc                      -1 ]
        [ g6p  :.:g6p                      1 ]
        [ atp  :.:atp                     -1 ]
        [ adp  :.:adp                      1 ];
    }

    Process LipogenesisFluxProcess( lipog )
    {
        Name "Lipogenesis(1.3.2)";
        V_LIPOG @(4.0); # defalut: 4.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ atp  :.:atp                     -7 ]
        [ adp  :.:adp                      7 ]
        [ acoa :.:acoa                    -8 ]
        [ fa   :.:fa                       1 ]
        [ phos :.:phos                     7 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process PyrKinaseFluxProcess( pk )
    {
        Name "Glycolysis stage 2 by PK(1.3.3)";
        V_PK @(87.0); # defalut: 87.0

        VariableReferenceList
        [ oxy  :/compartment_8:oxy    0 ]
        [ g3p  :.:g3p                     -4 ]
        [ pyr  :.:pyr                     4 ]
        [ phos :.:phos                    -9 ]
        [ atp  :.:atp                     13 ]
        [ adp  :.:adp                    -13 ]
        [ acoa :.:acoa                     0 ]
        [ L    :/compartment_8:L      0 ]
        [ ins  :/compartment_8:ins    0 ]
        [ IR   :.:IR                       0 ]
        [ GR   :.:GR                       0 ];
    }

    Process AtpConsumpFluxProcess( atpuse )
    {
        Name "ATP_Use_8";
        VariableReferenceList
        [ atp  :.:atp    -1 ]
        [ adp  :.:adp     1 ]
        [ phos :.:phos    1 ];
    }

    Process AdenoKinaseFluxProcess( ak )
    {
        Name "Adenosine_Kinase_8";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ amp  :.:amp  -1 ]
        [ adp  :.:adp   2 ];
    }

    Process PhosControlFluxProcess( phoscntl )
    {
        Name "Phosphate_Control_8";
        VariableReferenceList
        [ phos :.:phos   -1 ];
    }

    Process NDKGFluxProcess( ndkg )
    {
        Name "Nucleoside diphosphate kinases (GDP => GTP)_8";
        VariableReferenceList
        [ atp  :.:atp  -1 ]
        [ gdp  :.:gdp  -1 ]
        [ adp  :.:adp   1 ]
        [ gtp  :.:gtp   1 ];
    }

    Process NDKUFluxProcess( ndku )
    {
        Name "Nucleoside diphosphate kinases (UDP => UTP)_8";
        VariableReferenceList
        [ atp  :.:atp     -1 ]
        [ udp  :.:udp     -1 ]
        [ adp  :.:adp      1 ]
        [ utp  :.:utp      1 ];
    }

}
