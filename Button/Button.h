#pragma once
#include"common.h"

class Button
{
private:
    int x; // ��ť���Ͻ�x����
    int y; // ��ť���Ͻ�y����
    int width; // ��ť���
    int height; // ��ť�߶�
    float scale; // ���ű���������ʵ�������ͣЧ��
    bool isMouseOver; // ��ʾ����Ƿ��ڰ�ť�Ϸ�
    wstring text; // ��ť�ı�
    function<void()> onClick; // �����ť�����ĺ���

public:

    Button(int x, int y, int width, int height, const wstring& text, const function<void()>& onClick)
        : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), isMouseOver(false)
    {
    }

    // �������Ƿ��ڰ�ť�Ϸ�
    void checkMouseOver(int mouseX, int mouseY)
    {
        isMouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

        if (isMouseOver) {
            scale = 0.9f; // �����ͣʱ���Ű�ť
        }
        else {
            scale = 1.0f; // �ָ���ťԭʼ��С
        }
    }

    // ���������Ƿ��ڰ�ť�ڣ���ִ�к���
    bool checkClick(int mouseX, int mouseY)
    {
        if (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height)
        {
            onClick(); // ִ�а�ť���ʱ�ĺ���
            isMouseOver = false;
            scale = 1.0f;
            return true;
        }
        return false;
    }

    // ���ư�ť
    void draw()
    {
        int scaledWidth = width * scale; // ���ź�İ�ť���
        int scaledHeight = height * scale; // ���ź�İ�ť�߶�
        int scaledX = x + (width - scaledWidth) / 2; // ���ź�İ�ťx����
        int scaledY = y + (height - scaledHeight) / 2; // ���ź�İ�ťy����

        if (isMouseOver)
        {
            setlinecolor(RGB(0, 0, 0)); // �����ͣʱ��ť�߿���ɫ
            setfillcolor(RGB(254, 229, 161));// �����ͣʱ��ť�����ɫ

        }
        else
        {
            setlinecolor(RGB(0, 0, 0)); // ��ť�߿���ɫ
            setfillcolor(RGB(235, 196, 9)); // ��ť�����ɫ
        }

        fillroundrect(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight, 10, 10); // ���ư�ť
        settextcolor(BLACK); // �����ı���ɫΪ��ɫ
        setbkmode(TRANSPARENT); // �����ı�����͸��
        settextstyle(40 * scale, 0, _T("΢���ź�")); // �����ı���С������
        //������ʾ��ť�ı�
        int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // �����ı��ڰ�ť�����x����
        int textY = scaledY + (scaledHeight - textheight(_T("΢���ź�"))) / 2; // �����ı��ڰ�ť�����y����
        outtextxy(textX, textY, text.c_str()); // �ڰ�ť�ϻ����ı�
    }
};

// ����Widget�࣬��ʾһ���򵥵�ͼ���û�����
class Widget
{
private:
    int width; // ���
    int height; // �߶�
    int currentIndex; // ��ǰҳ������
    vector<IMAGE*> pages; // �洢����ҳ���ͼƬָ��
    vector<vector<Button*>> buttons; // �洢ÿ��ҳ���ϵİ�ť

    // ���һ��ҳ��
    void addPage(IMAGE* page)
    {
        pages.push_back(page);
        buttons.push_back({});
    }

    // ��ָ��ҳ�������һ����ť
    void addButton(int index, Button* button)
    {
        if (index >= 0 && index < buttons.size())
        {
            buttons[index].push_back(button);
        }
    }

    // ���õ�ǰ��ʾ��ҳ������
    void setCurrentIndex(int index)
    {
        if (index >= 0 && index < pages.size())
        {
            currentIndex = index;
        }
    }

    // ����������¼�
    void mouseClick(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                if (button->checkClick(mouseX, mouseY))
                {
                    break;
                }
            }
        }
    }

    // ��������ƶ��¼�
    void mouseMove(int mouseX, int mouseY)
    {
        if (currentIndex >= 0 && currentIndex < buttons.size())
        {
            for (Button* button : buttons[currentIndex])
            {
                button->checkMouseOver(mouseX, mouseY);
            }
        }
    }

    // ���Ƶ�ǰҳ�������
    void draw()
    {
        if (currentIndex >= 0 && currentIndex < pages.size())
        {
            putimage(0, 0, pages[currentIndex]); // �ڴ����л��Ƶ�ǰҳ���ͼƬ

            if (currentIndex >= 0 && currentIndex < buttons.size())
            {
                for (Button* button : buttons[currentIndex])
                {
                    button->draw(); // ���Ƶ�ǰҳ���ϵ����а�ť
                }
            }
        }
    }

