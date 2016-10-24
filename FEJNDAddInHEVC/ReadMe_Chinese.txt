更改的文件有：
1) TAppEncTop.cpp 开辟和释放内存
2) TEncSlice.cpp  在该文件中调用计算FEJND的代码
3）TComDataCU.cpp 得到LCU的位置（g_CuAddr）;
4) TComRdCost.cpp 计算Rdcost
5) 以Lin开头的文件就是就算FEJND的源文件
6) 运行该代码需要包含opencv3.0.0