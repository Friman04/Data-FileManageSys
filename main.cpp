/*******************************************
 * @note    �ð汾�����ύ����ҵʹ�ã�
 * @author  �紴2202-����ͮ-20221072060
 * @date    2023.01.12
 * @envir	Windows 11 dev_Build 25272.rs_prerelease.221216-1237 | Visual Studio 2022 | EasyX_20220901 | HiEasyX Ver 0.3.0
 * @ver     0.1alpha3
 *
 * @note    ����Ŀʹ���˻��� EasyX ����չ HiEasyX����ȷ�������а�װ�� EasyX
 *          ����ֻ���� Windows ���������У�Ӧ���� Windows 10/11 �� Visual Studio �±�������ݲ�֧�� MinGW ����������������δ���ԣ�����֤��������������
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

#ifdef UNICODE  //���ö��ֽ��ַ���
#undef UNICODE
#endif

//
// ��������
//

// �����С
#define ASPECT_RATIO 1.778                  // ��߱�
#define WINDOW_HEI 720                      // ����߶ȴ�С
#define WINDOW_WID WINDOW_HEI*ASPECT_RATIO  // �����ȴ�С

// ƽ������еĳ���
#define PHI 0.618
#define EX_LEFT (PHI - 0.5) / 2
#define MID_LEFT (1 - PHI) / 2

// �������
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
// �Ա���������
//

char token; // δʹ��

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
// ���ĺ�����
//

/**
 * @brief �����������
 * @param[in] x		            ���������x����
 * @param[in] y			        ���������y����
 * @param[in] w		            ��������Ŀ��
 * @param[in] h			        ��������ĸ߶�
 * @param[in] wid				�����ƶ���Ŀ��
 * @param[in] hei   			�����ƶ���ĸ߶�
 * @param[in] align_mode   		���뷽ʽ��Ĭ�Ͼ��ж��룩
 * @return                      �����ƶ��������
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
 * @brief �ھ��������ڼ���ͼ���ļ����������
 * @param[in] canvas		    Canvas���ʵ����
 * @param[in] img_file			ͼƬ�ļ�λ��
 * @param[in] iWidth			ͼƬĿ�������ȣ�Ϊ0��ʾ�����죩
 * @param[in] iHeight		    ͼƬĿ������߶ȣ�Ϊ0��ʾ�����죩
 * @param[in] pivot_x			ê��x���꣨���������Ͻ�x���꣩
 * @param[in] pivot_y			ê��y���꣨���������Ͻ�y���꣩
 * @param[in] wid				������
 * @param[in] hei   			����߶�
 * @param[in] align_mode   		���뷽ʽ��Ĭ�Ͼ��ж��룩
 * @param[in] alpha	            ����͸���ȣ�Ĭ��͸����100%/255U��
 * @param[in] isAlpha       	�Ƿ������ͼ��͸��ͨ����Ĭ���ǣ�
 * @return                      ��ȡ����IMAGE����
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
    // ê��Ӧ��������������Ͻ�
    IMAGE img;
    intPair text_pos = align(pivot_x, pivot_y, wid, hei, iWidth, iHeight, align_mode);
    img = canvas.Load_Image_Alpha(img_file, text_pos.x, text_pos.y, false, iWidth, iHeight, alpha, isAlpha);
    return img;
}

/**
 * @brief �ھ��������ڶ����������
 * @param[in] canvas		    Canvas���ʵ����
 * @param[in] str			    ���Ƶ��ַ���
 * @param[in] fHeight		    �ַ��߶�
 * @param[in] fWidth			�ַ���ȣ����ֵΪ0�����ַ�������Ӧ��ࣩ
 * @param[in] font  			����
 * @param[in] pivot_x			ê��x���꣨���������Ͻ�x���꣩
 * @param[in] pivot_y			ê��y���꣨���������Ͻ�y���꣩
 * @param[in] wid				������
 * @param[in] hei   			����߶�
 * @param[in] align_mode   		���뷽ʽ��Ĭ�Ͼ��ж��룩
 * @param[in] c	                ������ɫ
 * @param[in] esc       		�ַ�������д�Ƕȣ���λ0.1�㣩
 * @param[in] ori         		ÿ���ַ�����д�Ƕȣ���λ0.1�㣩
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
    // ê��Ӧ��������������Ͻ�
    canvas.SetTextEscapement(esc);
    canvas.SetTextOrientation(ori);
    canvas.SetBkMode(bg);
    canvas.SetTextStyle(fHeight, fWidth, font);

    intPair text_pos = align(pivot_x, pivot_y, wid, hei, canvas.TextWidth(str), canvas.TextHeight(str), align_mode);
    canvas.OutTextXY(text_pos.x, text_pos.y, str, true, c);
}

/**
 * @deprecated �˺����Ѿ���ʱ��������δ���汾�з�����
 * @brief �ھ��������ڶ����������
 * @param[in] canvas		    Canvas���ʵ����
 * @param[in] str			    ���Ƶ��ַ���
 * @param[in] fHeight		    �ַ����
 * @param[in] fWidth			�ַ���ȣ����ֵΪ0�����ַ�������Ӧ��ࣩ
 * @param[in] font  			����
 * @param[in] pivot_x			ê��x���꣨���������Ͻ�x���꣩
 * @param[in] pivot_y			ê��y���꣨���������Ͻ�y���꣩
 * @param[in] wid				������
 * @param[in] hei   			����߶�
 * @param[in] align_mode   		���뷽ʽ��Ĭ�Ͼ��ж��룩
 * @param[in] c	                ������ɫ
 * @param[in] esc       		�ַ�������д�Ƕȣ���λ0.1�㣩
 * @param[in] ori         		ÿ���ַ�����д�Ƕȣ���λ0.1�㣩
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
    // ê��Ӧ��������������Ͻ�

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
 * @deprecated �˺����Ѿ���ʱ���� Ver 0.1Alpha2 ���Ժ�汾������
 * @brief �ھ����ж����������
 * @param[in] x				    �������Ͻ�x����
 * @param[in] y				    �������Ͻ�y����
 * @param[in] width				���ο��
 * @param[in] height			���θ߶�
 * @param[in] horizontal	    �Ƿ�ˮƽ���У�Ĭ���ǣ�
 * @param[in] vertical  		�Ƿ�ֱ���У�Ĭ���ǣ�
*/
void __textAlign(LPCTSTR str, int x, int y, int width, int height, bool horizontal = true, bool vertical = true, int mode = TRANSPARENT)
{
    /* ʹ��ԭ��EasyX���ƣ����ڼ��������⣬����ʹ�ã�*/

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

/*���������û��˺�������ļ�*/
void createFile()
{
	FILE* fp;
	if ((fopen( "users.txt", "rb")) == NULL)                 // ������ļ�������
	{
		if ((fopen("users.txt", "wb+")) == NULL)
		{
			printf("�޷������ļ���\n");
			exit(0);
		}
	}
}

int index()  
{
	printf("___________________________________\n");
	printf("Welcome!\n");
	printf("�ڽ�һ����ʹ��ǰ������Ҫ���е�¼��\n");
	printf("��1��ע��\n");
	printf("��2����¼\n");
	printf("___________________________________\n");
	printf("�������Ӧ��ţ�");
	
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
		printf("��Ч�����룬���������룡");
	}

	return 0;
}

