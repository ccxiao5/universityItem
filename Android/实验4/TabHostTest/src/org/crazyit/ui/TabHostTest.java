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
	String[] arr1 = { "�̻���,151001215", "��˽�", "ţħ��" };
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		ListView list1 = (ListView) findViewById(R.id.list1);
		// ����һ������
		ArrayAdapter<String> adapter1 = new ArrayAdapter<String>
		(this, R.layout.array_item, arr1);
		// ΪListView����Adapter
		list1.setAdapter(adapter1);
		// ��ȡ��Activity�����TabHost���
		
		TabHost tabHost = getTabHost();
		// ������һ��Tabҳ
		TabSpec tab1 = tabHost.newTabSpec("tab1")
			.setIndicator("�ѽӵ绰") // ���ñ���
			.setContent(R.id.tab01); //��������
		// ��ӵ�һ����ǩҳ
		tabHost.addTab(tab1);
		
		TabSpec tab2 = tabHost.newTabSpec("tab2")
			// �ڱ�ǩ�����Ϸ���ͼ��
			.setIndicator("�����绰", getResources()
			.getDrawable(R.drawable.ic_launcher))
			.setContent(R.id.tab02);
		// ��ӵڶ�����ǩҳ
		tabHost.addTab(tab2);
		
		TabSpec tab3 = tabHost.newTabSpec("tab3")
			.setIndicator("δ�ӵ绰")
			.setContent(R.id.tab03);
		// ��ӵ�������ǩҳ
		tabHost.addTab(tab3);
		
		TabSpec tab4 = tabHost.newTabSpec("tab4")
				.setIndicator("����Ϣ")
				.setContent(R.id.tab04);
			// ��ӵ��ĸ���ǩҳ
			tabHost.addTab(tab4);
		ListView list = (ListView) findViewById(R.id.list1);
		// ΪListView����Adapter
		list.setAdapter(adapter1);
		// ΪListView���б�����¼����¼�������
		list.setOnItemClickListener(new OnItemClickListener()
		{
				// ��position�����ʱ�����÷�����
			@Override
			public void onItemClick(AdapterView<?> parent, View view,
						int position, long id)
			{
				Toast.makeText(TabHostTest.this, arr1[position]+"��������", Toast.LENGTH_LONG).show();
			}
		});
	}
}