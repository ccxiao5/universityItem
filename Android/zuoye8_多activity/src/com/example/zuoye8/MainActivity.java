package com.example.zuoye8;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

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
public class MainActivity extends Activity
{
	final String TAG = "--CrazyIt--";
	Button finish ,startActivity;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// �����־
		Log.d(TAG, "�̻���151001215-------onCreate------");
		finish = (Button) findViewById(R.id.finish);
		startActivity = (Button) findViewById(R.id.startActivity);
		// ΪstartActivity��ť���¼�������
		startActivity.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View source)
			{
				Intent intent = new Intent(MainActivity.this
						, SecondActivity.class);
				startActivity(intent);
			}
		});	
		// Ϊfinish��ť���¼�������
		finish.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View source)
			{
				// ������Activity
				MainActivity.this.finish();
			}
		});		
	}

	@Override
	public void onStart()
	{
		super.onStart();
		// �����־
		Log.d(TAG, "�̻���151001215-------onStart------");
	}

	@Override
	public void onRestart()
	{
		super.onRestart();
		// �����־
		Log.d(TAG, "�̻���151001215-------onRestart------");
	}

	@Override
	public void onResume()
	{
		super.onResume();
		// �����־
		Log.d(TAG, "�̻���151001215-------onResume------");
	}

	@Override
	public void onPause()
	{
		super.onPause();
		// �����־
		Log.d(TAG, "�̻���151001215-------onPause------");
	}

	@Override
	public void onStop()
	{
		super.onStop();
		// �����־
		Log.d(TAG, "�̻���151001215-------onStop------");
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();
		// �����־
		Log.d(TAG, "�̻���151001215-------onDestroy------");
	}
}