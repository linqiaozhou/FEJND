The source codes should be changed include fllows:
1) TAppEncTop.cpp     New and free memory;
2) TEncSlice.cpp      Calling the function to get FEJND;
3£©TComDataCU.cpp     Get the ID of LCU (g_CuAddr);
4) TComRdCost.cpp     Caculate the Rdcost;
5) The file name start with"Lin" is the source codes to calculate the FEJND.
6) It shoule include the opencv3.0.0