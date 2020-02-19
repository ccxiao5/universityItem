/**
 *
 */
package org.crazyit.app;

import android.app.ExpandableListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.BaseExpandableListAdapter;
import android.widget.ExpandableListAdapter;
import android.widget.ExpandableListView;
import android.widget.ExpandableListView.OnChildClickListener;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

/**
 * Description:
 * <br/>网站: <a href="http://www.crazyit.org">疯狂Java联盟</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class SelectMajorActivity extends ExpandableListActivity
{
	// 定义省份数组
	private String[] Major = new String[]
	{ "计算机", "物联网", "网工","树莓","信息"};
	private String[][] majors = new String[][]
	{
		{ "计算机15-1", "计算机15-2", "计算机15-3"},
		{ "物联网15-1", "物联网15-2", "物联网15-3"},
		{ "网工15-1"},
		{"树莓15-1","树莓15-2"},
		{"信息15-1","程虎威151001215"}
	};

	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		ExpandableListAdapter adapter = new BaseExpandableListAdapter()
		{
			// 获取指定组位置、指定子列表项处的子列表项数据
			@Override
			public Object getChild(int groupPosition, int childPosition)
			{
				return majors[groupPosition][childPosition];
			}

			@Override
			public long getChildId(int groupPosition, int childPosition)
			{
				return childPosition;
			}

			@Override
			public int getChildrenCount(int groupPosition)
			{
				return majors[groupPosition].length;
			}

			private TextView getTextView()
			{
				AbsListView.LayoutParams lp = new AbsListView.LayoutParams(
						ViewGroup.LayoutParams.MATCH_PARENT, 64);
				TextView textView = new TextView(SelectMajorActivity.this);
				textView.setLayoutParams(lp);
				textView.setGravity(Gravity.CENTER_VERTICAL | Gravity.LEFT);
				textView.setPadding(36, 0, 0, 0);
				textView.setTextSize(20);
				return textView;
			}

			// 该方法决定每个子选项的外观
			@Override
			public View getChildView(int groupPosition, int childPosition,
					boolean isLastChild, View convertView, ViewGroup parent)
			{
				TextView textView = getTextView();
				textView.setText(getChild(groupPosition, childPosition)
						.toString());
				return textView;
			}

			// 获取指定组位置处的组数据
			@Override
			public Object getGroup(int groupPosition)
			{
				return Major[groupPosition];
			}

			@Override
			public int getGroupCount()
			{
				return Major.length;
			}

			@Override
			public long getGroupId(int groupPosition)
			{
				return groupPosition;
			}

			// 该方法决定每个组选项的外观
			@Override
			public View getGroupView(int groupPosition, boolean isExpanded,
					View convertView, ViewGroup parent)
			{
				LinearLayout ll = new LinearLayout(SelectMajorActivity.this);
				ll.setOrientation(0);
				ImageView logo = new ImageView(SelectMajorActivity.this);
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
		// 设置该窗口显示列表
		setListAdapter(adapter);
		getExpandableListView().setOnChildClickListener(
			new OnChildClickListener()
			{
				@Override
				public boolean onChildClick(ExpandableListView parent,
						View source, int groupPosition, int childPosition,
						long id)
				{
					// 获取启动该Activity之前的Activity对应的Intent
					Intent intent = getIntent();
					intent.putExtra("majors",
							majors[groupPosition][childPosition]);
					// 设置该SelectActivity的结果码，并设置结束之后退回的Activity
					SelectMajorActivity.this.setResult(1, intent);//传递给ActivityForResult.java的onActivityResult的resultCode
					// 结束SelectCityActivity。**************************************************************************
					SelectMajorActivity.this.finish();
					return false;
				}
			});
	}
}
