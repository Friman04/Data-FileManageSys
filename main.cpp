/*******************************************
 * @note    该版本仅供提交大作业使用！
 * @author  电创2202-吴玮彤-20221072060
 * @date    2023.01.12
 * @envir	Windows 11 dev_Build 25272.rs_prerelease.221216-1237 | Visual Studio 2022 | EasyX_20220901 | HiEasyX Ver 0.3.0
 * @ver     0.1alpha3
 *
 * @note    本项目使用了基于 EasyX 的扩展 HiEasyX，请确保环境中安装了 EasyX
 *          程序只能在 Windows 环境下运行，应该在 Windows 10/11 和 Visual Studio 下编译程序，暂不支持 MinGW 编译器，其它环境未测试，不保证程序能正常运行
 * 
 *******************************************/

#define _CRT_SECURE_NO_WARNINGS

#define _SYS_VER_STR_	L"Ver 0.1Alpha2"


#include <io.h>
#include <stdio.h>
#include <direct.h>

#define MAX_LEN 4096
#include <stdio.h>
#include <string.h>
#include "HiEasyX.h"
//#include<graphics.h>

#ifdef UNICODE  //设置多字节字符集
#undef UNICODE
#endif

//
// 超参数区
//

// 窗体大小
#define ASPECT_RATIO 1.778                  // 宽高比
#define WINDOW_HEI 720                      // 窗体高度大小
#define WINDOW_WID WINDOW_HEI*ASPECT_RATIO  // 窗体宽度大小

// 平面设计中的常数
#define PHI 0.618
#define EX_LEFT (PHI - 0.5) / 2
#define MID_LEFT (1 - PHI) / 2

// 字体居中
#define TOP_LEFT     1
#define TOP          2
#define TOP_RIGHT    3
#define LEFT         4
#define CENTER       5
#define RIGHT        6
#define BOTTOM_LEFT  7
#define BOTTOM       8
#define BOTTOM_RIGHT 9

//
// 自变量声明区
//

char token; // 未使用

typedef struct
{
	char id[16];
	char pwd[20];
	int value1;
	int value2;
	int sex;
}user;

typedef struct
{int x, y;}intPair;


//
// 核心函数区
//

/**
 * @brief 计算对齐坐标
 * @param[in] x		            背景对象的x坐标
 * @param[in] y			        背景对象的y坐标
 * @param[in] w		            背景对象的宽度
 * @param[in] h			        背景对象的高度
 * @param[in] wid				被绘制对象的宽度
 * @param[in] hei   			被绘制对象的高度
 * @param[in] align_mode   		对齐方式（默认居中对齐）
 * @return                      被绘制对象的坐标
*/
intPair align(int x, int y, int w, int h, int wid, int hei, int align_mode = CENTER)
{
    intPair pos;
    switch (align_mode)
    {
        case 1:
        {
            pos.x = x;
            pos.y = y;
            return pos;
        }
        case 2:
        {
            pos.x = x + w / 2 - wid / 2;
            pos.y = y;
            return pos;
        }
        case 3:
        {
            pos.x = x + w - wid;
            pos.y = y;
            return pos;
        }
        case 4:
        {
            pos.x = x;
            pos.y = y + h / 2 - hei / 2;
            return pos;
        }
        case 5:
        {
            pos.x = x + w / 2 - wid / 2,
            pos.y = y + h / 2 - hei / 2;
            return pos;
        }
        case 6:
        {
            pos.x = x + w - wid;
            pos.y = y + h / 2 - hei / 2;
            return pos;
        }
        case 7:
        {
            pos.x = x;
            pos.y = y + h - hei;
            return pos;
        }
        case 8:
        {
            pos.x = x + w / 2 - wid / 2;
            pos.y = y + h - hei;
            return pos;
        }
        case 9:
        {
            pos.x = x + w - wid;
            pos.y = y + h - hei;
            return pos;
        }
    }
}


