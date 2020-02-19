package com.example.zuoye5;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.R.id;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.Button;
import android.widget.ViewSwitcher.ViewFactory;
import android.widget.GridView;
import android.widget.ImageSwitcher;
import android.widget.ImageView;
import android.widget.SimpleAdapter;

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
	int[] imageIds = new int[]
	{
		R.drawable.bomb5 , R.drawable.bomb6 , R.drawable.bomb7 
		, R.drawable.bomb8 , R.drawable.bomb9 , R.drawable.bomb10
		, R.drawable.bomb11 , R.drawable.bomb12	, R.drawable.bomb13
		, R.drawable.bomb14 , R.drawable.bomb15 , R.drawable.bomb16
	};
	int current;
	ImageSwitcher switcher;
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// ����һ��List����List�����Ԫ����Map
		List<Map<String, Object>> listItems = 
				new ArrayList<Map<String, Object>>();
		for (int i = 0; i < imageIds.length; i++)
		{
			Map<String, Object> listItem = new HashMap<String, Object>();
			listItem.put("image", imageIds[i]);
			listItems.add(listItem);
		}
		// ��ȡ��ʾͼƬ��ImageSwitcher
		switcher = (ImageSwitcher)
				findViewById(R.id.switcher);
		// ΪImageSwitcher����ͼƬ�л��Ķ���Ч��
		switcher.setFactory(new ViewFactory()
		{
			@Override
			public View makeView()
			{
				// ����ImageView����
				ImageView imageView = new ImageView(MainActivity.this);
				imageView.setScaleType(ImageView.ScaleType.FIT_CENTER);
				imageView.setLayoutParams(new ImageSwitcher.LayoutParams(
					LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
				// ����ImageView����
				return imageView;
			}
		});
		// ����һ��SimpleAdapter
		SimpleAdapter simpleAdapter = new SimpleAdapter(this,
			listItems
			// ʹ��/layout/cell.xml�ļ���Ϊ���沼��
			, R.layout.cell, new String[]{"image"},
			new int[] { R.id.image1 });
		GridView grid = (GridView) findViewById(R.id.grid01);
		// ΪGridView����Adapter
		grid.setAdapter(simpleAdapter);
		// ����б��ѡ�еļ�����
		grid.setOnItemSelectedListener(new OnItemSelectedListener()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent, View view,
					int position, long id)
			{
				// ��ʾ��ǰ��ѡ�е�ͼƬ
				switcher.setImageResource(imageIds[position]);
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent)
			{
			}
		});
		// ����б�������ļ�����
		grid.setOnItemClickListener(new OnItemClickListener()
		{
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
				int position, long id)
			{
				// ��ʾ��������ͼƬ��ͼƬ
				switcher.setImageResource(imageIds[position]);
				final Button bt1=(Button) findViewById(R.id.button1);
				final Button bt2=(Button) findViewById(R.id.button2);
				current=position;
				bt1.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						current--;
						switcher.setImageResource(imageIds[(current+imageIds.length)%imageIds.length]);
					}
				});
				bt2.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub
						current++;
						switcher.setImageResource(imageIds[current%imageIds.length]);
						
						
					}
				});

			}
			
		});
		
		
	}
}