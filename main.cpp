#include <iostream>
#include "single_instance.h"

//执行linux控制台指令，并输出控制台信息
int execCmd(std::string cmdStr)
{
    char buf[1024];
    FILE *p_file = popen(cmdStr.c_str(), "r");
    if (!p_file)
    {
        fprintf(stderr, "Erro to popen.");
        return -1;
    }
    //读取控制台输出。只有命令执行完并退出，此处才会读到输出结果
    while (fgets(buf, sizeof(buf), p_file) != NULL)
    {
        fprintf(stdout, "%s", buf);
    }
    pclose(p_file);

    return 0;
}

void mainProgram()
{
    //execCmd("cd /home/sexybug/testbug/Loader && ./loader");
    //execCmd("cd /home/Loader/ && ./loader");
    //撤销原loader的可执行权限，防止通过loader直接启动
    //system的输出直接输出到当前程序的控制台
    system("chmod +x ./loader && ./loader ; chmod -x ./loader");
    //execCmd("nohup ./loader > ./output.txt 2>&1 &");
}

//使用方法
int main(int, char **)
{
    //将会创建processName.pid文件
    std::string processName = "The program";
    if (isRunning(processName))
    {
        return 0;
    }
    //这里执行主程序
    mainProgram();

    return 0;
}