/**
 * @brief 在矩形区域内加载图像文件并对齐绘制
 * @param[in] canvas		    Canvas类的实例化
 * @param[in] img_file			图片文件位置
 * @param[in] iWidth			图片目标拉伸宽度（为0表示不拉伸）
 * @param[in] iHeight		    图片目标拉伸高度（为0表示不拉伸）
 * @param[in] pivot_x			锚点x坐标（即区域左上角x坐标）
 * @param[in] pivot_y			锚点y坐标（即区域左上角y坐标）
 * @param[in] wid				区域宽度
 * @param[in] hei   			区域高度
 * @param[in] align_mode   		对齐方式（默认居中对齐）
 * @param[in] alpha	            叠加透明度（默认透明度100%/255U）
 * @param[in] isAlpha       	是否开启混合图像透明通道（默认是）
 * @return                      读取到的IMAGE对象
*/
IMAGE imageAlign_alpha(hiex::Canvas& canvas,
                       LPCTSTR img_file,
                       int iWidth,
                       int iHeight,
                       int pivot_x,
                       int pivot_y,
                       int wid,
                       int hei,
                       int align_mode = CENTER,
                       BYTE alpha = 255U,
                       bool isAlpha = true)
{
    // 锚点应设置在区域的左上角
    IMAGE img;
    intPair text_pos = align(pivot_x, pivot_y, wid, hei, iWidth, iHeight, align_mode);
    img = canvas.Load_Image_Alpha(img_file, text_pos.x, text_pos.y, false, iWidth, iHeight, alpha, isAlpha);
    return img;
}

/**
 * @brief 在矩形区域内对齐绘制文字
 * @param[in] canvas		    Canvas类的实例化
 * @param[in] str			    绘制的字符串
 * @param[in] fHeight		    字符高度
 * @param[in] fWidth			字符宽度（如果值为0，则字符串自适应间距）
 * @param[in] font  			字体
 * @param[in] pivot_x			锚点x坐标（即区域左上角x坐标）
 * @param[in] pivot_y			锚点y坐标（即区域左上角y坐标）
 * @param[in] wid				区域宽度
 * @param[in] hei   			区域高度
 * @param[in] align_mode   		对齐方式（默认居中对齐）
 * @param[in] c	                字体颜色
 * @param[in] esc       		字符串的书写角度（单位0.1°）
 * @param[in] ori         		每个字符的书写角度（单位0.1°）
*/
void textAlign(hiex::Canvas& canvas,
               LPCTSTR str,
               int fHeight,
               int fWidth,
               LPCTSTR font,
               int pivot_x,
               int pivot_y,
               int wid,
               int hei,
               int align_mode = CENTER,
               COLORREF c = BLACK,
               LONG esc = 0,
               LONG ori = 0,
               int bg = TRANSPARENT)
{
    // 锚点应设置在区域的左上角
    canvas.SetTextEscapement(esc);
    canvas.SetTextOrientation(ori);
    canvas.SetBkMode(bg);
    canvas.SetTextStyle(fHeight, fWidth, font);

    intPair text_pos = align(pivot_x, pivot_y, wid, hei, canvas.TextWidth(str), canvas.TextHeight(str), align_mode);
    canvas.OutTextXY(text_pos.x, text_pos.y, str, true, c);
}

/**
 * @deprecated 此函数已经过时，可能在未来版本中废弃！
 * @brief 在矩形区域内对齐绘制文字
 * @param[in] canvas		    Canvas类的实例化
 * @param[in] str			    绘制的字符串
 * @param[in] fHeight		    字符宽度
 * @param[in] fWidth			字符宽度（如果值为0，则字符串自适应间距）
 * @param[in] font  			字体
 * @param[in] pivot_x			锚点x坐标（即区域左上角x坐标）
 * @param[in] pivot_y			锚点y坐标（即区域左上角y坐标）
 * @param[in] wid				区域宽度
 * @param[in] hei   			区域高度
 * @param[in] align_mode   		对齐方式（默认居中对齐）
 * @param[in] c	                字体颜色
 * @param[in] esc       		字符串的书写角度（单位0.1°）
 * @param[in] ori         		每个字符的书写角度（单位0.1°）
*/
void __TextAlign(hiex::Canvas & canvas,
                 LPCTSTR str,
                 int fHeight,
                 int fWidth,
                 LPCTSTR font,
                 int pivot_x,
                 int pivot_y,
                 int wid,
                 int hei,
                 int align_mode = CENTER,
                 COLORREF c = BLACK,
                 LONG esc = 0,
                 LONG ori = 0,
                 int bg = TRANSPARENT)
{
    // 锚点应设置在区域的左上角

    canvas.SetTextEscapement(esc);
    canvas.SetTextOrientation(ori);
    canvas.SetBkMode(bg);
    canvas.SetTextStyle(fHeight, fWidth, font);
    switch (align_mode)
    {
    case 1:
        canvas.OutTextXY(pivot_x, pivot_y, str, true, c);
        break;
    case 2:
        canvas.OutTextXY(
            pivot_x + wid / 2 - canvas.TextWidth(str) / 2,
            pivot_y,
            str,
            true, c);
        break;
    case 3:
        canvas.OutTextXY(
            pivot_x + wid - canvas.TextWidth(str),
            pivot_y,
            str,
            true, c);
        break;
    case 4:
        canvas.OutTextXY(
            pivot_x,
            pivot_y + wid / 2 - canvas.TextHeight(str) / 2,
            str,
            true, c);
        break;
    case 5:
        canvas.OutTextXY(
            pivot_x + wid / 2 - canvas.TextWidth(str) / 2,
            pivot_y + hei / 2 - canvas.TextHeight(str) / 2,
            str,
            true, c);
        break;
    case 6:
        canvas.OutTextXY(
            pivot_x + wid - canvas.TextWidth(str),
            pivot_y + hei / 2 - canvas.TextHeight(str) / 2,
            str,
            true, c);
        break;
    case 7:
        canvas.OutTextXY(
            pivot_x,
            pivot_y + hei - canvas.TextHeight(str),
            str,
            true, c);
        break;

    case 8:
        canvas.OutTextXY(
            pivot_x + wid / 2 - canvas.TextWidth(str) / 2,
            pivot_y + hei - canvas.TextHeight(str),
            str,
            true, c);
        break;
    case 9:
        canvas.OutTextXY(
            pivot_x + wid - canvas.TextWidth(str),
            pivot_y + hei - canvas.TextHeight(str),
            str,
            true, c);
        break;
    }
}