/*ע��*/
void reg()  // ���ṹ��Ĵ�С��һ��д����߶���1���ṹ���С
{
	user a, b;
	FILE* fp = NULL;
	errno_t err = 0;
	char temp[20];
	int count = 0;
	fp = fopen("users.txt", "r");
	fread(&b, sizeof(user), 1, fp);   // ��b����users

	printf("___________________________________\n");
	printf("��ӭ����ע����棡\n");
	Sleep(1000);
	printf("��info���û������ɳ���16���ַ�\n");
	printf("��info������q�ɷ���\n");
	printf("�������û���:\n");
	scanf("%s", &a.id);
	if (*a.id == 'q')
		return;
	while (1)
	{
		if (strcmp(a.id, b.id)) // ���ab��ͬ
		{
			if (!feof(fp))    // ���δ���ļ�β
			{
				fread(&b, sizeof(user), 1, fp);
			}
			else
				break;
		}
		else

		{
			printf("���û����Ѵ��ڣ�������ע�ᣡ\n");
			Sleep(1000);
			fclose(fp);
			return;
		}
	}



	printf("�������Ա�(1��Ů��/2���У�/3�������ˣ�)��\n");
	scanf("%d", &a.sex);
	while (a.sex != 1 && a.sex != 2 && a.sex != 3)
	{
		printf("����ǻ����˵Ļ�������3Ŷ-��-\n");
		scanf("%d", &a.sex);
	}
	getchar();
	while (1)
	{
		printf("����������\n");
		scanf(" %s", &a.pwd);
		printf("��ȷ������\n");
		scanf(" %s", &temp);

		if (!strcmp(a.pwd, temp))
		{
			fp = fopen("users.txt", "a");
			fwrite(&a, sizeof(user), 1, fp);
			printf("�˺�ע��ɹ������¼��\n");
			Sleep(500);
			fclose(fp);
			return;
		}
		else
		{
			printf("�������벻ƥ�䣡���������룡\n");
		}
	}
}

