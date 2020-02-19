package org.crazyit.ui;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

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
public class MenuResTest extends Activity
{
	private TextView txt;
	private TextView txt1;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		txt = (TextView) findViewById(R.id.txt);
		txt1= (TextView) findViewById(R.id.inf);
		// Ϊ�ı���ע�������Ĳ˵�
		registerForContextMenu(txt);
		registerForContextMenu(txt1);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu)
	{
		MenuInflater inflator = new MenuInflater(this);
		// ״̬R.menu.context��Ӧ�Ĳ˵�������ӵ�menu��
		inflator.inflate(R.menu.my_menu, menu);
		return super.onCreateOptionsMenu(menu);
	}
	// ���������Ĳ˵�ʱ�����÷���
	@Override
	public void onCreateContextMenu(ContextMenu menu, View source,
			ContextMenu.ContextMenuInfo menuInfo)
	{
		MenuInflater inflator = new MenuInflater(this);
		// ״̬R.menu.context��Ӧ�Ĳ˵�������ӵ�menu��
		if(source==txt)
		{ 
			inflator.inflate(R.menu.context, menu);
			menu.setHeaderIcon(R.drawable.tools);
			menu.setHeaderTitle("��ѡ�񱳾�ɫ");
		}
		else if(source==txt1)
		{
			inflator.inflate(R.menu.context1, menu);
			menu.setHeaderIcon(R.drawable.tools);
			menu.setHeaderTitle("��ѡ�����");
		}
	}
	
	// �����Ĳ˵��в˵������ʱ�����÷�����
	@Override
	public boolean onContextItemSelected(MenuItem mi)
	{
		mi.setChecked(true);  //��
		switch (mi.getItemId())
		{
			case R.id.red:
				mi.setChecked(true);
				txt.setBackgroundColor(Color.RED);
				break;
			case R.id.green:
				mi.setChecked(true);
				txt.setBackgroundColor(Color.GREEN);
				break;
			case R.id.blue:
				mi.setChecked(true);
				txt.setBackgroundColor(Color.BLUE);
				break;
			case R.id.bar10:
				mi.setChecked(true);
				ProgressBar bar = (ProgressBar) findViewById(R.id.bar);
				bar.setProgress(10);
				break;
			case R.id.bar90:
				mi.setChecked(true);
				ProgressBar bar1 = (ProgressBar) findViewById(R.id.bar);
				bar1.setProgress(90);
		}
		return true;
	}
	
	

	@Override
	// ѡ��˵��Ĳ˵��������Ļص�����
	public boolean onOptionsItemSelected(MenuItem mi)
	{
		if(mi.isCheckable())
		{
			mi.setChecked(true);  //��
		}
		// �жϵ��������ĸ��˵��������Ե�������Ӧ��
		switch (mi.getItemId())
		{
			case R.id.font_10:
				txt.setTextSize(10 * 2);
				break;
			case R.id.font_12:
				txt.setTextSize(12 * 2);
				break;
			case R.id.font_14:
				txt.setTextSize(14 * 2);
				break;
			case R.id.font_16:
				txt.setTextSize(16 * 2);
				break;
			case R.id.font_18:
				txt.setTextSize(18 * 2);
				break;
			case R.id.red_font:
				txt.setTextColor(Color.RED);
				mi.setChecked(true);
				break;
			case R.id.green_font:
				txt.setTextColor(Color.GREEN);
				mi.setChecked(true);
				break;
			case R.id.blue_font:
				txt.setTextColor(Color.BLUE);
				mi.setChecked(true);
				break;
			case R.id.plain_item:
				Toast toast = Toast.makeText(MenuResTest.this, "����������ͨ�˵���",
						Toast.LENGTH_SHORT);
				toast.show();
				break;
			case R.id.Progress_50:
				ProgressBar bar = (ProgressBar) findViewById(R.id.bar);
				bar.setProgress(50);
		}
		return true;
	}
}