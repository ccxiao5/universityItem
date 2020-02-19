package com.example.test2;

import android.app.Activity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ImageView;
import android.widget.LinearLayout;
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
public class MainActivity extends Activity
{
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		//����һ��BaseExpandableListAdapter����
		ExpandableListAdapter adapter = new BaseExpandableListAdapter()
		{
			int[] logos = new int[]
			{
				R.drawable.logo,
				R.drawable.logo,
				R.drawable.logo
			};
 			private String[] armTypes = new String[]
				{ "1.1 Android�ķ�չ����ʷ", "1.2 �Android��������", "1.3 Android���ÿ������ߵ��÷�"};
			private String[][] arms = new String[][]
			{
				{ "1.1.1 Android�ķ�չ�ͼ��", "1.1.2 Androidƽ̨�ܹ�������" },
				{ "1.2.1 ���غͰ�װAndroid SDK", "1.2.2 ��װ���С����Ի���", "1.2.3 ��װEclipse��ADT���"},
				{ "1.3.1 �������д�����ɾ�������AVD", "1.3.2 ʹ��Androidģ����" , "1.3.3 ʹ��DDMS���е���" }
			};
			// ��ȡָ����λ�á�ָ�����б�������б�������
			@Override
			public Object getChild(int groupPosition, int childPosition)
			{
				return arms[groupPosition][childPosition];
			}

			@Override
			public long getChildId(int groupPosition, int childPosition)
			{
				return childPosition;
			}

			@Override
			public int getChildrenCount(int groupPosition)
			{
				return arms[groupPosition].length;
			}

			private TextView getTextView()
			{
				AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT, 64);
				TextView textView = new TextView(MainActivity.this);
				textView.setLayoutParams(lp);
				textView.setGravity(Gravity.CENTER_VERTICAL | Gravity.LEFT);
				textView.setPadding(36, 0, 0, 0);
				textView.setTextSize(15);
				return textView;
			}

			// �÷�������ÿ����ѡ������
			@Override
			public View getChildView(int groupPosition, int childPosition,
					boolean isLastChild, View convertView, ViewGroup parent)
			{
				TextView textView = getTextView();
				textView.setText(getChild(groupPosition, childPosition)
						.toString());
				return textView;
			}

			// ��ȡָ����λ�ô���������
			@Override
			public Object getGroup(int groupPosition)
			{
				return armTypes[groupPosition];
			}

			@Override
			public int getGroupCount()
			{
				return armTypes.length;
			}

			@Override
			public long getGroupId(int groupPosition)
			{
				return groupPosition;
			}

			// �÷�������ÿ����ѡ������
			@Override
			public View getGroupView(int groupPosition, boolean isExpanded,
					View convertView, ViewGroup parent)
			{
				LinearLayout ll = new LinearLayout(MainActivity.this);
				ll.setOrientation(0);
				ImageView logo = new ImageView(MainActivity.this);
				logo.setImageResource(logos[groupPosition]);
				ll.addView(logo);
				TextView textView = getTextView();
				textView.setText(getGroup(groupPosition).toString());
				ll.addView(textView);
				return ll;
			}

			@Override
			public boolean isChildSelectable(int groupPosition,
					int childPosition)
			{
				return true;
			}

			@Override
			public boolean hasStableIds()
			{
				return true;
			}
		};
		ExpandableListView expandListView = (ExpandableListView) findViewById(R.id.list);
		expandListView.setAdapter(adapter);
	}
}