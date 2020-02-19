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
 * <br/>��վ: <a href="http://www.crazyit.org">���Java����</a>
 * <br/>Copyright (C), 2001-2014, Yeeku.H.Lee
 * <br/>This program is protected by copyright laws.
 * <br/>Program Name:
 * <br/>Date:
 * @author  Yeeku.H.Lee kongyeeku@163.com
 * @version  1.0
 */
public class SelectMajorActivity extends ExpandableListActivity
{
	// ����ʡ������
	private String[] Major = new String[]
	{ "�����", "������", "����","��ݮ","��Ϣ"};
	private String[][] majors = new String[][]
	{
		{ "�����15-1", "�����15-2", "�����15-3"},
		{ "������15-1", "������15-2", "������15-3"},
		{ "����15-1"},
		{"��ݮ15-1","��ݮ15-2"},
		{"��Ϣ15-1","�̻���151001215"}
	};

	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		ExpandableListAdapter adapter = new BaseExpandableListAdapter()
		{
			// ��ȡָ����λ�á�ָ�����б�������б�������
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

			// �÷�������ÿ����ѡ������
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
		// ���øô�����ʾ�б�
		setListAdapter(adapter);
		getExpandableListView().setOnChildClickListener(
			new OnChildClickListener()
			{
				@Override
				public boolean onChildClick(ExpandableListView parent,
						View source, int groupPosition, int childPosition,
						long id)
				{
					// ��ȡ������Activity֮ǰ��Activity��Ӧ��Intent
					Intent intent = getIntent();
					intent.putExtra("majors",
							majors[groupPosition][childPosition]);
					// ���ø�SelectActivity�Ľ���룬�����ý���֮���˻ص�Activity
					SelectMajorActivity.this.setResult(1, intent);//���ݸ�ActivityForResult.java��onActivityResult��resultCode
					// ����SelectCityActivity��**************************************************************************
					SelectMajorActivity.this.finish();
					return false;
				}
			});
	}
}