/**
 * @deprecated 此函数已经过时，在 Ver 0.1Alpha2 及以后版本废弃！
 * @brief 在矩形中对齐绘制文字
 * @param[in] x				    矩形左上角x坐标
 * @param[in] y				    矩形左上角y坐标
 * @param[in] width				矩形宽度
 * @param[in] height			矩形高度
 * @param[in] horizontal	    是否水平居中（默认是）
 * @param[in] vertical  		是否垂直居中（默认是）
*/
void __textAlign(LPCTSTR str, int x, int y, int width, int height, bool horizontal = true, bool vertical = true, int mode = TRANSPARENT)
{
    /* 使用原生EasyX绘制，存在兼容性问题，谨慎使用！*/

    LOGFONT font;
    setbkmode(mode);
    gettextstyle(&font);
    int textHeight = font.lfHeight;
    int textWidth = textHeight;
    int strWidth = 0;
    int strNum = lstrlen(str);
    for (int i = 0; i < strNum; ++i)
        strWidth += (str[i] > 127) ? textHeight : textHeight / 2;
    if (strWidth >= width || textHeight >= height)
    {
        outtextxy(x, y, str);
        return;
    }
    if (horizontal)
        x += (width - strWidth) / 2;
    if (vertical)
        y += (height - textHeight) / 2;
    outtextxy(x, y, str);
}

/*创建储存用户账号密码的文件*/
void createFile()
{
	FILE* fp;
	if ((fopen( "users.txt", "rb")) == NULL)                 // 如果此文件不存在
	{
		if ((fopen("users.txt", "wb+")) == NULL)
		{
			printf("无法建立文件！\n");
			exit(0);
		}
	}
}

int index()  
{
	printf("___________________________________\n");
	printf("Welcome!\n");
	printf("在进一步的使用前，你需要进行登录：\n");
	printf("【1】注册\n");
	printf("【2】登录\n");
	printf("___________________________________\n");
	printf("请输入对应序号：");
	
	int kbhit = getchar();
	getchar();
	return kbhit;
}

int indexInter()
{
	switch (index())
	{
	case 1:
		break;
	case 2:
		break;
	default:
		printf("无效的输入，请重新输入！");
	}

	return 0;
}

