<project xmlns="com.autoesl.autopilot.project" name="gamma_correction" top="gammacorrection_accel">
    <files>
        <file name="xf_gammacorrection_accel.cpp" sc="0" tb="false" cflags="-D__SDSVHLS__ -I../../L1/include/ --std=c++0x" blackbox="false"/>
        <file name="../../data/128x128.png" sc="0" tb="1" cflags=""/>
        <file name="../../data/ref_Y.png" sc="0" tb="1" cflags=""/>
        <file name="../../xf_gammacorrection_config.h" sc="0" tb="1" cflags="-D__SDSVHLS__ -I../../../../L1/include --std=c++0x"/>
        <file name="../../xf_gammacorrection_tb.cpp" sc="0" tb="1" cflags="-D__SDSVHLS__ -I../../../../L1/include --std=c++0x"/>
    </files>
    <includePaths/>
    <libraryPaths/>
    <Simulation argv="ref_Y.png">
        <SimFlow name="csim" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

