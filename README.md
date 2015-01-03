CAProject_MIPSSimulator
=======================

This code achieves a MIPS similator (Tomasulo algorihm for pipeline)

The entrance is in MIPsSimulator.cpp
How to use

- Go to the /src folder
- >> make
- Go to the /bin folder. The executable file is here
- >> ./MIPSsim inputfilename outputfilename -{sim|dis} [-Tm:n]
  - dis disassembles the inputfile to MIPS assamble language
  - sim simulates how the pipeline works in every stage
  - if error format, it will return the format requirement
  - [-Tm:n] is optional. It will output all the cycles in default.
  - So the Sample Output.txt in /src folder is the previous one in the Website


-------------------------------------------------
