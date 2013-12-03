google breakpad 测试
Win7 64  VS 2005

注意:
1.项目字符集需要使用Unicode字符集
2.VS的dbghelp.h可能会缺少一些定义，可以引用third_part/platformsdk_win7中的DbgHelp.h来解决
  不过新下载的breakpad项目已经移除了platformsdk_win7