/*注册*/
void reg()  // 按结构体的大小，一次写入或者读入1个结构体大小
{
	user a, b;
	FILE* fp = NULL;
	errno_t err = 0;
	char temp[20];
	int count = 0;
	fp = fopen("users.txt", "r");
	fread(&b, sizeof(user), 1, fp);   // 用b遍历users

	printf("___________________________________\n");
	printf("欢迎来到注册界面！\n");
	Sleep(1000);
	printf("【info】用户名不可超过16个字符\n");
	printf("【info】输入q可返回\n");
	printf("请输入用户名:\n");
	scanf("%s", &a.id);
	if (*a.id == 'q')
		return;
	while (1)
	{
		if (strcmp(a.id, b.id)) // 如果ab不同
		{
			if (!feof(fp))    // 如果未到文件尾
			{
				fread(&b, sizeof(user), 1, fp);
			}
			else
				break;
		}
		else

		{
			printf("此用户名已存在！请重新注册！\n");
			Sleep(1000);
			fclose(fp);
			return;
		}
	}



	printf("请输入性别(1（女）/2（男）/3（机器人）)：\n");
	scanf("%d", &a.sex);
	while (a.sex != 1 && a.sex != 2 && a.sex != 3)
	{
		printf("如果是机器人的话请输入3哦-ω-\n");
		scanf("%d", &a.sex);
	}
	getchar();
	while (1)
	{
		printf("请输入密码\n");
		scanf(" %s", &a.pwd);
		printf("请确认密码\n");
		scanf(" %s", &temp);

		if (!strcmp(a.pwd, temp))
		{
			fp = fopen("users.txt", "a");
			fwrite(&a, sizeof(user), 1, fp);
			printf("账号注册成功，请登录！\n");
			Sleep(500);
			fclose(fp);
			return;
		}
		else
		{
			printf("两次密码不匹配！请重新输入！\n");
		}
	}
}

/*登录系统*/
void  login()
{
	user a, b;
	FILE* fp;
	fp = fopen("users.txt", "r");
	fread(&b, sizeof(user), 1, fp);
	printf("___________________________________\n");
	printf("欢迎来到登录界面！\n");
	Sleep(500);
	printf("【info】输入q可返回\n");
	printf("请输入账号\n");

	scanf("%s", &a.id);
	if (*a.id == 'q')
		return;
	while (1)
	{
		if (strcmp(a.id, b.id) == 0)         // 如果有此用户名
		{
			break;
		}
		else
		{
			if (!feof(fp))  // 如果文件没有读完                            

			{
				fread(&b, sizeof(user), 1, fp);
			}

			else
			{
				printf("此用户名不存在，请重新输入！\n");
				Sleep(500);
				fclose(fp);
				return;
			}
		}
	}
	printf("请输入密码\n");
	scanf("%s", &a.pwd);
	while (1)
	{       

		if (strcmp(a.pwd, b.pwd) == 0)
		{
			fclose(fp);
			printf("登录成功,欢迎使用!");
			//token = &a.id;  /*bug*/
			Sleep(500);
			return;
		}
		else
		{
			printf("密码不正确!请重新输入密码\n");
			scanf("%s", &a.pwd);
		}
		
	} 
}

