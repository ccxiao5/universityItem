package org.crazyit.handler;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

/**
 * Description:
 * <br/>website: <a href="http://www.crazyit.org">crazyit.org</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author Yeeku.H.Lee kongyeeku@163.com
 * @version 1.0
 */
public class CalPrime extends Activity
{
	static final String UPPER_NUM = "upper";
	EditText etNum;
	CalThread calThread;
	CalThread cT;
	public Handler ml;
	// 定义一个线程类
	class CalThread extends Thread
	{
		public Handler mHandler;
		public void run()
		{
			Looper.prepare();
			mHandler = new Handler()
			{
				// 定义处理消息的方法
				@Override
				public void handleMessage(Message msg)
				{
					if(msg.what == 0x123)
					{
						int upper = msg.getData().getInt(UPPER_NUM);
						ArrayList<Integer> nums = new ArrayList<Integer>();
						// 计算从2开始、到upper的所有质数
						outer:
						for (int i = 2 ; i <= upper ; i++)
						{
							// 用i处于从2开始、到i的平方根的所有数
							for (int j = 2 ; j <= Math.sqrt(i) ; j++)
							{
								// 如果可以整除，表明这个数不是质数
								if(i != 2 && i % j == 0)
								{
									continue outer;
								}
							}
							nums.add(i);
						}
						Message msg1 = new Message();
						msg1.what = 0x123;
						Bundle bundle = new Bundle();
						bundle.putIntegerArrayList("chw", nums);
						msg1.setData(bundle);
						ml.sendMessage(msg1);
					}
				}
			};
			Looper.loop();
		}
	}
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		etNum = (EditText)findViewById(R.id.etNum);
		calThread = new CalThread();
		// 启动新线程
		calThread.start();
		ml= new Handler()
		{
			public void handleMessage(Message msg1)
			{
				if(msg1.what == 0x123)
				{
					ArrayList<Integer> a = msg1.getData().getIntegerArrayList("chw");
					TextView tv=(TextView) findViewById(R.id.tv);
					tv.setText(a.toString());
				}
			}
		};
		
	}
	// 为按钮的点击事件提供事件处理函数
	public void cal(View source)
	{
		// 创建消息
		Message msg = new Message();
		msg.what = 0x123;
		Bundle bundle = new Bundle();
		bundle.putInt(UPPER_NUM ,
			Integer.parseInt(etNum.getText().toString()));
		msg.setData(bundle);
		// 向新线程中的Handler发送消息
		calThread.mHandler.sendMessage(msg);
		
		
	}
}
