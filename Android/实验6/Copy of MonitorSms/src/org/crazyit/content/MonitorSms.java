package org.crazyit.content;

import java.io.FileNotFoundException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.util.Date;

import android.app.Activity;
import android.database.ContentObserver;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;

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
public class MonitorSms extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// Ϊcontent://sms�����ݸı�ע�������
		getContentResolver().registerContentObserver(
			Uri.parse("content://sms"), true,
			new SmsObserver(new Handler()));
	}

	// �ṩ�Զ����ContentObserver��������
	private final class SmsObserver extends ContentObserver
	{
		public SmsObserver(Handler handler)
		{
			super(handler);
		}

		public void onChange(boolean selfChange)
		{
			// ��ѯ�������еĶ���(�������ڷ���״̬�Ķ��ŷ��ڷ�����)
			Cursor cursor = getContentResolver().query(
				Uri.parse("content://sms/inbox")
				, null, null, null, null);
			OutputStream os = null;
			while (cursor.moveToNext())
			{
				StringBuilder sb = new StringBuilder();
				// ��ȡ���ŵķ��͵�ַ
				sb.append("address=").append(cursor
					.getString(cursor.getColumnIndex("address")));
				// ��ȡ���ŵı���
				sb.append(";subject=").append(cursor
					.getString(cursor.getColumnIndex("subject")));
				// ��ȡ���ŵ�����
				sb.append(";body=").append(cursor
					.getString(cursor.getColumnIndex("body")));
				// ��ȡ���ŵķ���ʱ��
				sb.append(";time=").append(cursor
					.getLong(cursor.getColumnIndex("date")));
				try
				{
					os = openFileOutput("smsList", MODE_APPEND);
				}
				catch (FileNotFoundException e)
				{
					e.printStackTrace();
				}
				PrintStream ps = new PrintStream(os);
				ps.println(new Date() + " ��������:"+sb.toString()+"\n");	
				ps.close();
			}
			
		}
	}
}