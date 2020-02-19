package org.crazyit.ui;

import android.app.TabActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TabHost;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.TabHost.TabSpec;
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
public class TabHostTest extends TabActivity
{
	String[] arr1 = { "程虎威,151001215", "猪八戒", "牛魔王" };
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		ListView list1 = (ListView) findViewById(R.id.list1);
		// 定义一个数组
		ArrayAdapter<String> adapter1 = new ArrayAdapter<String>
		(this, R.layout.array_item, arr1);
		// 为ListView设置Adapter
		list1.setAdapter(adapter1);
		// 获取该Activity里面的TabHost组件
		
		TabHost tabHost = getTabHost();
		// 创建第一个Tab页
		TabSpec tab1 = tabHost.newTabSpec("tab1")
			.setIndicator("已接电话") // 设置标题
			.setContent(R.id.tab01); //设置内容
		// 添加第一个标签页
		tabHost.addTab(tab1);
		
		TabSpec tab2 = tabHost.newTabSpec("tab2")
			// 在标签标题上放置图标
			.setIndicator("呼出电话", getResources()
			.getDrawable(R.drawable.ic_launcher))
			.setContent(R.id.tab02);
		// 添加第二个标签页
		tabHost.addTab(tab2);
		
		TabSpec tab3 = tabHost.newTabSpec("tab3")
			.setIndicator("未接电话")
			.setContent(R.id.tab03);
		// 添加第三个标签页
		tabHost.addTab(tab3);
		
		TabSpec tab4 = tabHost.newTabSpec("tab4")
				.setIndicator("短消息")
				.setContent(R.id.tab04);
			// 添加第四个标签页
			tabHost.addTab(tab4);
		ListView list = (ListView) findViewById(R.id.list1);
		// 为ListView设置Adapter
		list.setAdapter(adapter1);
		// 为ListView的列表项单击事件绑定事件监听器
		list.setOnItemClickListener(new OnItemClickListener()
		{
				// 第position项被单击时激发该方法。
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
						int position, long id)
			{
				Toast.makeText(TabHostTest.this, arr1[position]+"被单击了", Toast.LENGTH_LONG).show();
			}
		});
	}
}