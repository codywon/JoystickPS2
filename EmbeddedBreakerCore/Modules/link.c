#include "stdafx.h"
//code by </MATRIX>@Neod Anderjon
//author: Neod Anderjon
//====================================================================================================
/*
	模块对框架EmbeddBreakerCore的链接
	该文件写入对框架的函数调用支持
*/

#define PS2TimerInterval	20000						//扫描触发时间设置
PS2_PrintKeyValue 			PS2P_Switch;

//选项设置，链接到Universal_Resource_Config函数的模块库
void Modules_UniResConfig (void)
{
	//该函数设置内容可以更新Universal_Resource_Config函数原设置
	PS2P_Switch 		= PS2P_Enable;					//PS2P_Enable		PS2P_Disable
}

//模块选项映射表，链接到urcMapTable_Print函数
void Modules_URCMap (void)
{
	printf("\r\n%02d 	PS2 Joystick Print", urc_ps2p);
    usart1WaitForDataTransfer();
}

//选项处理，链接到pclURC_DebugHandler函数
void Modules_urcDebugHandler (u8 ed_status, Modules_SwitchNbr sw_type)
{
   //使用前请先更新Modules_SwitchNbr内容
	switch (sw_type)
    {
	case urc_ps2p:	PS2P_Switch		= (PS2_PrintKeyValue)ed_status; break;
    }
}

//协议调用指令响应，链接到OrderResponse_Handler函数
void Modules_ProtocolTask (void)
{
	
}

//OLED常量显示屏，链接到OLED_DisplayInitConst和UIScreen_DisplayHandler函数
void OLED_ScreenModules_Const (void)
{
	OLED_ShowString(strPos(0u), ROW1, (const u8*)" Joystick PS2  ", Font_Size);
    OLED_ShowString(strPos(0u), ROW2, (const u8*)"SonyPlaystation", Font_Size);
    OLED_Refresh_Gram();
}

//OLED模块调用数据显示，链接到UIScreen_DisplayHandler函数
void OLED_DisplayModules (void)
{
	//显示键值
	OLED_ShowString(strPos(0u), ROW1, (const u8*)"KeyValueMap: ", Font_Size);
	OLED_ShowNum(strPos(13u), ROW1, globalPS2keyValue, 2u, Font_Size);
	//显示摇杆模拟值
	OLED_ShowNum(strPos(0u), ROW2, KeyValueCache[ps2lx], 3u, Font_Size);
	OLED_ShowNum(strPos(4u), ROW2, KeyValueCache[ps2ly], 3u, Font_Size);
	OLED_ShowNum(strPos(8u), ROW2, KeyValueCache[ps2rx], 3u, Font_Size);
	OLED_ShowNum(strPos(12u), ROW2, KeyValueCache[ps2ry], 3u, Font_Size);
    OLED_Refresh_Gram();
}

//硬件底层初始化任务，链接到bspPeriSysCalls函数
void Modules_HardwareInit (void)
{
	PS2_InterfaceInit();
}

//硬件底层外部中断初始化，链接到EXTI_Config_Init函数
void Modules_ExternInterruptInit (void)
{
	
}

//外部中断任务，无需声明，使用时修改函数名
void EXTIx_IRQHandler (void)
{
#if SYSTEM_SUPPORT_OS 												
	OSIntEnter();    
#endif
	
	
	
#if SYSTEM_SUPPORT_OS 												
	OSIntExit();  											 
#endif
}

//模块非中断任务，链接到local_taskmgr.c，默认添加到第二任务
void Modules_NonInterruptTask (void)
{
	
}

//模块中断任务，链接到time_base.c TIM2_IRQHandler函数中
void Modules_InterruptTask (void)
{
	static u16 ps2ScanSem = 0u;
	
	if (ps2ScanSem++ == TickDivsIntervalus(PS2TimerInterval) - 1 
		&& pwsf != JBoot)
	{
		ps2ScanSem = 0u;
		PS2_JoyStickResponseHandler();			
	}
}

//====================================================================================================
//code by </MATRIX>@Neod Anderjon