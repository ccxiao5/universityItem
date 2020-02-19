package org.crazyit.ui;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.SimpleAdapter;
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
public class SimpleAdapterTest extends Activity
{
	private String[] names = new String[]
		{ "虎头", "弄玉", "李清照", "李白"};
	private String[] descs = new String[]
		{ "可爱的小孩", "一个擅长音乐的女孩"
			, "一个擅长的文学的女性", "浪漫主义诗人"};
	private String[] loc = new String[]
			{ "北京海淀区", "上海徐家汇"
				, "沈阳大东区", "天津河北区"};	
	private int[] imageIds = new int[]
		{ R.drawable.tiger , R.drawable.nongyu
		, R.drawable.qingzhao , R.drawable.libai};
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// 创建一个List集合，List集合的元素是Map
		List<Map<String, Object>> listItems = 
				new ArrayList<Map<String, Object>>();
		for (int i = 0; i < names.length; i++)
		{
			Map<String, Object> listItem = new HashMap<String, Object>();
			listItem.put("header", imageIds[i]);
			listItem.put("personName", names[i]);
			listItem.put("desc", descs[i]);
			listItem.put("loc", loc[i]);
			listItems.add(listItem);
		}
		// 创建一个SimpleAdapter
		SimpleAdapter simpleAdapter = new SimpleAdapter(this, listItems,
			R.layout.simple_item, 
			new String[] { "personName", "header" , "desc","loc"},
			new int[] { R.id.name, R.id.header , R.id.desc ,R.id.loc});
		ListView list = (ListView) findViewById(R.id.mylist);
		// 为ListView设置Adapter
		list.setAdapter(simpleAdapter);
		// 为ListView的列表项单击事件绑定事件监听器
		list.setOnItemClickListener(new OnItemClickListener()
		{
			ImageView img=(ImageView) findViewById(R.id.img);
			// 第position项被单击时激发该方法。
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id)
			{
				img.setImageResource(imageIds[position]);
			
			}
		});
	}
}