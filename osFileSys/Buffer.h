#pragma once
class Buffer
{
public:
	enum BufferFlag
	{
		B_WRITE = 0x1,		/* д�������������е���Ϣд��Ӳ����ȥ */
		B_READ = 0x2,		/* �����������̶�ȡ��Ϣ�������� */
		B_DONE = 0x4,		/* I/O�������� */
		B_ERROR = 0x8,		/* I/O��������ֹ */
		B_BUSY = 0x10,		/* ��Ӧ��������ʹ���� */
		B_WANTED = 0x20,	/* �н������ڵȴ�ʹ�ø�buf�������Դ����B_BUSY��־ʱ��Ҫ�������ֽ��� */
		B_ASYNC = 0x40,		/* �첽I/O������Ҫ�ȴ������ */
		B_DELWRI = 0x80		/* �ӳ�д������Ӧ����Ҫ��������ʱ���ٽ�������д����Ӧ���豸�� */
	};

public:
	unsigned int flags;	//������ƿ��־λ

	int padding;
	Buffer* b_forw;
	Buffer* b_back;
	Buffer* av_forw;
	Buffer* av_back;

	short b_dev;	//�����豸��
	int b_wcount;	//�贫���ֽ���
	unsigned char* b_addr;
	int b_nblk;		//�����߼����
	int b_error;	//I/O������Ϣ
	int b_resid;	//I/O������δ���͵��ֽ���
};