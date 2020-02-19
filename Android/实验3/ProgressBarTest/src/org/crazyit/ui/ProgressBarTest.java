package org.crazyit.ui;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.widget.ProgressBar;
import android.widget.TextView;

/**
 * Description:
 * <br/>site: <a href="http://www.crazyit.org">crazyit.org</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class ProgressBarTest extends Activity
{
	int hasData = 0;
	// ��¼ProgressBar����ɽ���
	int status = 0;
	String put="";
	TextView  tv;
	// ����һ��������µĽ��ȵ�Handler
	Handler mHandler = new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			// ������Ϣ���ɸó����͵�
			if (msg.what == 0x111)
			{
				put=put+","+status;
				tv.setText(put);
			}
		}
	};	
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		tv = (TextView) findViewById(R.id.tv);
		// �����߳���ִ������
		new Thread()
		{
			public void run()
			{
				while (status < 100)
				{
					mHandler.sendEmptyMessage(0x111);
					status = doWork();
				}
			}
		}.start();
	}
	// ģ��һ����ʱ�Ĳ���
	public int doWork()
	{
		// Ϊ����Ԫ�ظ�ֵ
		hasData++;
		try
		{
			Thread.sleep(1000);
		}
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}
		return hasData;
	}
}