/*��¼ϵͳ*/
void  login()
{
	user a, b;
	FILE* fp;
	fp = fopen("users.txt", "r");
	fread(&b, sizeof(user), 1, fp);
	printf("___________________________________\n");
	printf("��ӭ������¼���棡\n");
	Sleep(500);
	printf("��info������q�ɷ���\n");
	printf("�������˺�\n");

	scanf("%s", &a.id);
	if (*a.id == 'q')
		return;
	while (1)
	{
		if (strcmp(a.id, b.id) == 0)         // ����д��û���
		{
			break;
		}
		else
		{
			if (!feof(fp))  // ����ļ�û�ж���                            

			{
				fread(&b, sizeof(user), 1, fp);
			}

			else
			{
				printf("���û��������ڣ����������룡\n");
				Sleep(500);
				fclose(fp);
				return;
			}
		}
	}
	printf("����������\n");
	scanf("%s", &a.pwd);
	while (1)
	{       

		if (strcmp(a.pwd, b.pwd) == 0)
		{
			fclose(fp);
			printf("��¼�ɹ�,��ӭʹ��!");
			//token = &a.id;  /*bug*/
			Sleep(500);
			return;
		}
		else
		{
			printf("���벻��ȷ!��������������\n");
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
        /*����GUI���*/


        struct _finddata_t file;
        intptr_t hFile;
        char buf[MAX_LEN];

        hFile = _findfirst("data/*.txt", &file);
        if (hFile == -1)
            printf("û���ҵ��ļ���\n");
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
        edit.SetFont(26, 0, L"΢���ź�");
        */


        // ����1
        canvas_main.Clear(true, WHITE);  //���ñ�����ɫ

        // ����
        canvas_main.SetLineStyle(PS_SOLID, 2);
        canvas_main.SetLineColor(0xEEEEEE);
        canvas_main.Line(WINDOW_WID * MID_LEFT, WINDOW_WID * EX_LEFT, WINDOW_WID * MID_LEFT, WINDOW_HEI); // �ָ���Դ�������ͻ�ͼ��

        // ����2
        canvas_main.SetFillColor(0x662015);
        canvas_main.FillRectangle(0, 0, WINDOW_WID * EX_LEFT, WINDOW_HEI); // �������ɫ����
        canvas_main.SetFillColor(0xEFE1CF);
        canvas_main.FillRectangle(0, 0, WINDOW_WID, WINDOW_WID * EX_LEFT); // ����ǳ��ɫ����

        //logo
        canvas_main.Load_Image_Alpha(L"sprites/logo_light.png", 0, 0, false, WINDOW_WID * EX_LEFT - 1, WINDOW_WID * EX_LEFT - 1, 255U, true);

        // ����
        textAlign(canvas_main, L"��ҵ���ݷ������ļ���Ϣ����ϵͳ", 36, 0, L"΢���ź�", WINDOW_WID * EX_LEFT, 0, WINDOW_WID * (1 - EX_LEFT), WINDOW_WID * EX_LEFT); // ����
        textAlign(canvas_main, L"Created by Friman " _SYS_VER_STR_, 16, 0, L"Arial", 0, 0, WINDOW_WID, WINDOW_HEI, BOTTOM_RIGHT, GRAY); // ��Ϣ

        // ��ť����

        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
        REDRAW_WINDOW();
        //��ť�ؼ��߼�

        ExMessage m_msg;
        while (1)
        {   
            
            m_msg = getmessage(EX_MOUSE);
            
            
            if (m_msg.x > WINDOW_WID * (1 - EX_LEFT * 3 / 4) && m_msg.y > WINDOW_WID * EX_LEFT / 4 && m_msg.x < WINDOW_WID * (1 - EX_LEFT / 4) && m_msg.y < WINDOW_WID * EX_LEFT * 3 / 4) // �����������������
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
                        textAlign(canvas_main, _T("TEST"), 36, 0, _T("΢���ź�"), 0, 0, WINDOW_WID, WINDOW_WID); //do somthing
                        canvas_main.SetFillColor(BLUE);
                        canvas_main.FillRoundRect(WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * (1 - EX_LEFT / 4), WINDOW_WID * EX_LEFT * 3 / 4, 20, 20);
                        imageAlign_alpha(canvas_main, L"sprites/login.png", 32, 32, WINDOW_WID * (1 - EX_LEFT * 3 / 4), WINDOW_WID * EX_LEFT / 4, WINDOW_WID * EX_LEFT / 2, WINDOW_WID * EX_LEFT / 2);
                        REDRAW_WINDOW();
                        break;
                    }
                    default:
                    {
                        // ���ָ�뾭����ť
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
                // Ĭ��״̬
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
		printf("��1��Module A\n");
		printf("��2��Module B\n");
		printf("��3���˳���¼\n");
	}
	else
	{
		printf("����δ��¼�����ȵ�¼��");
		printf("��1����¼");
		printf("��2��ע��");
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