public:
    Widget(int width, int height)
        :width(width), height(height), currentIndex(-1)
    {
    }
    ~Widget() {}

    // ��ʼ���ؼ�������ͼ�λ���������ҳ��Ͱ�ť
    void init()
    {
        initgraph(width, height);

        // ����ҳ��1
        IMAGE menu;
        loadimage(&menu, _T("menu.png"), width, height);

        IMAGE* page1 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
        putimage(0, 0, &menu);
        getimage(page1, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��

        addPage(page1); // ���ҳ��1

        // ��ҳ��1������ť1
        Button* button1_1 = new Button(363, 400, 200, 50, L"��ʼ��Ϸ", [&]() {
            setCurrentIndex(1); // �����ť1ʱ���л���ҳ��2
            });
        addButton(0, button1_1); // ����ť1��ӵ�ҳ��1

        // ��ҳ��1������ť2
        Button* button1_2 = new Button(363, 500, 200, 50, L"�ؿ�ѡ��", [&]() {
            setCurrentIndex(2);
            });
        addButton(0, button1_2); // ����ť2��ӵ�ҳ��1

        // ��ҳ��1������ť3
        Button* button1_3 = new Button(363, 600, 200, 50, L"��  ��", [&]() {
            // �����ť3ʱ��ִ����ص��߼�
            close();
            });
        addButton(0, button1_3); // ����ť3��ӵ�ҳ��1

        // ����ҳ��2
        IMAGE* page2 = new IMAGE(width, height);
        setfillcolor(RED); // ����ҳ��2�ı�����ɫΪ��ɫ
        solidrectangle(0, 0, width, height); // ����ҳ��2�ı�������
        getimage(page2, 0, 0, width, height); // ��ҳ��2�����ݱ��浽ͼƬ��

        addPage(page2); // ���ҳ��2

        // ��ҳ��2������ť1
        Button* button2_1 = new Button(100, 200, 200, 50, L"���ص�Page 1", [&]() {
            setCurrentIndex(0); // �����ť1ʱ���л���ҳ��1
            });
        addButton(1, button2_1); // ����ť1��ӵ�ҳ��2

        IMAGE* page3 = new IMAGE(width, height);
        putimage(0, 0, &menu);
        getimage(page3, 0, 0, width, height); // ��ҳ��2�����ݱ��浽ͼƬ��

        addPage(page3);

        Button* button3_1 = new Button(50, 50, 200, 50, L"������ҳ", [&]() {
            setCurrentIndex(0); // �����ť1ʱ���л���ҳ��1
            });
        addButton(2, button3_1); // ����ť1��ӵ�ҳ��2

        setCurrentIndex(0); // ���ó�ʼ��ʾҳ��Ϊҳ��1
    }

    // ���У�������Ϣѭ��
    void run()
    {
        ExMessage msg;

        BeginBatchDraw(); // ��ʼ��������

        while (true)
        {
            if (peekmessage(&msg)) // ����Ƿ�����Ϣ
            {
                int mouseX = msg.x; // ��ȡ���x����
                int mouseY = msg.y; // ��ȡ���y����

                switch (msg.message)
                {
                case WM_LBUTTONDOWN: // �����������¼�
                    mouseClick(mouseX, mouseY); // ����������¼�
                    break;
                case WM_MOUSEMOVE: // ����ƶ��¼�
                    mouseMove(mouseX, mouseY); // ��������ƶ��¼�
                    break;
                }
            }

            draw(); // ���Ƶ�ǰҳ������
            FlushBatchDraw(); // ��������������ʾ����Ļ��
            Sleep(10);
        }

        EndBatchDraw(); // ������������
    }

    // �ر�
    void close()
    {
        closegraph(); // �ر�ͼ�λ���
    }
};