void home()
{

    hiex::Canvas canvas_main;
    hiex::Window wnd(WINDOW_WID, WINDOW_HEI);
    wnd.BindCanvas(&canvas_main);
    hiex::AutoExit();
    HWND hwnd = wnd.GetHandle();


    while (1)
    {
        /*绘制GUI框架*/


        struct _finddata_t file;
        intptr_t hFile;
        char buf[MAX_LEN];

        hFile = _findfirst("data/*.txt", &file);
        if (hFile == -1)
            printf("没有找到文件！\n");
        while (true)
        {
            sprintf_s(buf, MAX_LEN, "%s\\%s", file.name);
            printf("%s\n", buf);
            if(_findnext(hFile, &file))
                break;
        }



        /*
        edit.PreSetStyle(true, false, true);
        edit.Create(hwnd, WINDOW_WID * EX_LEFT, WINDOW_WID * EX_LEFT, WINDOW_WID * (MID_LEFT - EX_LEFT), WINDOW_HEI - WINDOW_WID * EX_LEFT,
            L"Welcome to HiEasyX !\r\n"
            L"\r\n"
            L"Here you will experience the overall upgrade of EasyX.\r\n"
            L"Let's start HiEasyX from the simple sample.\r\n"
            L"\r\n"
            L"What's new in HiEasyX\r\n"
            L"\r\n"
            L">> You could create multi-window application with HiEasyX easily.\r\n"
            L">> You could use HiCanvas, which is a C++ canvas wrapper of EasyX, in the similar way of EasyX.\r\n"
            L">> The best support of Windows system controls integrated in HiEasyX is avalible.\r\n"
            L">> The most easy method to create a tray.\r\n"
            L"etc.\r\n"
        );
        edit.SetFont(26, 0, L"微软雅黑");
        */


        // 背景1
        canvas_main.Clear(true, WHITE);  //设置背景颜色

        // 线条
        canvas_main.SetLineStyle(PS_SOLID, 2);
        canvas_main.SetLineColor(0xEEEEEE);
        canvas_main.Line(WINDOW_WID * MID_LEFT, WINDOW_WID * EX_LEFT, WINDOW_WID * MID_LEFT, WINDOW_HEI); // 分隔资源管理器和绘图区

        // 背景2
        canvas_main.SetFillColor(0x662015);
        canvas_main.FillRectangle(0, 0, WINDOW_WID * EX_LEFT, WINDOW_HEI); // 左侧深蓝色背景
        canvas_main.SetFillColor(0xEFE1CF);
        canvas_main.FillRectangle(0, 0, WINDOW_WID, WINDOW_WID * EX_LEFT); // 顶侧浅蓝色背景

        //logo
        canvas_main.Load_Image_Alpha(L"sprites/logo_light.png", 0, 0, false, WINDOW_WID * EX_LEFT - 1, WINDOW_WID * EX_LEFT - 1, 255U, true);

        // 文字
        textAlign(canvas_main, L"工业数据分析与文件信息管理系统", 36, 0, L"微软雅黑", WINDOW_WID * EX_LEFT, 0, WINDOW_WID * (1 - EX_LEFT), WINDOW_WID * EX_LEFT); // 标题
        textAlign(canvas_main, L"Created by Friman " _SYS_VER_STR_, 16, 0, L"Arial", 0, 0, WINDOW_WID, WINDOW_HEI, BOTTOM_RIGHT, GRAY); // 信息

        // 按钮绘制

        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
        REDRAW_WINDOW();
        //按钮控件逻辑

        ExMessage m_msg;
        while (1)
        {   
            
            m_msg = getmessage(EX_MOUSE);
            
            
            if (m_msg.x > WINDOW_WID * (1 - EX_LEFT * 3 / 4) && m_msg.y > WINDOW_WID * EX_LEFT / 4 && m_msg.x < WINDOW_WID * (1 - EX_LEFT / 4) && m_msg.y < WINDOW_WID * EX_LEFT * 3 / 4) // 如果鼠标在这个区间内
            {
                switch (m_msg.message)
                {
                    case WM_LBUTTONDOWN:
                    {
                        canvas_main.SetFillColor(YELLOW);
                        canvas_main.FillRoundRect(WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * (1 - EX_LEFT / 4), WINDOW_WID * EX_LEFT * 3 / 4, 20, 20);
                        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
                        REDRAW_WINDOW();
                        break;
                    }
                    case WM_LBUTTONUP:
                    {
                        textAlign(canvas_main, _T("TEST"), 36, 0, _T("微软雅黑"), 0, 0, WINDOW_WID, WINDOW_WID); //do somthing
                        canvas_main.SetFillColor(BLUE);
                        canvas_main.FillRoundRect(WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * (1 - EX_LEFT / 4), WINDOW_WID * EX_LEFT * 3 / 4, 20, 20);
                        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
                        REDRAW_WINDOW();
                        break;
                    }
                    default:
                    {
                        // 鼠标指针经过按钮
                        canvas_main.SetFillColor(LIGHTBLUE);
                        canvas_main.FillRoundRect(WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * (1 - EX_LEFT / 4), WINDOW_WID * EX_LEFT * 3 / 4, 20, 20);
                        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
                        REDRAW_WINDOW();
                        break;
                    }
                }
            }
            else
            {
                // 默认状态
                canvas_main.ClearRoundRect(WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * (1 - EX_LEFT / 4), WINDOW_WID * EX_LEFT * 3 / 4, 20, 20);
                imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
                REDRAW_WINDOW();
            }
        }


        hiex::init_end(hwnd);
		break;
	}
}


void menu()
{
	int username = 0, statu = 0;
	printf("___________________________________\n");
	if (token != -1)
	{

		printf("Welcome!%s\n", username);
		printf("【1】Module A\n");
		printf("【2】Module B\n");
		printf("【3】退出登录\n");
	}
	else
	{
		printf("您还未登录，请先登录！");
		printf("【1】登录");
		printf("【2】注册");
	}
	printf("___________________________________\n");



	char option = getchar();
	switch (option)
	{
	case '1':
		while (1)
		{
			system("cls");
			//Menu();
			//Operate();
		}break;
	case 50:
		while (1)
		{
			system("cls");
			//Menu1();
			//Operate1();
		}break; break;
	case 48:exit(0);
	}
}

int main()
{   

	home();


    getchar();
	return 0;
}